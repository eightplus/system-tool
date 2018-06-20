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

#ifndef INFOWIDGET_H
#define INFOWIDGET_H

#include "basewidget.h"

#include <QMap>
#include <QVBoxLayout>
#include <QPushButton>

class MyWidget;
class MyWorker;
class MainDialog;

class InfoWidget : public BaseWidget
{
    Q_OBJECT

public:
    explicit InfoWidget(MainDialog *frame);
    ~InfoWidget();

    void initWidgets();
    void initData();
    void initConnect();

private:
    QPushButton *m_refreshBtn = nullptr;
    QVBoxLayout *m_vLayout = nullptr;
    QFrame *m_widget = nullptr;
    MyWidget *m_computerWidget = nullptr;
    MyWidget *m_cpuWidget = nullptr;
    MyWidget *m_memoryWidget = nullptr;
    MyWorker *m_myWorker = nullptr;
};

#endif // INFOWIDGET_H
