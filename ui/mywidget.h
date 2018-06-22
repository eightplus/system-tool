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

#ifndef MYWIDGET_H
#define MYWIDGET_H

#include "mygroup.h"

#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>

class MyWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MyWidget(const QString &title, const QString &icon, QWidget *parent = 0);
    ~MyWidget();

    const QString title() const;
    void setTitle(const QString &title);
    void setIcon(const QString &icon);

public slots:
    void addOneInfoItem(const QString &key, const QString &value);

private:
    bool event(QEvent *event);

private:
    QLabel *m_icon = nullptr;
    QLabel *m_title = nullptr;
    QVBoxLayout *m_centralLayout = nullptr;
    MyGroup *m_infoGroup = nullptr;
    QList<MyItem *> m_itemList;
};

#endif // MYWIDGET_H
