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

#ifndef CPURATEWIDGET_H
#define CPURATEWIDGET_H

#include <QWidget>
#include <QTimer>

class QGraphicsDropShadowEffect;

class CpuRateWidget : public QWidget
{
    Q_OBJECT

public:
    CpuRateWidget(QWidget *parent = 0);
    ~CpuRateWidget();

    void updateCpuPercent(double value);

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    QGraphicsDropShadowEffect *m_shadowEffect = nullptr;
    QString m_progressText;
    double m_prevPercentValue;
    double m_percentValue;
};

#endif // CPURATEWIDGET_H
