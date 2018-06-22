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

#include "infowidget.h"
#include "maindialog.h"
#include "myworker.h"
#include "mywidget.h"
#include "utils.h"

#include <QApplication>
#include <QDebug>

InfoWidget::InfoWidget(MainDialog *frame)
    : BaseWidget(frame)
    , m_refreshBtn(new QPushButton)
    , m_vLayout(new QVBoxLayout)
    , m_widget(new QFrame)
{
    setCenterWidget(m_widget);
    setTitle(tr("System info"));
//    setIcon(":/res/tool.png");

    m_refreshBtn->setText(tr("Refresh"));
    m_refreshBtn->setFixedWidth(200);
    m_vLayout->setSpacing(30);
    m_vLayout->setMargin(0);
    m_vLayout->addSpacing(10);
    m_vLayout->addWidget(m_refreshBtn, 0, Qt::AlignHCenter);
    m_vLayout->addSpacing(20);

    this->initWidgets();
    this->initData();
    this->initConnect();

    m_myWorker->refreshSystemInfo();
}

InfoWidget::~InfoWidget()
{
//    m_computerWidget->deleteLater();
//    m_fileSysWidget->deleteLater();
//    m_memoryWidget->deleteLater();

    delete m_myWorker;//m_myWorker->deleteLater();
    if (m_computerWidget) {
        delete m_computerWidget;
        m_computerWidget = 0;
    }
    if (m_fileSysWidget) {
        delete m_fileSysWidget;
        m_fileSysWidget = 0;
    }
    if (m_memoryWidget) {
        delete m_memoryWidget;
        m_memoryWidget = 0;
    }
    delete m_refreshBtn;
    delete m_vLayout;
    delete m_widget;
}

void InfoWidget::initWidgets()
{
    m_computerWidget = new MyWidget(tr("Computer"), ":/res/computer.png", this);
    m_vLayout->insertWidget(0, m_computerWidget);


    m_fileSysWidget = new MyWidget(tr("CPU"), ":/res/cpu.png", this);
    m_vLayout->insertWidget(1, m_fileSysWidget);

    m_memoryWidget = new MyWidget(tr("Memory"), ":/res/memory.png", this);
    m_vLayout->insertWidget(2, m_memoryWidget);

    m_widget->setLayout(m_vLayout);
}

void InfoWidget::initData()
{
    m_myWorker = new MyWorker();
    m_myWorker->moveToThread(qApp->thread());
}

void InfoWidget::initConnect()
{
    connect(m_refreshBtn, &QPushButton::clicked, this, [=] () {
        m_myWorker->refreshSystemInfo();
    });

    connect(m_myWorker, &MyWorker::sendInformation, this, [=] (int flag, QMap<QString, QString> values) {
        QMap<QString,QString>::iterator it;
        switch (flag) {
        case SystemInfoID::COMPUTER_INFO:
            for (it = values.begin(); it != values.end(); ++it) {
                m_computerWidget->addOneInfoItem(it.key(), it.value());
            }
            break;
        case SystemInfoID::CPU_INFO:
            for (it = values.begin(); it != values.end(); ++it) {
                m_fileSysWidget->addOneInfoItem(it.key(), it.value());
            }
            break;
        case SystemInfoID::MEMORY_INFO:
            for (it = values.begin(); it != values.end(); ++it) {
                m_memoryWidget->addOneInfoItem(it.key(), it.value());
            }
            break;
        default:
            break;
        }
    }, Qt::QueuedConnection);
}
