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

#include "mygroup.h"
#include "myitem.h"

#include <QVBoxLayout>
#include <QEvent>
#include <QDebug>

MyGroup::MyGroup(QFrame *parent) :
    QFrame(parent),
    m_layout(new QVBoxLayout),
    m_updateHeightTimer(new QTimer(this))
{
    m_layout->setMargin(0);
    m_layout->setSpacing(1);

    this->setLayout(m_layout);

    m_updateHeightTimer->setSingleShot(true);
    m_updateHeightTimer->setInterval(10);

    connect(m_updateHeightTimer, &QTimer::timeout, this, [=] () {
        this->setFixedHeight(m_layout->sizeHint().height());
    }, Qt::QueuedConnection);
}

MyGroup::~MyGroup()
{
    const int index = 0;
    const int count = m_layout->count();

    for (int i(index); i != count; ++i) {
        QLayoutItem *item = m_layout->takeAt(index);
        QWidget *w = item->widget();
        w->removeEventFilter(this);
        w->setParent(nullptr);
        delete item;
    }

    delete m_layout;
    delete m_updateHeightTimer;
}

void MyGroup::appendItem(MyItem *item)
{
    m_layout->insertWidget(m_layout->count(), item);
    item->installEventFilter(this);

    m_updateHeightTimer->start();
}

void MyGroup::removeItem(MyItem *item)
{
    m_layout->removeWidget(item);
    item->removeEventFilter(this);

    m_updateHeightTimer->start();
}

bool MyGroup::eventFilter(QObject *, QEvent *event)
{
    switch (event->type())
    {
    case QEvent::Show:
    case QEvent::Hide:
    case QEvent::Resize:
        m_updateHeightTimer->start();
        break;
    default:
        break;
    }

    return false;
}
