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

#include <QtGui/QPainter>
#include <QtWidgets/QGraphicsSceneEvent>
#include <QtWidgets/QGraphicsTextItem>
#include <QtWidgets/QGraphicsEllipseItem>
#include <QtWidgets/QGraphicsRectItem>
#include <QtWidgets/QGraphicsLineItem>
#include <QtGui/QTextDocument>
#include <QtCore/QtMath>

#include <QtCharts/QLegend>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QLineSeries>
#include <QtCharts/QSplineSeries>
#include <private/qlegend_p.h>
#include <QtCharts/QLegendMarker>
#include <private/qlegendmarker_p.h>
#include <private/legendmarkeritem_p.h>
#include <private/chartpresenter_p.h>

QT_CHARTS_BEGIN_NAMESPACE

// 构造函数
LegendMarkerItem::LegendMarkerItem(QLegendMarkerPrivate *marker, QGraphicsObject *parent) :
    QGraphicsObject(parent),
    m_marker(marker), // 源标记私有成员
    m_defaultMarkerRect(0.0, 0.0, 10.0, 10.0), // 默认标记矩形
    m_markerRect(0.0, 0.0, -1.0, -1.0), // 标记矩形
    m_boundingRect(0,0,0,0), // 标记外接矩形
    m_textItem(new QGraphicsTextItem(this)), // 标记文本
    m_markerItem(nullptr), // 标记项
    m_margin(3), // 留白
    m_space(4), // 间隔
    m_markerShape(QLegend::MarkerShapeDefault), // 标记形状
    m_hovering(false), // 捕获鼠标在其上
    m_itemType(TypeRect) // 项类型
{
    m_textItem->document()->setDocumentMargin(ChartPresenter::textMargin()); // 设置文本留白
    setAcceptHoverEvents(true); // 设置接收捕获鼠标在其上事件
}

// 析构函数
LegendMarkerItem::~LegendMarkerItem()
{
    if (m_hovering) { // 鼠标在其上
        emit m_marker->q_ptr->hovered(false); // 发送信号
    }
}

// 设置画笔
void LegendMarkerItem::setPen(const QPen &pen)
{
    m_pen = pen; // 保存画笔
    setItemBrushAndPen(); // 设置画笔
}

// 获取画笔
QPen LegendMarkerItem::pen() const
{
    return m_pen;
}

// 设置画刷
void LegendMarkerItem::setBrush(const QBrush &brush)
{
    m_brush = brush;
    setItemBrushAndPen();
}

// 获取画刷
QBrush LegendMarkerItem::brush() const
{
    return m_brush;
}

// 设置序列画笔
void LegendMarkerItem::setSeriesPen(const QPen &pen)
{
    m_seriesPen = pen;
    setItemBrushAndPen();
}

// 设置序列画刷
void LegendMarkerItem::setSeriesBrush(const QBrush &brush)
{
    m_seriesBrush = brush;
    setItemBrushAndPen();
}

// 设置字体
void LegendMarkerItem::setFont(const QFont &font)
{
    QFontMetrics fn(font); // 创建字体
    m_font = font; // 记录字体

    m_defaultMarkerRect = QRectF(0, 0, fn.height() / 2, fn.height() / 2); // 设置默认标记矩形
    if (effectiveMarkerShape() != QLegend::MarkerShapeFromSeries) // 从序列继承形状
        updateMarkerShapeAndSize(); // 更新标记形状、尺寸
    m_marker->invalidateLegend(); // 更新图例
}

// 获取字体
QFont LegendMarkerItem::font() const
{
    return m_font;
}

// 设置标签
void LegendMarkerItem::setLabel(const QString label)
{
    m_label = label;
    updateGeometry();
}

// 获取标签
QString LegendMarkerItem::label() const
{
    return m_label;
}

// 设置标签画刷
void LegendMarkerItem::setLabelBrush(const QBrush &brush)
{
    m_textItem->setDefaultTextColor(brush.color()); // 设置默认颜色
}

// 获取标签画刷
QBrush LegendMarkerItem::labelBrush() const
{
    return QBrush(m_textItem->defaultTextColor());
}

// 设置几何尺寸，同时设定标记、文本的位置
void LegendMarkerItem::setGeometry(const QRectF &rect)
{
    // 如果标记项不存在，则按形状、尺寸创建
    if (!m_markerItem)
        updateMarkerShapeAndSize(); // 更新形状、尺寸
    // 记录输入宽度
    const qreal width = rect.width();
    // 有效宽度
    const qreal markerWidth = effectiveMarkerWidth();
    // 计算文本的起始x坐标
    const qreal x = m_margin + markerWidth + m_space + m_margin;
    // 计算文本外接矩形，并对其进行剪裁
    QRectF truncatedRect;
    const QString html = ChartPresenter::truncatedText(m_font, m_label, qreal(0.0),
                                                       width - x, rect.height(), truncatedRect);
    // 设置文本
    m_textItem->setHtml(html);
    // 设置tooltip
#if QT_CONFIG(tooltip)
    if (m_marker->m_legend->showToolTips() && html != m_label)
        m_textItem->setToolTip(m_label);
    else
        m_textItem->setToolTip(QString());
#endif
    // 设置字体
    m_textItem->setFont(m_font);
    // 设置文本宽度
    m_textItem->setTextWidth(truncatedRect.width());
    // 计算起始y坐标
    qreal y = qMax(m_markerRect.height() + 2 * m_margin, truncatedRect.height() + 2 * m_margin);
    // 计算文本外接矩形
    const QRectF &textRect = m_textItem->boundingRect();
    // 设置文本位置
    m_textItem->setPos(x - m_margin, y / 2 - textRect.height() / 2);
    // 设置项目矩形
    setItemRect();
    // The textMargin adjustments to position are done to make default case rects less blurry with anti-aliasing
    // 设置标记位置
    m_markerItem->setPos(m_margin - ChartPresenter::textMargin()
                         + (markerWidth - m_markerRect.width()) / 2.0,
                         y / 2.0  - m_markerRect.height() / 2.0 + ChartPresenter::textMargin());
    // 更新几何图形
    prepareGeometryChange(); // 继承自QGraphicsItem
    // 重新计算外接矩形
    m_boundingRect = QRectF(0, 0, x + textRect.width() + m_margin, y);
}

// 获取外接矩形
QRectF LegendMarkerItem::boundingRect() const
{
    return m_boundingRect;
}

// 获取标记矩形
QRectF LegendMarkerItem::markerRect() const
{
    return m_markerRect;
}

// 绘图
void LegendMarkerItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
    Q_UNUSED(painter)
}

// 默认尺寸
QSizeF LegendMarkerItem::sizeHint(Qt::SizeHint which, const QSizeF& constraint) const
{
    Q_UNUSED(constraint)

    QSizeF sh;
    const qreal markerWidth = effectiveMarkerWidth(); // 有效的标记宽度

    switch (which) {
    case Qt::MinimumSize: { // 最小尺寸
        const QRectF labelRect = ChartPresenter::textBoundingRect(m_font, QStringLiteral("..."));
        sh = QSizeF(labelRect.width() + (2.0 * m_margin) + m_space + markerWidth,
                    qMax(m_markerRect.height(), labelRect.height()) + (2.0 * m_margin));
        break;
    }
    case Qt::PreferredSize: { // 合适尺寸
        const QRectF labelRect = ChartPresenter::textBoundingRect(m_font, m_label);
        sh = QSizeF(labelRect.width() + (2.0 * m_margin) + m_space + markerWidth,
                    qMax(m_markerRect.height(), labelRect.height()) + (2.0 * m_margin));
        break;
    }
    default:
        break;
    }

    return sh;
}

// 在其上进入事件
void LegendMarkerItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event)
    m_hovering = true;
    emit m_marker->q_ptr->hovered(true);
}

// 在其上离开事件
void LegendMarkerItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event)
    m_hovering = false;
    emit m_marker->q_ptr->hovered(false);
}

// 获取显示的标签
QString LegendMarkerItem::displayedLabel() const
{
    return m_textItem->toHtml();
}

// 设置提示气泡
void LegendMarkerItem::setToolTip(const QString &tip)
{
#if QT_CONFIG(tooltip)
    m_textItem->setToolTip(tip);
#endif
}

// 获取标记形状
QLegend::MarkerShape LegendMarkerItem::markerShape() const
{
    return m_markerShape;
}

// 设置标记形状
void LegendMarkerItem::setMarkerShape(QLegend::MarkerShape shape)
{
    m_markerShape = shape;
}

// 更新标记形状、尺寸
void LegendMarkerItem::updateMarkerShapeAndSize()
{
    // 获取有效形状
    const QLegend::MarkerShape shape = effectiveMarkerShape();

    // 项类型
    ItemType itemType = TypeRect;

    // 默认标记矩形尺寸
    QRectF newRect = m_defaultMarkerRect;

    // 从序列继承形状
    if (shape == QLegend::MarkerShapeFromSeries) {
        // 散点序列
        QScatterSeries *scatter = qobject_cast<QScatterSeries *>(m_marker->series());
        // 确认散点序列
        if (scatter) {
            // 新标记尺寸
            newRect.setSize(QSizeF(scatter->markerSize(), scatter->markerSize()));
            // 原型标记
            if (scatter->markerShape() == QScatterSeries::MarkerShapeCircle)
                itemType = TypeCircle; // 更新项类型
        }
        // 线或样条
        else if (qobject_cast<QLineSeries *>(m_marker->series())
                   || qobject_cast<QSplineSeries *>(m_marker->series())) {
            newRect.setHeight(m_seriesPen.width()); // 高度
            newRect.setWidth(qRound(m_defaultMarkerRect.width() * 1.5)); // 宽度
            itemType = TypeLine; // 更新项类型
        }
    }
    // 圆形
    else if (shape == QLegend::MarkerShapeCircle) {
        itemType = TypeCircle; // 更新项类型
    }

    // 如果当前标记不存在或者项目类型与预期不符
    if (!m_markerItem || m_itemType != itemType) {
        // 保存项类型
        m_itemType = itemType;
        // 删除当前项，并记录其位置
        QPointF oldPos;
        if (m_markerItem) {
            oldPos = m_markerItem->pos();
            delete m_markerItem;
        }
        // 根据不同类型创建相应项
        if (itemType == TypeRect)
            m_markerItem = new QGraphicsRectItem(this);
        else if (itemType == TypeCircle)
            m_markerItem = new QGraphicsEllipseItem(this);
        else
            m_markerItem =  new QGraphicsLineItem(this);
        // Immediately update the position to the approximate correct position to avoid marker
        // jumping around when changing markers
        // 恢复项的位置
        m_markerItem->setPos(oldPos);
    }
    // 更新画笔、画刷
    setItemBrushAndPen();
    // 新尺寸不等于当前标记尺寸
    if (newRect != m_markerRect) {
        // 尺寸小于新尺寸
        if (useMaxWidth() && m_marker->m_legend->d_ptr->maxMarkerWidth() < newRect.width())
            m_marker->invalidateAllItems(); // 更新全部标记、布局
        // 更新尺寸
        m_markerRect = newRect;
        setItemRect(); // 更新当前标记向尺寸
        emit markerRectChanged(); // 发送信号
        updateGeometry(); // 更新几何
    }
}

// 获取有效的标记形状
QLegend::MarkerShape LegendMarkerItem::effectiveMarkerShape() const
{
    QLegend::MarkerShape shape = m_markerShape; // 获取标志形状
    if (shape == QLegend::MarkerShapeDefault) // 如果为默认标记形状
        shape = m_marker->m_legend->markerShape(); // 返回图例中记录的标记形状
    return shape; // 返回相应形状
}

// 获取有效宽度
qreal LegendMarkerItem::effectiveMarkerWidth() const
{
    return useMaxWidth() ? m_marker->m_legend->d_ptr->maxMarkerWidth()
                         : m_markerRect.width();
}

// 设置画刷以及画笔
void LegendMarkerItem::setItemBrushAndPen()
{
    if (m_markerItem) { // 标记项存在
        // 转化为形状项
        QAbstractGraphicsShapeItem *shapeItem =
                qgraphicsitem_cast<QGraphicsRectItem *>(m_markerItem);
        // 转化失败
        if (!shapeItem)
            shapeItem = qgraphicsitem_cast<QGraphicsEllipseItem *>(m_markerItem); // 转化为椭圆项
        if (shapeItem) { // 转化成功
            if (effectiveMarkerShape() == QLegend::MarkerShapeFromSeries) { // 采用序列的形状
                shapeItem->setPen(m_seriesPen); // 设置序列画笔
                shapeItem->setBrush(m_seriesBrush); // 设置序列画刷
            } else { // 如果不采用序列的形状
                shapeItem->setPen(m_pen); // 设置画笔
                shapeItem->setBrush(m_brush); // 设置画刷
            }
        } else { // 转化失败
            // Must be line item, it has no brush.
            QGraphicsLineItem *lineItem =
                    qgraphicsitem_cast<QGraphicsLineItem *>(m_markerItem); // 转化为线
            if (lineItem) // 转化成功
                lineItem->setPen(m_seriesPen); // 设定画笔
        }
    }
}

// 设置标记项尺寸
void LegendMarkerItem::setItemRect()
{
    if (m_itemType == TypeRect) { // 矩形标记
        static_cast<QGraphicsRectItem *>(m_markerItem)->setRect(m_markerRect);
    } else if (m_itemType == TypeCircle) { // 圆标记
        static_cast<QGraphicsEllipseItem *>(m_markerItem)->setRect(m_markerRect);
    } else { // 其他标记（线标记）
        qreal y = m_markerRect.height() / 2.0;
        QLineF line(0.0, y, m_markerRect.width(), y);
        static_cast<QGraphicsLineItem *>(m_markerItem)->setLine(line);
    }
}

// 是否使用最大宽度
bool LegendMarkerItem::useMaxWidth() const
{
    return (m_marker->m_legend->alignment() == Qt::AlignLeft
            || m_marker->m_legend->alignment() == Qt::AlignRight);
}

#include "moc_legendmarkeritem_p.cpp"

QT_CHARTS_END_NAMESPACE
