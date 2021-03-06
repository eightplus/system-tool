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

#ifndef FILESYSWIDGET_H
#define FILESYSWIDGET_H

#include <QMap>
#include <QVBoxLayout>
#include <QPushButton>

class FileSystemWorker;
class FileSystemListWidget;

class FileSysWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FileSysWidget(QWidget *widget);
    ~FileSysWidget();

    void initWidgets();
    void initData();
    void initConnect();

    void refreshFileSysList();

private:
    QPushButton *m_refreshBtn = nullptr;
    QVBoxLayout *m_vLayout = nullptr;
    FileSystemWorker *m_fileSystemWorker = nullptr;
    FileSystemListWidget *m_fileSysListWidget = nullptr;
};

#endif // FILESYSWIDGET_H
