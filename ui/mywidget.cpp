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

#include "mywidget.h"
#include "myitem.h"

#include <QPushButton>
#include <QDebug>

#include <QEvent>

MyWidget::MyWidget(const QString &title, const QString &icon, QWidget *parent)
  : QWidget(parent)
   , m_infoGroup(new MyGroup)
{
    m_icon = new QLabel;
    m_icon->setFixedSize(16, 16);

    m_title = new QLabel;
    m_title->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    m_title->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    this->setTitle(title);
    this->setIcon(icon);

    QHBoxLayout *titleLayout = new QHBoxLayout;
    titleLayout->setSpacing(0);
    titleLayout->setContentsMargins(5, 0, 0, 0);
    titleLayout->addWidget(m_icon);
    titleLayout->setAlignment(m_icon, Qt::AlignCenter);
    titleLayout->addWidget(m_title);

    QFrame *headerWidget = new QFrame;
    headerWidget->setStyleSheet("QFrame {padding: 2px 0;}"
                  "QFrame:hover {background-color: rgba(255, 255, 255, 0.2);border-radius: 2px;}");
    headerWidget->setLayout(titleLayout);

    m_centralLayout = new QVBoxLayout;
    m_centralLayout->addWidget(headerWidget);
    m_centralLayout->setSpacing(10);
    m_centralLayout->setMargin(0);

    m_centralLayout->addWidget(m_infoGroup);


    this->setLayout(m_centralLayout);
}

MyWidget::~MyWidget()
{
    for(int i=0; i<m_itemList.count(); i++) {
        MyItem *item = m_itemList.at(i);
        delete item;
        item = NULL;
    }
    m_itemList.clear();

    QList<MyItem *> items = findChildren<MyItem*>();
    for (MyItem *item : items) {
        m_infoGroup->removeItem(item);
        item->deleteLater();
    }

    if (m_infoGroup) {
        delete m_infoGroup;
        m_infoGroup = 0;
    }
}

void MyWidget::addOneInfoItem(const QString &key, const QString &value)
{
    bool isExists = false;

    MyItem *w;
    QList<MyItem *>::Iterator it = m_itemList.begin(), itend = m_itemList.end();
    for(;it !=itend;it++) {
        if ((*it)->title() == key ) {
            isExists = true;
            w = *it;
            break;
        }
    }

    if (!isExists) {
        w = new MyItem;
        m_itemList.append(w);
        m_infoGroup->appendItem(w);
    }
    w->setLineInfo(key, value);
}

const QString MyWidget::title() const
{
    return m_title->text();
}

void MyWidget::setTitle(const QString &title)
{
    m_title->setText(title);
}

void MyWidget::setIcon(const QString &icon)
{
    QString style = QString("QLabel{background-image: url(%1);}").arg(icon);
    this->m_icon->setStyleSheet(style);
}

bool MyWidget::event(QEvent *event)
{
    if (event->type() == QEvent::LayoutRequest)
        setFixedHeight(m_centralLayout->sizeHint().height());

    return QWidget::event(event);
}
