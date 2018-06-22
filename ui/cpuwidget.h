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

#ifndef CPUWIDGET_H
#define CPUWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>

class CpuRateWidget;

class CpuWidget : public QWidget
{
    Q_OBJECT

public:
    CpuWidget(QWidget *parent = 0);
    ~CpuWidget();

    void refreshData(double cpu);
    void initWidgets();

public slots:
    void onUpdateCpuPercent();

private:
    QLabel *m_cpuRateTitle = nullptr;
    QLabel *m_cpuRateText = nullptr;
    QLabel *m_cpuIdleRateTitle = nullptr;
    QLabel *m_cpuIdleRateText = nullptr;
    QLabel *m_cpuRunTimeTitle = nullptr;
    QLabel *m_cpuRunTimeText = nullptr;
    QLabel *m_cpuIdleTimeTitle = nullptr;
    QLabel *m_cpuIdleTimeText = nullptr;
    CpuRateWidget *m_cpuBall = nullptr;
    QHBoxLayout *m_contentLayout = nullptr;
    QVBoxLayout *m_labelLayout = nullptr;
    QVBoxLayout *m_layout = nullptr;
    QTimer *m_updateTimer = nullptr;
    unsigned long long m_prevCpuTotalTime;
    unsigned long long m_prevCpuWorkTime;
    unsigned long long m_cpuTotalTime;
    unsigned long long m_cpuworkTime;
};

#endif // CPUWIDGET_H
