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

#include "myitem.h"

#include <QHBoxLayout>

MyItem::MyItem(QWidget *parent)
    : QFrame(parent)
    , m_title(new QLabel)
    , m_value(new QLabel)
{
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->setSpacing(0);
    mainLayout->setMargin(0);
    mainLayout->setContentsMargins(20, 0, 20, 0);
    mainLayout->addWidget(m_title);
    mainLayout->addStretch();
    mainLayout->addWidget(m_value);

    this->setFixedHeight(36);
    this->setLayout(mainLayout);
}

MyItem::~MyItem()
{
    delete m_title;
    delete m_value;
}

void MyItem::setLineInfo(const QString &title, const QString &value)
{
    m_title->setText(title);
    m_value->setText(value);
}
