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

#ifndef MAINDIALOG_H
#define MAINDIALOG_H

#include <QDialog>
#include <QWidget>
#include <QStackedLayout>

class CpuWidget;
class InfoWidget;
class FileSysWidget;

class MainDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MainDialog(QWidget *parent = 0);
    ~MainDialog();

private:
    void keyPressEvent(QKeyEvent *e);

private:
    CpuWidget *m_cpuWidget = nullptr;
    InfoWidget *m_infoWidget = nullptr;
    FileSysWidget *m_fileSysWidget = nullptr;
    QPushButton *m_cpuBtn = nullptr;
    QPushButton *m_fsBtn = nullptr;
    QPushButton *m_infoBtn = nullptr;
    QWidget *m_displayFrame = nullptr;
    QStackedLayout *m_stackLayout = nullptr;
};

#endif // MAINDIALOG_H
