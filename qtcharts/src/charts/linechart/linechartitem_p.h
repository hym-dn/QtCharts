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

#ifndef LINECHARTITEM_H
#define LINECHARTITEM_H

#include <QtCharts/QChartGlobal>
#include <private/xychart_p.h>
#include <QtCharts/QChart>
#include <QtGui/QPen>
#include <QtCharts/private/qchartglobal_p.h>

QT_CHARTS_BEGIN_NAMESPACE

class QLineSeries;
class ChartPresenter;

// 线图表项
class QT_CHARTS_PRIVATE_EXPORT LineChartItem :  public XYChart
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
public:
    explicit LineChartItem(QLineSeries *series, QGraphicsItem *item = 0); // 构造
    ~LineChartItem() {} // 析构

    //from QGraphicsItem
    QRectF boundingRect() const; // 外接矩形
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget); // 绘图
    QPainterPath shape() const; // 形状

    QPainterPath path() const { return m_fullPath; } // 绘制路径

public Q_SLOTS:
    void handleUpdated(); // 更新信号响应槽

protected:
    void updateGeometry(); // 更新几何图形
    void mousePressEvent(QGraphicsSceneMouseEvent *event); // 鼠标按下事件
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event); // 鼠标在其上事件
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event); // 鼠标离开事件
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event); // 鼠标释放事件
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event); // 鼠标双击事件
    void suppressPoints() { m_pointsVisible = false; } //  废除点集
    void forceChartType(QChart::ChartType chartType) { m_chartType = chartType; } // 强制类型

private:
    QLineSeries *m_series; // 所属序列
    QPainterPath m_linePath; // 线路径
    QPainterPath m_linePathPolarRight; // 极右路径
    QPainterPath m_linePathPolarLeft; // 极左路径
    QPainterPath m_fullPath; // 填充路径
    QPainterPath m_shapePath; // 形状路径

    QVector<QPointF> m_linePoints; // 线点集
    QRectF m_rect; // 矩形尺寸
    QPen m_linePen; // 线画笔
    bool m_pointsVisible; // 点集是否可见
    QChart::ChartType m_chartType; // 图表类型

    bool m_pointLabelsVisible; // 点标签是否可见
    QString m_pointLabelsFormat; // 点标签格式
    QFont m_pointLabelsFont; // 点标签字体
    QColor m_pointLabelsColor; // 点标签颜色
    bool m_pointLabelsClipping; // 点标签是否支持剪裁

    QPointF m_lastMousePos; // 鼠标最后位置
    bool m_mousePressed; // 鼠标按下
};

QT_CHARTS_END_NAMESPACE

#endif
