/*
 * Copyright (C) 2018 lixiang
 *
 * Authors:
 *  lixiang kobe24_lixiang@126.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "cpuwidget.h"
#include "cpuratewidget.h"

#include <QApplication>
#include <QDebug>
#include <QVBoxLayout>
#include <QFile>

unsigned long long getCpuTimeData(unsigned long long &workTime)
{
    FILE *file = fopen("/proc/stat", "r");
    if (file == NULL) {
        return 0;
    }

    char buffer[1024] = {0};
    unsigned long long user = 0, nice = 0, system = 0, idle = 0;
    unsigned long long iowait = 0, irq = 0, softirq = 0, steal = 0, guest = 0, guestnice = 0;

    char* ret = fgets(buffer, sizeof(buffer) - 1, file);
    if (ret == NULL) {
        fclose(file);
        return 0;
    }
    fclose(file);

    sscanf(buffer, "cpu  %16llu %16llu %16llu %16llu %16llu %16llu %16llu %16llu %16llu %16llu",
           &user, &nice, &system, &idle, &iowait, &irq, &softirq, &steal, &guest, &guestnice);
    workTime = user + nice + system;

    return user + nice + system + idle + iowait + irq + softirq + steal;
}

inline QString convertTimeToString(long seconds)
{
    int run_day = seconds / 86400;
    int run_hour = (seconds % 86400)/3600;
    int run_minute = (seconds % 3600)/60;
    int run_second = seconds % 60;

    QString hourStr;
    QString minuteStr;
    QString secondStr;

    if (run_hour >1)
        hourStr = QString(QObject::tr("%1hours")).arg(run_hour);
    else
        hourStr = QString(QObject::tr("%1hour")).arg(run_hour);
    if (run_minute > 1)
        minuteStr = QString(QObject::tr("%1minutes")).arg(run_minute);
    else
        minuteStr = QString(QObject::tr("%1minute")).arg(run_minute);
    if (run_second > 1)
        secondStr = QString(QObject::tr("%1seconds")).arg(run_second);
    else
        secondStr = QString(QObject::tr("%1second")).arg(run_second);

    QString run_time;
    if (run_day > 0) {
        if (run_day == 1)
            return QString("%1 %2 %3 %4").arg(QString(QObject::tr("%1day"))).arg(run_day).arg(hourStr).arg(minuteStr).arg(secondStr);
        else
            return QString("%1 %2 %3 %4").arg(QString(QObject::tr("%1days"))).arg(run_day).arg(hourStr).arg(minuteStr).arg(secondStr);
    }
    else {
        return QString("%1 %2 %3").arg(hourStr).arg(minuteStr).arg(secondStr);
    }

    return run_time;
}

inline int getCoreCounts()
{
    int cpuCounts = 0;

    QFile file("/proc/cpuinfo");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QString content = file.readLine().trimmed();
        while (!content.isEmpty() && content.contains(QChar(':'))) {
            const QStringList tokens = content.split(QChar(':'));
            if (tokens.size() == 2) {
                if (tokens[0] == "processor")
                    cpuCounts ++;
            }
            content = file.readLine().trimmed();
        }
        file.close();
    }

    if (cpuCounts == 0)
        cpuCounts = 4;

    return cpuCounts;
}

inline QString getIdelRate(unsigned long &runSeconds, unsigned long &idleSeconds)
{
    int cpuNumber = getCoreCounts();

    QString rate;
    QFile file("/proc/uptime");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QString content = file.readLine();
        while (!content.isEmpty() && content.contains(" ")) {
            QStringList tokens = content.split(" ");
            QString runStr = tokens.at(0);
            if (runStr.contains(QChar('.'))) {
                QString senconds = runStr.split(QChar('.')).at(0);
                runSeconds = senconds.toLong();
            }
            else
                runSeconds = runStr.toLong();

            QString idleStr = tokens.at(1);
            if (idleStr.contains(QChar('.'))) {
                QString senconds = idleStr.split(QChar('.')).at(0);
                idleSeconds = senconds.toLong();
            }
            else
                idleSeconds = idleStr.toLong();
            rate = QString::number((idleSeconds * 1.0) /(runSeconds *1.0 * cpuNumber) * 100, 'f', 0) + "%";
            break;
        }
        file.close();
    }
    return rate;
}

CpuWidget::CpuWidget(QWidget *parent) : QWidget(parent)
{
    m_cpuTotalTime = 0;
    m_cpuworkTime = 0;
    m_prevCpuTotalTime = 0;
    m_prevCpuWorkTime = 0;

    m_layout = new QVBoxLayout(this);
    m_layout->setContentsMargins(0, 0, 0, 0);

    QWidget *w = new QWidget;
    m_contentLayout = new QHBoxLayout(w);
    m_contentLayout->setContentsMargins(0, 0, 0, 0);
    m_contentLayout->setSpacing(50);
    m_layout->addWidget(w, 0, Qt::AlignCenter);

    initWidgets();

    m_updateTimer = new QTimer(this);
    connect(m_updateTimer, SIGNAL(timeout()), this, SLOT(onUpdateCpuPercent()));
    m_updateTimer->start(2000);
}

CpuWidget::~CpuWidget()
{
    if (m_updateTimer) {
        disconnect(m_updateTimer, SIGNAL(timeout()), this, SLOT(onUpdateCpuPercent()));
        if(m_updateTimer->isActive()) {
            m_updateTimer->stop();
        }
        delete m_updateTimer;
        m_updateTimer = nullptr;
    }

    delete m_cpuRateTitle;
    delete m_cpuRateText;
    delete m_cpuIdleRateTitle;
    delete m_cpuIdleRateText;
    delete m_cpuRunTimeTitle;
    delete m_cpuRunTimeText;
    delete m_cpuIdleTimeTitle;
    delete m_cpuIdleTimeText;

    delete m_cpuBall;
    QLayoutItem *child;
    while ((child = m_labelLayout->takeAt(0)) != 0) {
        if (child->widget())
            child->widget()->deleteLater();
        delete child;
    }
    while ((child = m_contentLayout->takeAt(0)) != 0) {
        if (child->widget())
            child->widget()->deleteLater();
        delete child;
    }
    delete m_layout;
}

void CpuWidget::initWidgets()
{
    QWidget *w = new QWidget;
    m_labelLayout = new QVBoxLayout(w);
    m_labelLayout->setContentsMargins(0, 0, 0, 0);
    m_labelLayout->setSpacing(0);

    QLabel *m_title = new QLabel(tr("CPU"));
    m_title->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    m_title->setStyleSheet("background:transparent;font-size:24px;color:#000000");

    m_cpuRateTitle = new QLabel;
    m_cpuRateTitle->setStyleSheet("QLabel{background:transparent;font-size:12px;color:#999999;}");
    m_cpuRateTitle->setText(tr("Occupancy rate"));
    m_cpuRateText = new QLabel;
    m_cpuRateText->setStyleSheet("QLabel{background:transparent;font-size:14px;color:#000000;}");

    m_cpuIdleRateTitle = new QLabel;
    m_cpuIdleRateTitle->setStyleSheet("QLabel{background:transparent;font-size:12px;color:#999999;}");
    m_cpuIdleRateTitle->setText(tr("Idle rate"));
    m_cpuIdleRateText = new QLabel;
    m_cpuIdleRateText->setStyleSheet("QLabel{background:transparent;font-size:14px;color:#000000;}");

    m_cpuRunTimeTitle = new QLabel;
    m_cpuRunTimeTitle->setStyleSheet("QLabel{background:transparent;font-size:12px;color:#999999;}");
    m_cpuRunTimeTitle->setText(tr("The running time of system"));
    m_cpuRunTimeText = new QLabel;
    m_cpuRunTimeText->setStyleSheet("QLabel{background:transparent;font-size:14px;color:#000000;}");

    m_cpuIdleTimeTitle = new QLabel;
    m_cpuIdleTimeTitle->setStyleSheet("QLabel{background:transparent;font-size:12px;color:#999999;}");
    m_cpuIdleTimeTitle->setText(tr("The idle time of system"));
    m_cpuIdleTimeText = new QLabel;
    m_cpuIdleTimeText->setStyleSheet("QLabel{background:transparent;font-size:14px;color:#000000;}");

    QVBoxLayout *cpuRateLayout = new QVBoxLayout;
    cpuRateLayout->setSpacing(10);
    cpuRateLayout->addWidget(m_cpuRateTitle);
    cpuRateLayout->addWidget(m_cpuRateText);

    QVBoxLayout *cpuIdleRateLayout = new QVBoxLayout;
    cpuIdleRateLayout->setSpacing(10);
    cpuIdleRateLayout->addWidget(m_cpuIdleRateTitle);
    cpuIdleRateLayout->addWidget(m_cpuIdleRateText);

    QHBoxLayout *rateLayout = new QHBoxLayout;
    rateLayout->setSpacing(30);
    rateLayout->addLayout(cpuRateLayout);
    rateLayout->addLayout(cpuIdleRateLayout);

    QVBoxLayout *cpuRunTimeLayout = new QVBoxLayout;
    cpuRunTimeLayout->setSpacing(10);
    cpuRunTimeLayout->addWidget(m_cpuRunTimeTitle);
    cpuRunTimeLayout->addWidget(m_cpuRunTimeText);

    QVBoxLayout *cpuIdleTimeLayout = new QVBoxLayout;
    cpuIdleTimeLayout->setSpacing(10);
    cpuIdleTimeLayout->addWidget(m_cpuIdleTimeTitle);
    cpuIdleTimeLayout->addWidget(m_cpuIdleTimeText);

    m_labelLayout->setContentsMargins(0, 0, 0, 0);
    m_labelLayout->setSpacing(10);
    m_labelLayout->addWidget(m_title);
    m_labelLayout->addLayout(rateLayout);
    m_labelLayout->addLayout(cpuRunTimeLayout);
    m_labelLayout->addLayout(cpuIdleTimeLayout);
    m_contentLayout->addWidget(w, 1, Qt::AlignLeft);

    m_cpuBall = new CpuRateWidget;
    m_contentLayout->addWidget(m_cpuBall);
}

void CpuWidget::refreshData(double cpu)
{
    m_cpuRateText->setText(QString::number(cpu, 'f', 1) + "%");

    unsigned long runtime;
    unsigned long idletime;
    QString rate = getIdelRate(runtime, idletime);
    m_cpuIdleRateText->setText(rate);
    m_cpuRunTimeText->setText(convertTimeToString(runtime));
    m_cpuIdleTimeText->setText(convertTimeToString(idletime));
}

void CpuWidget::onUpdateCpuPercent()
{
    double value = 0.0;
    m_prevCpuWorkTime = m_cpuworkTime;
    m_prevCpuTotalTime = m_cpuTotalTime;
    m_cpuTotalTime = getCpuTimeData(m_cpuworkTime);
    if (m_prevCpuWorkTime != 0 && m_prevCpuTotalTime != 0) {
        value = (m_cpuworkTime - m_prevCpuWorkTime) * 100.0 / (m_cpuTotalTime - m_prevCpuTotalTime);
    }

    this->refreshData(value);
    m_cpuBall->updateCpuPercent(value);
}
