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

#ifndef BASEWIDGET_H
#define BASEWIDGET_H

#include <QWidget>
#include <QLabel>

class QVBoxLayout;
class QScrollArea;
class QPropertyAnimation;
class QPushButton;

class BaseWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BaseWidget(QWidget *parent = 0);

    void setTitle(const QString &title);
    void setIcon(const QString &picture);
    void setCenterWidget(QWidget * const widget);

protected:
    void wheelEvent(QWheelEvent *event);
    void mousePressEvent(QMouseEvent *event);

private:
    bool eventFilter(QObject *watched, QEvent *event);

protected:
    double m_defaultMaxSpeedTime;
    double m_speedTime;
    int m_speed;
    QLabel *m_title = nullptr;
//    QLabel *m_icon = nullptr;
    QVBoxLayout *m_contentTopLayout = nullptr;
    QScrollArea *m_contentArea = nullptr;
    QWidget *m_centerWidget = nullptr;
    QPropertyAnimation *m_animation = nullptr;

};

#endif // BASEWIDGET_H
