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

#include <QtCharts/QLegend>
#include <private/qlegend_p.h>
#include <QtCharts/QAbstractSeries>
#include <private/qabstractseries_p.h>
#include <private/qchart_p.h>
#include <private/legendlayout_p.h>
#include <private/chartpresenter_p.h>
#include <private/abstractchartlayout_p.h>
#include <QtCharts/QLegendMarker>
#include <private/qlegendmarker_p.h>
#include <private/legendmarkeritem_p.h>
#include <private/chartdataset_p.h>
#include <QtGui/QPainter>
#include <QtGui/QPen>
#include <QtWidgets/QGraphicsItemGroup>

QT_CHARTS_BEGIN_NAMESPACE

/*!
    \class QLegend
    \inmodule Qt Charts
    \inherits QGraphicsWidget
    \brief The QLegend class displays the legend of a chart.

    A legend is a graphical object that displays the legend of a chart. The legend state is updated
    by QChart when series change. By default, the legend is attached to the chart, but it can be
    detached to make it independent of chart layout. Legend objects cannot be created or deleted,
    but they can be referenced via the QChart class.

    \image examples_percentbarchart_legend.png

    \sa QChart
*/
/*!
    \qmltype Legend
    \instantiates QLegend
    \inqmlmodule QtCharts

    \brief Displays the legend of a chart.

    A legend is a graphical object that displays the legend of a chart. The legend state is updated
    by the ChartView type when series change. The \l Legend type properties can be attached to the
    ChartView type. For example:
    \code
        ChartView {
            legend.visible: true
            legend.alignment: Qt.AlignBottom
            // Add a few series...
        }
    \endcode

    \image examples_percentbarchart_legend.png

    \note There is no QML API available for modifying legend markers. Markers can be modified by
    creating a custom legend, as illustrated by \l {qmlcustomlegend}{Qml Custom Example}.
*/

/*!
    \property QLegend::alignment
    \brief How the legend is aligned with the chart.

    Can be Qt::AlignTop, Qt::AlignBottom, Qt::AlignLeft, Qt::AlignRight. If you set more than one
    flag, the result is undefined.
*/
/*!
    \qmlproperty alignment Legend::alignment
    Defines how the legend is aligned with the chart. Can be \l{Qt::AlignLeft}{Qt.AlignLeft},
    \l{Qt::AlignRight}{Qt.AlignRight}, \l{Qt::AlignBottom}{Qt.AlignBottom}, or
    \l{Qt::AlignTop}{Qt.AlignTop}. If you set more than one flag, the result is undefined.
*/

/*!
    \property QLegend::backgroundVisible
    \brief Whether the legend background is visible.
*/
/*!
    \qmlproperty bool Legend::backgroundVisible
    Whether the legend background is visible.
*/

/*!
    \qmlproperty bool Legend::visible
    Whether the legend is visible.

    By default, this property is \c true.
    \sa QGraphicsObject::visible
*/

/*!
    \property QLegend::color
    \brief The background (brush) color of the legend.

    If you change the color of the legend, the style of the legend brush is set to
    Qt::SolidPattern.
*/
/*!
    \qmlproperty color Legend::color
    The background (brush) color of the legend.
*/

/*!
    \property QLegend::borderColor
    \brief The line color of the legend.
*/
/*!
    \qmlproperty color Legend::borderColor
    The line color of the legend.
*/

/*!
    \property QLegend::font
    \brief The font of the markers used by the legend.
*/
/*!
    \qmlproperty Font Legend::font
    The font of the markers used by the legend.
*/

/*!
    \property QLegend::labelColor
    \brief The color of the brush used to draw labels.
*/
/*!
    \qmlproperty color Legend::labelColor
    The color of the brush used to draw labels.
*/

/*!
    \property QLegend::reverseMarkers
    \brief Whether reverse order is used for the markers in the legend.

    This property is \c false by default.
*/
/*!
    \qmlproperty bool Legend::reverseMarkers
    Whether reverse order is used for the markers in the legend. This property
    is \c false by default.
*/

/*!
    \property QLegend::showToolTips
    \brief Whether tooltips are shown when the text is truncated.

    This property is \c false by default.
*/

/*!
    \enum QLegend::MarkerShape

    This enum describes the shape used when rendering legend marker items.

    \value MarkerShapeDefault Default shape determined by QLegend is used for the marker.
           This value is supported only for individual QLegendMarker items.
    \value MarkerShapeRectangle Rectangular markers are used.
           Marker size is determined by font size.
    \value MarkerShapeCircle Circular markers are used.
           Marker size is determined by font size.
    \value MarkerShapeFromSeries The marker shape is determined by the series.
           In case of a scatter series, the legend marker looks like a scatter dot and is the same
           size as the dot. In case of a line or spline series, the legend marker looks like a small
           segment of the line. For other series types, rectangular markers are shown.

    \sa markerShape
*/

/*!
    \qmlproperty enumeration Legend::markerShape
    \since 5.9

    The default shape of the legend markers.
    The default value is \c{MarkerShapeRectangle}.

    \value Legend.MarkerShapeRectangle Legend markers are rectangular
    \value Legend.MarkerShapeCircle Legend markers are circular
    \value Legend.MarkerShapeFromSeries Legend marker shape is determined by the series

    \sa QLegend::MarkerShape
*/

/*!
    \property QLegend::markerShape
    \since 5.9

    The default shape of the legend markers.
    The default value is \c{MarkerShapeRectangle}.
*/

/*!
    \qmlproperty bool Legend::showToolTips
    Whether tooltips are shown when the text is truncated. This property is \c false by default.
    This property currently has no effect as there is no support for tooltips in QML.
*/

/*!
    \fn void QLegend::backgroundVisibleChanged(bool)
    This signal is emitted when the visibility of the legend background changes to \a visible.
*/

/*!
    \fn void QLegend::colorChanged(QColor)
    This signal is emitted when the color of the legend background changes to \a color.
*/

/*!
    \fn void QLegend::borderColorChanged(QColor)
    This signal is emitted when the border color of the legend background changes to \a color.
*/

/*!
    \fn void QLegend::fontChanged(QFont)
    This signal is emitted when the font of the markers of the legend changes to \a font.
*/

/*!
    \fn void QLegend::labelColorChanged(QColor color)
    This signal is emitted when the color of the brush used to draw the legend
    labels changes to \a color.
*/

/*!
    \fn void QLegend::reverseMarkersChanged(bool)
    This signal is emitted when the use of reverse order for the markers in the
    legend is changed to \a reverseMarkers.
*/

/*!
    \fn void QLegend::showToolTipsChanged(bool showToolTips)
    This signal is emitted when the visibility of tooltips is changed to \a showToolTips.
*/
// 构造函数
QLegend::QLegend(QChart *chart): QGraphicsWidget(chart),
    d_ptr(new QLegendPrivate(chart->d_ptr->m_presenter, chart, this))
{
    setZValue(ChartPresenter::LegendZValue); // 设置Z轴坐标
    setFlags(QGraphicsItem::ItemClipsChildrenToShape); // 命令所有的后代绘制到自己的形状里
    // 序列添加、删除信号、槽
    QObject::connect(chart->d_ptr->m_dataset, SIGNAL(seriesAdded(QAbstractSeries*)), d_ptr.data(), SLOT(handleSeriesAdded(QAbstractSeries*)));
    QObject::connect(chart->d_ptr->m_dataset, SIGNAL(seriesRemoved(QAbstractSeries*)), d_ptr.data(), SLOT(handleSeriesRemoved(QAbstractSeries*)));
    // 设置布局
    setLayout(d_ptr->m_layout);
}

/*!
    Destroys the legend object. The legend is always owned by a QChart, so an application
    should never call this function.
*/
// 析构函数
QLegend::~QLegend()
{
}

/*!
 \internal
 */
// 绘制
void QLegend::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    // 背景不可见
    if (!d_ptr->m_backgroundVisible)
        return;

    // 绘制背景
    painter->setOpacity(opacity()); // 设置不透明
    painter->setPen(d_ptr->m_pen); // 设置画笔
    painter->setBrush(d_ptr->m_brush); // 设置画刷
    painter->drawRoundRect(rect(), d_ptr->roundness(rect().width()), d_ptr->roundness(rect().height())); // 绘制背景
}


/*!
    Sets the \a brush that is used to draw the background of the legend.
 */
// 设置画刷
void QLegend::setBrush(const QBrush &brush)
{
    if (d_ptr->m_brush != brush) { // 画刷不一致
        d_ptr->m_brush = brush; // 更新画刷
        update(); // 更新
        emit colorChanged(brush.color()); // 发送信号
    }
}

/*!
    Returns the brush used by the legend.
 */
// 获取当前画刷
QBrush QLegend::brush() const
{
    if (d_ptr->m_brush == QChartPrivate::defaultBrush()) // 默认画刷
        return QBrush();
    else
        return d_ptr->m_brush; // 当前画刷
}

// 设置颜色
void QLegend::setColor(QColor color)
{
    QBrush b = brush();
    if (b.style() != Qt::SolidPattern || b.color() != color) {
        b.setStyle(Qt::SolidPattern);
        b.setColor(color);
        setBrush(b);
    }
}

// 获取颜色
QColor QLegend::color()
{
    return d_ptr->m_brush.color();
}

/*!
    Sets the \a pen that is used to draw the legend borders.
 */
// 设置画笔
void QLegend::setPen(const QPen &pen)
{
    if (d_ptr->m_pen != pen) {
        d_ptr->m_pen = pen;
        update();
        emit borderColorChanged(pen.color());
    }
}

/*!
    Returns the pen used by the legend.
 */

// 获取画笔
QPen QLegend::pen() const
{
    if (d_ptr->m_pen == QChartPrivate::defaultPen())
        return QPen();
    else
        return d_ptr->m_pen;
}

// 设置字体
void QLegend::setFont(const QFont &font)
{
    if (d_ptr->m_font != font) { // 字体不同
        // Hide items to avoid flickering
        d_ptr->items()->setVisible(false); // 设置是否可见
        d_ptr->m_font = font; // 设置字体
        foreach (QLegendMarker *marker, d_ptr->markers()) {
            marker->setFont(d_ptr->m_font); // 设置标记字体
        }
        layout()->invalidate(); // 布局更新
        emit fontChanged(font); // 发送信号
    }
}

// 获取字体
QFont QLegend::font() const
{
    return d_ptr->m_font;
}

// 设置边框颜色
void QLegend::setBorderColor(QColor color)
{
    QPen p = pen();
    if (p.color() != color) {
        p.setColor(color);
        setPen(p);
    }
}

// 获取边框颜色
QColor QLegend::borderColor()
{
    return d_ptr->m_pen.color();
}

/*!
    Sets the brush used to draw the legend labels to \a brush.
*/
// 设置标签画刷
void QLegend::setLabelBrush(const QBrush &brush)
{
    if (d_ptr->m_labelBrush != brush) {
        d_ptr->m_labelBrush = brush;
        foreach (QLegendMarker *marker, d_ptr->markers()) {
            marker->setLabelBrush(d_ptr->m_labelBrush);
            // Note: The pen of the marker rectangle could be exposed in the public QLegend API
            // instead of mapping it from label brush color
            marker->setPen(brush.color());
        }
        emit labelColorChanged(brush.color());
    }
}

/*!
    Returns the brush used to draw labels.
*/
// 获取标签画刷
QBrush QLegend::labelBrush() const
{
    if (d_ptr->m_labelBrush == QChartPrivate::defaultBrush())
        return QBrush();
    else
        return d_ptr->m_labelBrush;
}

// 设置标签颜色
void QLegend::setLabelColor(QColor color)
{
    QBrush b = labelBrush();
    if (b.style() != Qt::SolidPattern || b.color() != color) {
        b.setStyle(Qt::SolidPattern);
        b.setColor(color);
        setLabelBrush(b);
    }
}

// 获取标签颜色
QColor QLegend::labelColor() const
{
    return d_ptr->m_labelBrush.color();
}

// 设置停靠方式
void QLegend::setAlignment(Qt::Alignment alignment)
{
    if (d_ptr->m_alignment != alignment) {
        d_ptr->m_alignment = alignment;
        layout()->invalidate();
    }
}

// 获取停靠方式
Qt::Alignment QLegend::alignment() const
{
    return d_ptr->m_alignment;
}

/*!
    Detaches the legend from the chart. The chart will no longer adjust the layout of the legend.
 */
// 松绑表格
void QLegend::detachFromChart()
{
    d_ptr->m_attachedToChart = false;
//    layout()->invalidate();
    d_ptr->m_chart->layout()->invalidate();
    setParent(0);

}

/*!
    Attaches the legend to a chart. The chart may adjust the layout of the legend.
 */
// 绑定表格
void QLegend::attachToChart()
{
    d_ptr->m_attachedToChart = true; // 标记
//    layout()->invalidate();
    d_ptr->m_chart->layout()->invalidate(); // 更新
    setParent(d_ptr->m_chart);
}

/*!
    Returns \c true, if the legend is attached to a chart.
 */
// 是否捆绑表格
bool QLegend::isAttachedToChart()
{
    return d_ptr->m_attachedToChart;
}

/*!
    Sets the visibility of the legend background to \a visible.
 */
// 设置背景是否可见
void QLegend::setBackgroundVisible(bool visible)
{
    if (d_ptr->m_backgroundVisible != visible) {
        d_ptr->m_backgroundVisible = visible;
        update();
        emit backgroundVisibleChanged(visible);
    }
}

/*!
    Returns the visibility of the legend background.
 */
// 获取背景是否可见
bool QLegend::isBackgroundVisible() const
{
    return d_ptr->m_backgroundVisible;
}

/*!
    Returns the list of markers in the legend. The list can be filtered by specifying
    the \a series for which the markers are returned.
*/
// 获取与指定序列相关的标记列表
QList<QLegendMarker*> QLegend::markers(QAbstractSeries *series) const
{
    return d_ptr->markers(series);
}

// 是否为逆向标记
bool QLegend::reverseMarkers()
{
    return d_ptr->m_reverseMarkers;
}

// 设置逆向标记
void QLegend::setReverseMarkers(bool reverseMarkers)
{
    if (d_ptr->m_reverseMarkers != reverseMarkers) {
        d_ptr->m_reverseMarkers = reverseMarkers;
        layout()->invalidate();
        emit reverseMarkersChanged(reverseMarkers);
    }
}

/*!
    Returns whether the tooltips are shown for the legend labels
    when they are elided.
*/
// 是否显示提示气泡
bool QLegend::showToolTips() const
{
    return d_ptr->m_showToolTips;
}

/*!
    When \a show is \c true, the legend labels will show a tooltip when
    the mouse hovers over them if the label itself is shown elided.
    This is \c false by default.
*/
// 设置显示提示气泡
void QLegend::setShowToolTips(bool show)
{
    if (d_ptr->m_showToolTips != show) {
        d_ptr->m_showToolTips = show;
        d_ptr->updateToolTips();
        emit showToolTipsChanged(show);
    }
}

// 标记形状
QLegend::MarkerShape QLegend::markerShape() const
{
    return d_ptr->m_markerShape;
}

// 设置标记形状
void QLegend::setMarkerShape(QLegend::MarkerShape shape)
{
    QLegend::MarkerShape newShape = shape;
    if (newShape == MarkerShapeDefault)
        newShape = MarkerShapeRectangle;
    if (d_ptr->m_markerShape != newShape) {
        d_ptr->m_markerShape = newShape;
        layout()->invalidate();
        emit markerShapeChanged(newShape);
    }
}

/*!
    \internal
    \a event, see QGraphicsWidget for details.
 */
// 隐藏事件
void QLegend::hideEvent(QHideEvent *event)
{
    if (isAttachedToChart())
        d_ptr->m_presenter->layout()->invalidate();
    QGraphicsWidget::hideEvent(event);
}
/*!
    \internal
    \a event, see QGraphicsWidget for details.
 */
// 显示事件
void QLegend::showEvent(QShowEvent *event)
{
    if (isAttachedToChart())
        layout()->invalidate();
    QGraphicsWidget::showEvent(event);
    //layout activation will show the items
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 构造函数
QLegendPrivate::QLegendPrivate(ChartPresenter *presenter, QChart *chart, QLegend *q)
    : q_ptr(q), // 源图例
      m_presenter(presenter), // 图表主持
      m_layout(new LegendLayout(q)), // 布局
      m_chart(chart), // 图表
      m_items(new QGraphicsItemGroup(q)), // 项目组
      m_alignment(Qt::AlignTop), // 停靠方式
      m_brush(QChartPrivate::defaultBrush()), // 画刷
      m_pen(QChartPrivate::defaultPen()), // 画笔
      m_labelBrush(QChartPrivate::defaultBrush()), // 标签画刷
      m_diameter(5), // 直径
      m_attachedToChart(true), // 绑定到图表
      m_backgroundVisible(false), // 背景可视
      m_reverseMarkers(false), // 逆向标记
      m_showToolTips(false), // 显示提示
      m_markerShape(QLegend::MarkerShapeRectangle) // 标记形状
{
    m_items->setHandlesChildEvents(false); // 禁止响应子项事件
}

// 析构
QLegendPrivate::~QLegendPrivate()
{
}

// 设置偏移
void QLegendPrivate::setOffset(const QPointF &offset)
{
    m_layout->setOffset(offset.x(), offset.y());
}

// 获取偏移
QPointF QLegendPrivate::offset() const
{
    return m_layout->offset();
}

// 圆角角度
int QLegendPrivate::roundness(qreal size)
{
    return 100 * m_diameter / int(size);
}

// 获取指定序列相关标记
QList<QLegendMarker*> QLegendPrivate::markers(QAbstractSeries *series)
{
    // Return all markers
    // 如果序列为空，返回全部标记
    if (!series) {
        return m_markers;
    }

    // Create filtered list
    // 遍历标记
    QList<QLegendMarker *> markers;
    foreach (QLegendMarker *marker, m_markers) {
        if (marker->series() == series) { // 序列相同
            markers.append(marker); // 记录标记
        }
    }

    // 返回标记链表
    return markers;
}

// 最大标记宽度
qreal QLegendPrivate::maxMarkerWidth() const
{
    // 遍历标记
    qreal maxWidth = 0.0;
    for (int i = 0; i < m_markers.size(); i++) {
        // 记录标记项
        LegendMarkerItem *item = m_markers.at(i)->d_ptr->item();
        // 计算最大标记
        if (item)
            maxWidth = qMax(item->markerRect().width(), maxWidth);
    }
    // 返回最大标记宽度
    return maxWidth;
}

// 增加序列信号消息相应槽
void QLegendPrivate::handleSeriesAdded(QAbstractSeries *series)
{
    // 序列已经存在
    if (m_series.contains(series)) {
        return;
    }

    // 创建标记
    QList<QLegendMarker*> newMarkers = series->d_ptr->createLegendMarkers(q_ptr);
    decorateMarkers(newMarkers); // 布置标记
    addMarkers(newMarkers); // 增加标记

    // 捆绑信号、槽
    QObject::connect(series->d_ptr.data(), SIGNAL(countChanged()), this, SLOT(handleCountChanged()));
    QObject::connect(series, SIGNAL(visibleChanged()), this, SLOT(handleSeriesVisibleChanged()));

    // 增加序列
    m_series.append(series);

    // 设置不可见
    m_items->setVisible(false);

    // 更新布局
    m_layout->invalidate();
}

// 需里移除信号相应槽
void QLegendPrivate::handleSeriesRemoved(QAbstractSeries *series)
{
    // 如果序列以存在
    if (m_series.contains(series)) {
        m_series.removeOne(series); // 删除指定序列
    }

    // Find out, which markers to remove
    // 删除相关标记
    QList<QLegendMarker *> removed;
    foreach (QLegendMarker *m, m_markers) {
        if (m->series() == series) {
            removed << m;
        }
    }
    removeMarkers(removed);

    // 断开信号、槽
    QObject::disconnect(series->d_ptr.data(), SIGNAL(countChanged()), this, SLOT(handleCountChanged()));
    QObject::disconnect(series, SIGNAL(visibleChanged()), this, SLOT(handleSeriesVisibleChanged()));

    // 更新
    m_layout->invalidate();
}

// 序列可视变化信号响应槽
void QLegendPrivate::handleSeriesVisibleChanged()
{
    // 获取发送者
    QAbstractSeries *series = qobject_cast<QAbstractSeries *> (sender());
    Q_ASSERT(series);
    // 遍历标记，修改相应标记可视状态
    foreach (QLegendMarker *marker, m_markers) {
        if (marker->series() == series) {
            marker->setVisible(series->isVisible());
        }
    }
    // 如果图表可视
    if (m_chart->isVisible())
        m_layout->invalidate(); // 布局更新
}

// 计数变更
void QLegendPrivate::handleCountChanged()
{
    // Here we handle the changes in marker count.
    // Can happen for example when pieslice(s) have been added to or removed from pieseries.
    // 获取序列
    QAbstractSeriesPrivate *series = qobject_cast<QAbstractSeriesPrivate *> (sender());
    // 创建链表
    QList<QLegendMarker *> createdMarkers = series->createLegendMarkers(q_ptr);

    // Find out removed markers and created markers
    // 遍历标记
    QList<QLegendMarker *> removedMarkers;
    foreach (QLegendMarker *oldMarker, m_markers) {
        // we have marker, which is related to sender.
        // 找到指定序列
        if (oldMarker->series() == series->q_ptr) {
            // 发现标志
            bool found = false;
            // 遍历创建标记
            foreach(QLegendMarker *newMarker, createdMarkers) {
                // New marker considered existing if:
                // - d_ptr->relatedObject() is same for both markers.
                if (newMarker->d_ptr->relatedObject() == oldMarker->d_ptr->relatedObject()) { // 如果相关对象相同
                    // Delete the new marker, since we already have existing marker, that might be connected on user side.
                    found = true;
                    createdMarkers.removeOne(newMarker);
                    delete newMarker;
                }
            }
            // 尚未发现
            if (!found) {
                // No related object found for marker, add to removedMarkers list
                removedMarkers << oldMarker;
            }
        }
    }

    // 删除指定标记
    removeMarkers(removedMarkers);

    // 布置标记
    decorateMarkers(createdMarkers);

    // 添加标记
    addMarkers(createdMarkers);

    // 布局更新
    q_ptr->layout()->invalidate();
}

// 增加标记
void QLegendPrivate::addMarkers(QList<QLegendMarker *> markers)
{
    foreach (QLegendMarker *marker, markers) { // 遍历链表中标记
        m_items->addToGroup(marker->d_ptr.data()->item()); // 添加进组
        m_markers << marker; // 添加进链表
        m_markerHash.insert(marker->d_ptr->item(), marker); // 添加进Hash
    }
}

// 删除标记
void QLegendPrivate::removeMarkers(QList<QLegendMarker *> markers)
{
    foreach (QLegendMarker *marker, markers) { // 遍历输入链表
        marker->d_ptr->item()->setVisible(false); // 取消可见
        m_items->removeFromGroup(marker->d_ptr->item()); // 从组中移除
        m_markers.removeOne(marker); // 删除标记
        m_markerHash.remove(marker->d_ptr->item()); // 删除标记
        delete marker; // 释放标记
    }
}

// 布置标记
void QLegendPrivate::decorateMarkers(QList<QLegendMarker *> markers)
{
    foreach (QLegendMarker *marker, markers) { // 遍历标记
        marker->setFont(m_font); // 设置字体
        marker->setLabelBrush(m_labelBrush); // 设置标签画刷
    }
}

// 更新提示气泡
void QLegendPrivate::updateToolTips()
{
    foreach (QLegendMarker *m, m_markers) { // 遍历标记
        if (m->d_ptr->m_item->displayedLabel() != m->label())
            m->d_ptr->m_item->setToolTip(m->label()); // 设置提示气泡
        else
            m->d_ptr->m_item->setToolTip(QString()); // 设置提示气泡
    }
}
#include "moc_qlegend.cpp"
#include "moc_qlegend_p.cpp"

QT_CHARTS_END_NAMESPACE
