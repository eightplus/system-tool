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

#include "filesyswidget.h"
#include "utils.h"
#include "filesystemlistitem.h"
#include "filesystemlistwidget.h"
#include "filesystemworker.h"

#include <QApplication>
#include <QDebug>

FileSysWidget::FileSysWidget(QWidget *parent)
    : QWidget(parent)
    , m_refreshBtn(new QPushButton)
    , m_vLayout(new QVBoxLayout)
{
    m_refreshBtn->setFixedWidth(200);
    m_refreshBtn->setText(tr("Refresh"));
    m_vLayout->setSpacing(30);
    m_vLayout->setMargin(0);
    m_vLayout->addSpacing(10);
    m_vLayout->addWidget(m_refreshBtn, 0, Qt::AlignHCenter);
    m_vLayout->addSpacing(20);

    this->initWidgets();
    this->initData();
    this->initConnect();
}

FileSysWidget::~FileSysWidget()
{
    m_fileSystemWorker->deleteLater();
    if (m_fileSysListWidget) {
        delete m_fileSysListWidget;
        m_fileSysListWidget = 0;
    }
    delete m_refreshBtn;
    delete m_vLayout;
}

void FileSysWidget::initWidgets()
{
    m_fileSysListWidget = new FileSystemListWidget(this);
    m_vLayout->insertWidget(0, m_fileSysListWidget);
    this->setLayout(m_vLayout);
}

void FileSysWidget::initData()
{   
    m_fileSystemWorker = new FileSystemWorker;
    m_fileSystemWorker->moveToThread(qApp->thread());

    this->refreshFileSysList();
}

void FileSysWidget::initConnect()
{
    connect(m_refreshBtn, &QPushButton::clicked, this, [=] () {
        this->refreshFileSysList();
    });
}

void FileSysWidget::refreshFileSysList()
{
    m_fileSystemWorker->onFileSystemListChanged();

    QList<FileSystemListItem*> items;
    for (FileSystemData *info : m_fileSystemWorker->diskInfoList()) {
        FileSystemListItem *item = new FileSystemListItem(info);
        items << item;
    }
    m_fileSysListWidget->refreshFileSystemItems(items);
}
