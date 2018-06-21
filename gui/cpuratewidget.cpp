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

#include "cpuratewidget.h"

#include <QApplication>
#include <QDebug>
#include <QPainter>
#include <QGraphicsDropShadowEffect>

CpuRateWidget::CpuRateWidget(QWidget *parent) : QWidget(parent)
{
    this->setFixedSize(210, 210);

    m_prevPercentValue = 0.0;
    m_percentValue = 0.0;
    m_progressText = QString("%1%").arg(QString::number(m_percentValue, 'f', 1));

    m_shadowEffect = new QGraphicsDropShadowEffect(this);
    m_shadowEffect->setOffset(0, 3);
    m_shadowEffect->setColor(QColor(232, 232, 232, 127));
    m_shadowEffect->setBlurRadius(10);
    this->setGraphicsEffect(m_shadowEffect);
}

CpuRateWidget::~CpuRateWidget()
{
    delete m_shadowEffect;
}

void CpuRateWidget::updateCpuPercent(double value)
{
    if (this->m_percentValue == value || value > 100 || value < 0) {
        return;
    }
    m_prevPercentValue = m_percentValue;
    m_percentValue = value;
    m_progressText = QString("%1%").arg(QString::number(value, 'f', 1));
}

void CpuRateWidget::paintEvent(QPaintEvent *)
{
    int currentPercent = static_cast<int>(m_percentValue);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    QRectF rect = QRectF(0, 0, this->width(), this->height());
    QSize ball_size = this->size();

    QImage ball_rectImage = QImage(ball_size, QImage::Format_ARGB32_Premultiplied);
    QPainter ball_painter(&ball_rectImage);
    ball_painter.setRenderHint(QPainter::Antialiasing, true);
    ball_painter.setCompositionMode(QPainter::CompositionMode_Source);
    if (currentPercent > 88) {
        m_shadowEffect->setColor(QColor(255, 0, 0, 127));//red
    } else if (currentPercent > 55) {
        m_shadowEffect->setColor(QColor(255, 193, 37, 127));//yellow
    } else {
        m_shadowEffect->setColor(QColor(232, 232, 232, 127));//gray
    }
    ball_painter.fillRect(ball_rectImage.rect(), QColor(255, 255, 255, 127));

    QRectF outRect = QRectF(0, 0, ball_size.width(), ball_size.height());
    QConicalGradient conicalGradient(ball_size.width()/2, ball_size.height()/2, ball_size.width());
    conicalGradient.setColorAt(0, QColor("#59aee2"));
    conicalGradient.setColorAt(1.0, QColor("#0f84bc"));
    ball_painter.setPen(QPen(QBrush(conicalGradient), 1));
    ball_painter.drawEllipse(outRect.marginsRemoved(QMarginsF(0.5, 0.5, 0.5, 0.5)));

    QFont font = ball_painter.font();
    font.setPixelSize(44);
    ball_painter.setFont(font);
    ball_painter.setPen(Qt::black);
    ball_painter.drawText(QRect(rect.x(), rect.y() + rect.height()*1/3, rect.width(), rect.height()), Qt::AlignHCenter, m_progressText);
    ball_painter.end();

    QImage image = QImage(ball_size, QImage::Format_ARGB32_Premultiplied);
    QPainter m_painter(&image);
    m_painter.setCompositionMode(QPainter::CompositionMode_Source);
    m_painter.fillRect(image.rect(), Qt::transparent);

    QPixmap maskPixmap(ball_size);
    maskPixmap.fill(Qt::transparent);
    QPainterPath path;
    path.addEllipse(QRectF(0, 0, ball_size.width(), ball_size.height()));
    QPainter maskPainter(&maskPixmap);
    maskPainter.setRenderHint(QPainter::Antialiasing, true);
    maskPainter.setPen(QPen(Qt::white, 1));
    maskPainter.fillPath(path, QBrush(Qt::white));
    m_painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    m_painter.drawImage(0, 0, maskPixmap.toImage());

    m_painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
    m_painter.drawImage(0, 0, ball_rectImage);

    m_painter.setCompositionMode(QPainter::CompositionMode_DestinationOver);
    m_painter.end();

    painter.drawImage(this->rect(), image);
}
