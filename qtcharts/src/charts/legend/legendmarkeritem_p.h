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

#ifndef LEGENDMARKERITEM_P_H
#define LEGENDMARKERITEM_P_H

#include <QtCharts/QChartGlobal>
#include <QtCharts/QLegend>
#include <QGraphicsObject>
#include <QtGui/QFont>
#include <QtGui/QBrush>
#include <QtGui/QPen>
#include <QtWidgets/QGraphicsTextItem>
#include <QtWidgets/QGraphicsLayoutItem>
#include <QtCharts/private/qchartglobal_p.h>

QT_CHARTS_BEGIN_NAMESPACE

class QLegendMarkerPrivate;

// 图例中的标记项，描述图例中一项标记
class QT_CHARTS_PRIVATE_EXPORT LegendMarkerItem : public QGraphicsObject, public QGraphicsLayoutItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsLayoutItem)
public:
    enum ItemType { // 标记类型
        TypeRect, // 矩形
        TypeLine, // 线
        TypeCircle // 圆圈
    };

    explicit LegendMarkerItem(QLegendMarkerPrivate *marker, QGraphicsObject *parent = nullptr); // 构造
    ~LegendMarkerItem(); // 析构

    void setPen(const QPen &pen); // 设置画笔
    QPen pen() const; // 获取画笔

    void setBrush(const QBrush &brush); // 设置画刷
    QBrush brush() const; // 返回画刷

    void setSeriesPen(const QPen &pen); // 设置序列画笔
    void setSeriesBrush(const QBrush &brush); // 设置序列画刷

    void setFont(const QFont &font); // 设置字体
    QFont font() const; // 返回字体

    void setLabel(const QString label); // 设置标签
    QString label() const; // 返回标签

    void setLabelBrush(const QBrush &brush); // 设置标签画刷
    QBrush labelBrush() const; // 返回标签画刷

    void setGeometry(const QRectF &rect); // 设置几何尺寸
    QRectF boundingRect() const; // 返回外接矩形尺寸
    QRectF markerRect() const; // 标记尺寸

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget = nullptr); // 绘制
    QSizeF sizeHint (Qt::SizeHint which, const QSizeF &constraint) const; // 默认尺寸

    void hoverEnterEvent(QGraphicsSceneHoverEvent *event); // 鼠标进入事件
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event); // 鼠标离去事件

    QString displayedLabel() const; // 显示标签
    void setToolTip(const QString &tooltip); // 设置提示气泡

    QLegend::MarkerShape markerShape() const; // 获取标记形状
    void setMarkerShape(QLegend::MarkerShape shape); // 设置标记形状

    void updateMarkerShapeAndSize(); // 更新标记形状以及尺寸
    QLegend::MarkerShape effectiveMarkerShape() const; // 有效标记形状
    qreal effectiveMarkerWidth() const; // 有效标记宽度

    ItemType itemType() const { return m_itemType; } // 项目类型

Q_SIGNALS:
    void markerRectChanged();

protected:
    void setItemBrushAndPen(); // 设置项目画刷以及画笔
    void setItemRect(); // 设置项目尺寸
    bool useMaxWidth() const; // 是否使用最大宽度

    QLegendMarkerPrivate *m_marker; // Knows // 源标记
    QRectF m_defaultMarkerRect; //默认标记矩形（父窗口坐标）
    QRectF m_markerRect; // 标记矩形(父窗口坐标)
    QRectF m_boundingRect; // 外接矩形
    QGraphicsTextItem *m_textItem; // 文本项
    QGraphicsItem *m_markerItem; // 标记项
    qreal m_margin; // 留白
    qreal m_space; // 间隔
    QString m_label; // 标签
    QLegend::MarkerShape m_markerShape; // 标记形状

    QBrush m_labelBrush; // 标签画刷
    QPen m_pen; // 画笔
    QBrush m_brush; // 画刷
    QPen m_seriesPen; // 序列画笔
    QBrush m_seriesBrush; // 序列画刷
    QFont m_font; // 字体
    bool m_hovering; // 是否鼠标处于其上

    ItemType m_itemType; // 项目类型

    friend class QLegendMarker;
    friend class QLegendMarkerPrivate;
    friend class LegendLayout;
};

QT_CHARTS_END_NAMESPACE

#endif // LEGENDMARKERITEM_P_H
