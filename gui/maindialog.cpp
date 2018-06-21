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

#include "maindialog.h"
#include "cpuwidget.h"
#include "filesyswidget.h"
#include "infowidget.h"

#include <QApplication>
#include <QKeyEvent>

MainDialog::MainDialog(QWidget *parent)
    : QDialog(parent)
{
    resize(500, 680);

    m_cpuBtn = new QPushButton(this);
    m_cpuBtn->setText(tr("CPU"));
    m_fsBtn = new QPushButton(this);
    m_fsBtn->setText(tr("File System"));
    m_infoBtn = new QPushButton(this);
    m_infoBtn->setText(tr("Sys Info"));
    m_cpuWidget = new CpuWidget(this);
    m_fileSysWidget = new FileSysWidget(this);
    m_infoWidget = new InfoWidget(this);

    QHBoxLayout *btn_layout = new QHBoxLayout;
    btn_layout->addStretch();
    btn_layout->addWidget(m_cpuBtn);
    btn_layout->addWidget(m_fsBtn);
    btn_layout->addWidget(m_infoBtn);
    btn_layout->addStretch();

    m_stackLayout = new QStackedLayout,
    m_stackLayout->setMargin(0);
    m_stackLayout->setSpacing(0);
    m_displayFrame = new QWidget;
    m_displayFrame->setLayout(m_stackLayout);

    m_cpuWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_fileSysWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_infoWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_stackLayout->addWidget(m_cpuWidget);
    m_stackLayout->addWidget(m_fileSysWidget);
    m_stackLayout->addWidget(m_infoWidget);
    m_stackLayout->setCurrentIndex(0);

    QVBoxLayout *main_layout = new QVBoxLayout(this);
    main_layout->addLayout(btn_layout);
    main_layout->addSpacing(10);
//    main_layout->addWidget(m_infoWidget);
    main_layout->addWidget(m_displayFrame);
    main_layout->setContentsMargins(0,0,0,0);

    connect(m_cpuBtn, &QPushButton::clicked, this, [=] () {
        m_stackLayout->setCurrentIndex(0);
    });

    connect(m_fsBtn, &QPushButton::clicked, this, [=] () {
        m_stackLayout->setCurrentIndex(1);
    });

    connect(m_infoBtn, &QPushButton::clicked, this, [=] () {
        m_stackLayout->setCurrentIndex(2);
    });
}

MainDialog::~MainDialog()
{
    m_cpuWidget->deleteLater();
    m_cpuWidget = 0;

    m_infoWidget->deleteLater();
    m_infoWidget = 0;

    m_fileSysWidget->deleteLater();
    m_fileSysWidget = 0;

    delete m_cpuBtn;
    delete m_fsBtn;
    delete m_infoBtn;
    delete m_stackLayout;
    delete m_displayFrame;
}

void MainDialog::keyPressEvent(QKeyEvent *e)
{
    switch (e->key()) {
    case Qt::Key_Escape:
        qApp->quit();
        break;
    default:
        break;
    }
}
