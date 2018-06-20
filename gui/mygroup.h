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

#ifndef MYGROUP_H
#define MYGROUP_H

#include <QFrame>
#include <QTimer>
#include <QFrame>

class QVBoxLayout;
class MyItem;

class MyGroup : public QFrame
{
    Q_OBJECT

public:
    explicit MyGroup(QFrame *parent = 0);
    ~MyGroup();

    void appendItem(MyItem * item);
    void removeItem(MyItem * item);

private:
    bool eventFilter(QObject *, QEvent *event);

private:
    QVBoxLayout *m_layout = nullptr;
    QTimer *m_updateHeightTimer = nullptr;
};


#endif // MYGROUP_H
