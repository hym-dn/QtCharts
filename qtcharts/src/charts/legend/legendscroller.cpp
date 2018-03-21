/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt Charts module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 or (at your option) any later version
** approved by the KDE Free Qt Foundation. The licenses are as published by
** the Free Software Foundation and appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtCore/QDebug>
#include <QtWidgets/QGraphicsSceneMouseEvent>
#include <QtWidgets/QGraphicsScene>
#include <QtCharts/QLegendMarker>
#include <private/qlegendmarker_p.h>
#include <private/legendmarkeritem_p.h>
#include <private/legendscroller_p.h>

QT_CHARTS_BEGIN_NAMESPACE

// 构造函数
LegendScroller::LegendScroller(QChart *chart) : QLegend(chart)
{
}

// 设置偏移量
void LegendScroller::setOffset(const QPointF &point)
{
    d_ptr->setOffset(point);
}

// 获取偏移量
QPointF LegendScroller::offset() const
{
    return d_ptr->offset();
}

// 鼠标按下事件
void LegendScroller::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Scroller::handleMousePressEvent(event);
}

// 鼠标移动事件
void LegendScroller::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    Scroller::handleMouseMoveEvent(event);
}

// 鼠标释放事件
void LegendScroller::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    // 鼠标释放事件
    Scroller::handleMouseReleaseEvent(event);
    // 事件尚未被接收
    if (!event->isAccepted()) {
        // 取定点击标记
        QList<QGraphicsItem *> items = scene()->items(event->scenePos());

        foreach (QGraphicsItem *i, items) {
            if (d_ptr->m_markerHash.contains(i)) {
                QLegendMarker *marker = d_ptr->m_markerHash.value(i);
                emit marker->clicked();
            }
        }
        // 接收事件
    event->accept();
    }
}


#include "moc_legendscroller_p.cpp"
QT_CHARTS_END_NAMESPACE
