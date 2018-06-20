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

#include "basewidget.h"

#include <QVBoxLayout>
#include <QPushButton>
#include <QResizeEvent>
#include <QTimer>
#include <QDebug>
#include <QScroller>
#include <QScrollBar>
#include <QScrollArea>
#include <QApplication>
#include <QWheelEvent>
#include <QPropertyAnimation>

BaseWidget::BaseWidget(QWidget *parent)
    : QWidget(parent)
    , m_defaultMaxSpeedTime(15)
    , m_speedTime(1)
{
    m_title = new QLabel;
    m_title->setAlignment(Qt::AlignCenter);
    m_title->setStyleSheet("QLabel{color:#000000;font-size:18px;text-align:center;}");
//    m_icon = new QLabel;

    m_contentTopLayout = new QVBoxLayout;
    m_contentTopLayout->setMargin(0);
    m_contentTopLayout->setSpacing(0);

    m_contentArea = new QScrollArea;
    m_contentArea->setWidgetResizable(true);
    m_contentArea->installEventFilter(this);
    m_contentArea->setFrameStyle(QFrame::NoFrame);
    m_contentArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_contentArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_contentArea->viewport()->installEventFilter(this);
    QScroller::grabGesture(m_contentArea, QScroller::LeftMouseButtonGesture);

    m_animation = new QPropertyAnimation(m_contentArea->verticalScrollBar(), "value");
    m_animation->setEasingCurve(QEasingCurve::OutQuint);
    m_animation->setDuration(1500);
    connect(m_animation, &QPropertyAnimation::finished, this, [=] {
        m_animation->setEasingCurve(QEasingCurve::OutQuint);
        m_animation->setDuration(1500);
    });

    /*QWidget *w = new QWidget;
    w->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    QHBoxLayout *titleLayout = new QHBoxLayout(w);
    titleLayout->setContentsMargins(0, 0, 0, 0);
    titleLayout->setSpacing(0);
    titleLayout->addWidget(m_icon, 0 , Qt::AlignHCenter);
    titleLayout->addWidget(m_title, 0 , Qt::AlignHCenter);*/

    QHBoxLayout *titleLayout = new QHBoxLayout;
    titleLayout->addWidget(m_title);

    QVBoxLayout *mainLayout = new QVBoxLayout;
//    mainLayout->addWidget(w);
    mainLayout->addLayout(titleLayout);
    mainLayout->addSpacing(15);
    mainLayout->addLayout(m_contentTopLayout);
    mainLayout->addWidget(m_contentArea);
    mainLayout->setContentsMargins(8, 8, 8, 0);
    mainLayout->setSpacing(0);

    this->setLayout(mainLayout);
}

void BaseWidget::setTitle(const QString &title)
{
    m_title->setText(title);
}

void BaseWidget::setIcon(const QString &picture)
{
//    QImage image(picture);
//    image = image.scaled(QSize(32, 32), Qt::KeepAspectRatio, Qt::SmoothTransformation);
//    m_icon->setPixmap(QPixmap::fromImage(image));

    /*m_icon->setPixmap(QPixmap(picture));
//    m_icon->setScaledContents(true);//自动缩放,显示图像大小自动调整为Qlabel大小*/
}

void BaseWidget::setCenterWidget(QWidget * const widget)
{
    m_centerWidget = widget;
    m_centerWidget->installEventFilter(this);
    m_centerWidget->setFixedWidth(m_contentArea->width());
    m_contentArea->setWidget(m_centerWidget);
}

bool BaseWidget::eventFilter(QObject *obj, QEvent *event)
{
    if (m_centerWidget && obj == m_contentArea && event->type() == QEvent::Resize)
        m_centerWidget->setFixedWidth(static_cast<QResizeEvent *>(event)->size().width());

    if (m_centerWidget && obj == m_contentArea->viewport() && event->type() == QEvent::Wheel) {
        const QWheelEvent *wheel = static_cast<QWheelEvent*>(event);
        QWheelEvent *newEvent =  new QWheelEvent(wheel->pos(), wheel->delta(), wheel->buttons(),  wheel->modifiers(), wheel->orientation());
        qApp->postEvent(this, newEvent);

        return true;
    }

    if (obj == m_centerWidget && event->type() == QEvent::LayoutRequest) {
        if (m_centerWidget->hasHeightForWidth())
            m_centerWidget->setMinimumHeight(m_centerWidget->heightForWidth(m_centerWidget->width()));
        else
            m_centerWidget->setFixedHeight(m_centerWidget->layout()->sizeHint().height());
    }

    return false;
}

void BaseWidget::wheelEvent(QWheelEvent *event)
{
    if (event->pixelDelta().y() != 0) {//touchpad
        QWheelEvent we(event->pos(), event->globalPos(), event->pixelDelta()
                       , event->angleDelta(), event->delta() * 4 , Qt::Vertical, event->buttons(), event->modifiers());
        QWidget::wheelEvent(&we);
    }
    else {//mouse
        int offset = - event->delta();
        if (m_animation->state() == QPropertyAnimation::Running) {
            m_speedTime += 0.2;
            if (m_speed != offset) {
                m_speed = offset;
                m_speedTime = 1;
            }
        }
        else {
            m_speedTime = 1;
        }

        m_animation->stop();
        m_animation->setStartValue(m_contentArea->verticalScrollBar()->value());
        m_animation->setEndValue(m_contentArea->verticalScrollBar()->value() + offset * qMin(m_speedTime, m_defaultMaxSpeedTime));

        m_animation->start();
    }
}

void BaseWidget::mousePressEvent(QMouseEvent *event)
{
    //stop scroll
    m_speedTime = 1;
    m_animation->stop();

    QWidget::mousePressEvent(event);
}
