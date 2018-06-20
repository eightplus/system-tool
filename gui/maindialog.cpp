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
#include "infowidget.h"

#include <QApplication>
#include <QKeyEvent>

MainDialog::MainDialog(QWidget *parent)
    : QDialog(parent)
{
    resize(500, 680);
    m_infoWidget = new InfoWidget(this);

    QVBoxLayout *main_layout = new QVBoxLayout(this);
    main_layout->addWidget(m_infoWidget);
    main_layout->setContentsMargins(0,0,0,0);
}

MainDialog::~MainDialog()
{
    m_infoWidget->deleteLater();
    m_infoWidget = 0;
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
