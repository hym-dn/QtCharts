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

//  W A R N I N G
//  -------------
//
// This file is not part of the Qt Chart API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.

#ifndef AREACHARTITEM_H
#define AREACHARTITEM_H

#include <QtCharts/QChartGlobal>
#include <private/linechartitem_p.h>
#include <QtCharts/private/qchartglobal_p.h>
#include <QtCharts/QAreaSeries>
#include <QtGui/QPen>

QT_CHARTS_BEGIN_NAMESPACE

class AreaChartItem;

// 区域图表项
class QT_CHARTS_PRIVATE_EXPORT AreaChartItem : public ChartItem
{
    Q_OBJECT
public:
    AreaChartItem(QAreaSeries *areaSeries, QGraphicsItem* item = 0); // 构造
    ~AreaChartItem(); // 析构

    //from QGraphicsItem
    QRectF boundingRect() const; // 外接矩形
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget); // 绘图
    QPainterPath shape() const; // 形状

    LineChartItem *upperLineItem() const { return m_upper; } // 上限图形项
    LineChartItem *lowerLineItem() const { return m_lower; } // 下限图形项

    void updatePath(); // 更新路径

    void setPresenter(ChartPresenter *presenter); // 设置主持
    QAreaSeries *series() const { return m_series; } // 获取序列

    void setUpperSeries(QLineSeries *series); // 设置上界序列
    void setLowerSeries(QLineSeries *series); // 设置下界序列

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event); // 鼠标按下事件
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event); // 鼠标进入事件
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event); // 鼠标离开事件
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event); // 鼠标释放事件
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event); // 鼠标双击事件

Q_SIGNALS:
    void clicked(const QPointF &point);
    void hovered(const QPointF &point, bool state);
    void pressed(const QPointF &point);
    void released(const QPointF &point);
    void doubleClicked(const QPointF &point);

public Q_SLOTS:
    void handleUpdated(); // 更新信号响应槽
    void handleDomainUpdated(); // 区域更新响应槽

private:
    void fixEdgeSeriesDomain(LineChartItem *edgeSeries); // 修复边界序列区域

    QAreaSeries *m_series; // 所属序列
    LineChartItem *m_upper; // 上限图表项
    LineChartItem *m_lower; // 下限图表项
    QPainterPath m_path; // 路径
    QRectF m_rect; // 尺寸
    QPen m_linePen; // 线画笔
    QPen m_pointPen; // 电画笔
    QBrush m_brush; // 画刷
    bool m_pointsVisible; // 点是否可见

    bool m_pointLabelsVisible; // 点标签是否可见
    QString m_pointLabelsFormat; // 点标签格式
    QFont m_pointLabelsFont; // 点标签字体
    QColor m_pointLabelsColor; // 点标签颜色
    bool m_pointLabelsClipping; // 点标签是否可剪裁

    QPointF m_lastMousePos; // 最后鼠标位置
    bool m_mousePressed; // 鼠标是否被按下

};

// 区域范围项目
class QT_CHARTS_PRIVATE_EXPORT AreaBoundItem : public LineChartItem
{
public:
    // 构造
    AreaBoundItem(AreaChartItem *area, QLineSeries *lineSeries,QGraphicsItem* item = 0)
        : LineChartItem(lineSeries, item), m_item(area)
    {
        // We do not actually want to draw anything from LineChartItem.
        // Drawing is done in AreaChartItem only.
        setVisible(false);
    }
    // 析构
    ~AreaBoundItem() {}

    // 更新几何图形
    void updateGeometry()
    {
        // Make sure the series is in a chart before trying to update
        if (m_item->series()->chart()) { // 图表存在
            // Turn off points drawing from component line chart item, as that
            // messes up the fill for area series.
            suppressPoints(); // 阻止点集显示
            // Component lineseries are not necessarily themselves on the chart,
            // so get the chart type for them from area chart.
            forceChartType(m_item->series()->chart()->chartType()); // 强制设置图表类型
            // 更新几何
            LineChartItem::updateGeometry();
            // 更新路径
            m_item->updatePath();
        }
    }

private:
    AreaChartItem *m_item; // 所属图表区域项
};

QT_CHARTS_END_NAMESPACE

#endif
