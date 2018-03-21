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

#ifndef QLEGEND_H
#define QLEGEND_H

#include <QtCharts/QChartGlobal>
#include <QtWidgets/QGraphicsWidget>
#include <QtGui/QPen>
#include <QtGui/QBrush>

QT_CHARTS_BEGIN_NAMESPACE

class QChart;
class QLegendPrivate;
class QLegendMarker;
class QAbstractSeries;

// 图例类，负责抽象描述一项图列信息
class QT_CHARTS_EXPORT QLegend : public QGraphicsWidget
{
    Q_OBJECT
    Q_PROPERTY(Qt::Alignment alignment READ alignment WRITE setAlignment)
    Q_PROPERTY(bool backgroundVisible READ isBackgroundVisible WRITE setBackgroundVisible NOTIFY backgroundVisibleChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(QColor borderColor READ borderColor WRITE setBorderColor NOTIFY borderColorChanged)
    Q_PROPERTY(QFont font READ font WRITE setFont NOTIFY fontChanged)
    Q_PROPERTY(QColor labelColor READ labelColor WRITE setLabelColor NOTIFY labelColorChanged)
    Q_PROPERTY(bool reverseMarkers READ reverseMarkers WRITE setReverseMarkers NOTIFY reverseMarkersChanged)
    Q_PROPERTY(bool showToolTips READ showToolTips WRITE setShowToolTips NOTIFY showToolTipsChanged)
    Q_PROPERTY(MarkerShape markerShape READ markerShape WRITE setMarkerShape NOTIFY markerShapeChanged)

private:
    explicit QLegend(QChart *chart); // 构造

public:
    // 标记形状
    enum MarkerShape {
        MarkerShapeDefault,
        MarkerShapeRectangle,
        MarkerShapeCircle,
        MarkerShapeFromSeries
    };
    Q_ENUMS(MarkerShape)

    ~QLegend(); // 析构函数

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr); // 绘制

    void setBrush(const QBrush &brush); // 设置画刷
    QBrush brush() const; // 画刷
    void setColor(QColor color); // 设置颜色
    QColor color(); // 获取颜色

    void setPen(const QPen &pen); // 设置画笔
    QPen pen() const; // 获取画笔
    void setBorderColor(QColor color); // 设置边框颜色
    QColor borderColor(); // 获取边框颜色

    void setFont(const QFont &font); // 设置字体
    QFont font() const; // 获取字体
    void setLabelBrush(const QBrush &brush); // 设置标签画刷
    QBrush labelBrush() const; // 获取标签画刷

    void setLabelColor(QColor color); // 设置标签颜色
    QColor labelColor() const; // 返回标签颜色

    void setAlignment(Qt::Alignment alignment); // 设置停靠方式
    Qt::Alignment alignment() const; // 返回停靠方式

    void detachFromChart(); // 松绑表格
    void attachToChart(); // 绑定表格
    bool isAttachedToChart(); // 是否绑定表格

    void setBackgroundVisible(bool visible = true); // 设置背景是否可见
    bool isBackgroundVisible() const; // 背景是否可见

    QList <QLegendMarker*> markers(QAbstractSeries *series = nullptr) const; // 获取标记链表

    bool reverseMarkers(); // 是否为逆向链表
    void setReverseMarkers(bool reverseMarkers = true); // 设置逆向链表

    bool showToolTips() const; // 显示提示信息
    void setShowToolTips(bool show); // 设置显示提示

    MarkerShape markerShape() const; // 获取标记形状
    void setMarkerShape(MarkerShape shape); // 设置标记形状

protected:
    void hideEvent(QHideEvent *event); // 隐藏事件
    void showEvent(QShowEvent *event); // 显示事件

Q_SIGNALS:
    void backgroundVisibleChanged(bool visible);
    void colorChanged(QColor color);
    void borderColorChanged(QColor color);
    void fontChanged(QFont font);
    void labelColorChanged(QColor color);
    void reverseMarkersChanged(bool reverseMarkers);
    void showToolTipsChanged(bool showToolTips);
    void markerShapeChanged(MarkerShape shape);

private:
    QScopedPointer<QLegendPrivate> d_ptr; // 图列私有成员
    Q_DISABLE_COPY(QLegend)
    friend class LegendScroller;
    friend class LegendLayout;
    friend class ChartLayout;
    friend class LegendMarkerItem;
    friend class QLegendMarkerPrivate;
};

QT_CHARTS_END_NAMESPACE

#endif // QLEGEND_H
