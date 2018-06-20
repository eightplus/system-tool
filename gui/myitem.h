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

#ifndef MYITEM_H
#define MYITEM_H

#include <QFrame>
#include <QLabel>

class MyItem : public QFrame
{
    Q_OBJECT

public:
    explicit MyItem(QWidget *parent = 0);
    ~MyItem();

    inline QString title() const { return m_title->text(); }
    void setLineInfo(const QString &title, const QString &value);

private:
    QLabel *m_title = nullptr;
    QLabel *m_value = nullptr;
};

#endif // MYITEM_H
