/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
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

#include <QtCharts/QXYSeries>
#include <private/qxyseries_p.h>
#include <private/abstractdomain_p.h>
#include <QtCharts/QValueAxis>
#include <private/xychart_p.h>
#include <QtCharts/QXYLegendMarker>
#include <private/charthelpers_p.h>
#include <private/qchart_p.h>
#include <QtGui/QPainter>

QT_CHARTS_BEGIN_NAMESPACE

/*!
    \class QXYSeries
    \inmodule Qt Charts
    \brief The QXYSeries class is a base class for line, spline, and scatter
    series.
*/
/*!
    \qmltype XYSeries
    \instantiates QXYSeries
    \inqmlmodule QtCharts

    \inherits AbstractSeries

    \brief A base type for line, spline, and scatter series.
*/

/*!
    \qmlproperty AbstractAxis XYSeries::axisX
    The x-axis used for the series. If you leave both axisX and axisXTop
    undefined, a value axis is created for the series.
    \sa axisXTop, ValueAxis
*/

/*!
    \qmlproperty AbstractAxis XYSeries::axisY
    The y-axis used for the series. If you leave both axisY and axisYRight
    undefined, a value axis is created for the series.
    \sa axisYRight, ValueAxis
*/

/*!
    \qmlproperty AbstractAxis XYSeries::axisXTop
    The x-axis used for the series, drawn on top of the chart view.

    \note You can only provide either axisX or axisXTop, not both.
    \sa axisX
*/

/*!
    \qmlproperty AbstractAxis XYSeries::axisYRight
    The y-axis used for the series, drawn to the right on the chart view.

    \note You can only provide either axisY or axisYRight, not both.
    \sa axisY
*/

/*!
    \qmlproperty AbstractAxis XYSeries::axisAngular
    The angular axis used for the series, drawn around the polar chart view.
    \sa axisX
*/

/*!
    \qmlproperty AbstractAxis XYSeries::axisRadial
    The radial axis used for the series, drawn inside the polar chart view.
    \sa axisY
*/

/*!
    \property QXYSeries::pointsVisible
    \brief Whether the data points are visible and should be drawn.
*/
/*!
    \qmlproperty bool XYSeries::pointsVisible
    Whether the data points are visible and should be drawn.
*/

/*!
   \fn QPen QXYSeries::pen() const
   Returns the pen used to draw the outline of the data points for the series.
    \sa setPen()
*/

/*!
   \fn QBrush QXYSeries::brush() const
   Returns the brush used to fill the data points for the series.
    \sa setBrush()
*/

/*!
    \property QXYSeries::color
    \brief The color of the series.

    This is the line (pen) color in case of QLineSeries or QSplineSeries and the
    fill (brush) color in case of QScatterSeries or QAreaSeries.
    \sa pen(), brush()
*/
/*!
    \qmlproperty color XYSeries::color
    The color of the series. This is the line (pen) color in case of LineSeries
    or SplineSeries and the fill (brush) color in case of ScatterSeries or
    AreaSeries.
*/

/*!
    \property QXYSeries::pointLabelsFormat
    \brief The format used for showing labels with data points.

    QXYSeries supports the following format tags:
    \table
      \row
        \li @xPoint      \li The x-coordinate of the data point.
      \row
        \li @yPoint      \li The y-coordinate of the data point.
    \endtable

    For example, the following usage of the format tags would produce labels
    that display the data point shown inside brackets separated by a comma
    (x, y):

    \code
    series->setPointLabelsFormat("(@xPoint, @yPoint)");
    \endcode

    By default, the labels' format is set to \c {@xPoint, @yPoint}. The labels
    are shown on the plot area, and the labels on the edge of the plot area are
    cut. If the points are close to each other, the labels may overlap.

    \sa pointLabelsVisible, pointLabelsFont, pointLabelsColor
*/
/*!
    \qmlproperty string XYSeries::pointLabelsFormat
    The format used for showing labels with data points.

    \sa pointLabelsVisible, pointLabelsFont, pointLabelsColor
*/
/*!
    \fn void QXYSeries::pointLabelsFormatChanged(const QString &format)
    This signal is emitted when the format of data point labels changes to
    \a format.
*/

/*!
    \property QXYSeries::pointLabelsVisible
    \brief The visibility of data point labels.

    This property is \c false by default.

    \sa pointLabelsFormat, pointLabelsClipping
*/
/*!
    \qmlproperty bool XYSeries::pointLabelsVisible
    The visibility of data point labels. This property is \c false by default.

    \sa pointLabelsFormat, pointLabelsClipping
*/
/*!
    \fn void QXYSeries::pointLabelsVisibilityChanged(bool visible)
    This signal is emitted when the visibility of the data point labels
    changes to \a visible.
*/

/*!
    \property QXYSeries::pointLabelsFont
    \brief The font used for data point labels.

    \sa pointLabelsFormat
*/
/*!
    \qmlproperty font XYSeries::pointLabelsFont
    The font used for data point labels.

    \sa pointLabelsFormat
*/
/*!
    \fn void QXYSeries::pointLabelsFontChanged(const QFont &font);
    This signal is emitted when the font used for data point labels changes to
    \a font.
*/

/*!
    \property QXYSeries::pointLabelsColor
    \brief The color used for data point labels. By default, the color is the color of the brush
    defined in theme for labels.

    \sa pointLabelsFormat
*/
/*!
    \qmlproperty font XYSeries::pointLabelsColor
    The color used for data point labels. By default, the color is the color of the brush
    defined in theme for labels.

    \sa pointLabelsFormat
*/
/*!
    \fn void QXYSeries::pointLabelsColorChanged(const QColor &color);
    This signal is emitted when the color used for data point labels changes to
    \a color.
*/

/*!
    \property QXYSeries::pointLabelsClipping
    \brief The clipping for data point labels.

    This property is \c true by default. The labels on the edge of the plot area
    are cut when clipping is enabled.

    \sa pointLabelsVisible
*/
/*!
    \qmlproperty bool XYSeries::pointLabelsClipping
    The clipping for data point labels. This property is \c true by default. The
    labels on the edge of the plot area are cut when clipping is enabled.

    \sa pointLabelsVisible
*/
/*!
    \fn void QXYSeries::pointLabelsClippingChanged(bool clipping)
    This signal is emitted when the clipping of the data point labels changes to
    \a clipping.
*/

/*!
    \fn void QXYSeries::clicked(const QPointF& point)
    This signal is emitted when the user triggers a mouse event by
    clicking the point \a point in the chart.

    \sa pressed(), released(), doubleClicked()
*/
/*!
    \qmlsignal XYSeries::clicked(point point)
    This signal is emitted when the user triggers a mouse event by clicking the
    point \a point in the chart. For example:
    \code
    LineSeries {
        XYPoint { x: 0; y: 0 }
        XYPoint { x: 1.1; y: 2.1 }
        onClicked: console.log("onClicked: " + point.x + ", " + point.y);
    }
    \endcode

    The corresponding signal handler is \c onClicked().

    \sa pressed(), released(), doubleClicked()
*/

/*!
    \fn void QXYSeries::hovered(const QPointF &point, bool state)
    This signal is emitted when a mouse is hovered over the point \a point in
    the chart. When the mouse moves over the point, \a state turns \c true,
    and when the mouse moves away again, it turns \c false.
*/
/*!
    \qmlsignal XYSeries::hovered(point point, bool state)
    This signal is emitted when a mouse is hovered over the point \a point in
    the chart. When the mouse moves over the point, \a state turns \c true,
    and when the mouse moves away again, it turns \c false.

    The corresponding signal handler is \c onHovered().
*/

/*!
    \fn void QXYSeries::pressed(const QPointF& point)
    This signal is emitted when the user presses the data point \a point in the
    chart and holds down the mouse button.

    \sa clicked(), released(), doubleClicked()
*/
/*!
    \qmlsignal XYSeries::pressed(point point)
    This signal is emitted when the user presses the data point \a point in the
    chart and holds down the mouse button. For example:
    \code
    LineSeries {
        XYPoint { x: 0; y: 0 }
        XYPoint { x: 1.1; y: 2.1 }
        onPressed: console.log("onPressed: " + point.x + ", " + point.y);
    }
    \endcode

    The corresponding signal handler is \c onPressed().

    \sa clicked(), released(), doubleClicked()
*/

/*!
    \fn void QXYSeries::released(const QPointF& point)
    This signal is emitted when the user releases the mouse press on the data
    point specified by \a point.
    \sa pressed(), clicked(), doubleClicked()
*/
/*!
    \qmlsignal XYSeries::released(point point)
    This signal is emitted when the user releases the mouse press on the data
    point specified by \a point.
    For example:
    \code
    LineSeries {
        XYPoint { x: 0; y: 0 }
        XYPoint { x: 1.1; y: 2.1 }
        onReleased: console.log("onReleased: " + point.x + ", " + point.y);
    }
    \endcode

    The corresponding signal handler is \c onReleased().

    \sa pressed(), clicked(), doubleClicked()
*/

/*!
    \fn void QXYSeries::doubleClicked(const QPointF& point)
    This signal is emitted when the user double-clicks the data point \a point
    in the chart. The \a point is the point where the first press was triggered.
    \sa pressed(), released(), clicked()
*/
/*!
    \qmlsignal XYSeries::doubleClicked(point point)
    This signal is emitted when the user double-clicks the data point \a point
    in the chart. The \a point is the point where the first press was triggered.
    For example:
    \code
    LineSeries {
        XYPoint { x: 0; y: 0 }
        XYPoint { x: 1.1; y: 2.1 }
        onDoubleClicked: console.log("onDoubleClicked: " + point.x + ", " + point.y);
    }
    \endcode

    The corresponding signal handler is \c onDoubleClicked().

    \sa pressed(), released(), clicked()
*/

/*!
    \fn void QXYSeries::pointReplaced(int index)
    This signal is emitted when a point is replaced at the position specified by
    \a index.
    \sa replace()
*/
/*!
    \qmlsignal XYSeries::pointReplaced(int index)
    This signal is emitted when a point is replaced at the position specified by
    \a index.

    The corresponding signal handler is \c onPointReplaced().
*/

/*!
    \fn void QXYSeries::pointsReplaced()
    This signal is emitted when all points are replaced with other points.
    \sa replace()
*/
/*!
    \qmlsignal XYSeries::pointsReplaced()
    This signal is emitted when all points are replaced with other points.

    The corresponding signal handler is \c onPointsReplaced().
*/

/*!
    \fn void QXYSeries::pointAdded(int index)
    This signal is emitted when a point is added at the position specified by
    \a index.
    \sa append(), insert()
*/
/*!
    \qmlsignal XYSeries::pointAdded(int index)
    This signal is emitted when a point is added at the position specified by
    \a index.

    The corresponding signal handler is \c onPointAdded().
*/

/*!
    \fn void QXYSeries::pointRemoved(int index)
    This signal is emitted when a point is removed from the position specified
    by \a index.
    \sa remove()
*/

/*!
    \qmlsignal XYSeries::pointRemoved(int index)
    This signal is emitted when a point is removed from the position specified
    by \a index.

    The corresponding signal handler is \c onPointRemoved().
*/

/*!
    \fn void QXYSeries::pointsRemoved(int index, int count)
    This signal is emitted when the number of points specified by \a count
    is removed starting at the position specified by \a index.
    \sa removePoints(), clear()
*/

/*!
    \qmlsignal XYSeries::pointsRemoved(int index, int count)
    This signal is emitted when the number of points specified by \a count
    is removed starting at the position specified by \a index.

    The corresponding signal handler is \c onPointRemoved().
*/

/*!
    \fn void QXYSeries::colorChanged(QColor color)
    This signal is emitted when the line (pen) color changes to \a color.
*/

/*!
    \fn void QXYSeries::penChanged(const QPen &pen)
    This signal is emitted when the pen changes to \a pen.
*/

/*!
    \fn void QXYSeriesPrivate::updated()
    \internal
*/

/*!
    \qmlmethod XYSeries::append(real x, real y)
    Appends a point with the coordinates \a x and \a y to the series.
*/

/*!
    \qmlmethod XYSeries::replace(real oldX, real oldY, real newX, real newY)
    Replaces the point with the coordinates \a oldX and \a oldY with the point
    with the coordinates \a newX and \a newY. Does nothing if the old point does
    not exist.
*/

/*!
    \qmlmethod XYSeries::remove(real x, real y)
    Removes the point with the coordinates \a x and \a y from the series. Does
    nothing if the point does not exist.
*/

/*!
    \qmlmethod XYSeries::remove(int index)
    Removes the point at the position specified by \a index from the series.
*/

/*!
    \qmlmethod XYSeries::removePoints(int index, int count)
    Removes the number of points specified by \a count from the series starting
    at the position specified by \a index.
*/

/*!
    \qmlmethod XYSeries::insert(int index, real x, real y)
    Inserts a point with the coordinates \a x and \a y to the position specified
    by \a index in the series. If the index is 0 or less than 0, the point is
    prepended to the list of points. If the index is equal to or greater than
    than the number of points in the series, the point is appended to the
    list of points.
*/

/*!
    \qmlmethod QPointF XYSeries::at(int index)
    Returns the point at the position specified by \a index. Returns (0, 0) if
    the index is not valid.
*/

/*!
    \internal

    Constructs an empty series object that is a child of \a parent.
    When the series object is added to QChart, instance ownerships is transferred.
*/
// 构造函数
QXYSeries::QXYSeries(QXYSeriesPrivate &d, QObject *parent)
    : QAbstractSeries(d, parent) // 基类构造
{
}

/*!
    Deletes the series. Series added to QChart instances are owned by them,
    and are deleted when the QChart instances are deleted.
*/
// 析构函数
QXYSeries::~QXYSeries()
{
}

/*!
    Adds the data point with the coordinates \a x and \a y to the series.
 */
// 追加点
void QXYSeries::append(qreal x, qreal y)
{
    append(QPointF(x, y));
}

/*!
   \overload
   Adds the data point \a point to the series.
 */
// 追加点
void QXYSeries::append(const QPointF &point)
{
    // 启用私有成员
    Q_D(QXYSeries);

    // 如果点有效
    if (isValidValue(point)) {
        // 存储点
        d->m_points << point;
        // 发送点增加信号
        emit pointAdded(d->m_points.count() - 1);
    }
}

/*!
   \overload
   Adds the list of data points specified by \a points to the series.
 */
// 追加点集
void QXYSeries::append(const QList<QPointF> &points)
{
    foreach (const QPointF &point , points)
        append(point); // 追加集
}

/*!
    Replaces the point with the coordinates \a oldX and \a oldY with the point
    with the coordinates \a newX and \a newY. Does nothing if the old point does
    not exist.

    \sa pointReplaced()
*/
// 替换点
void QXYSeries::replace(qreal oldX, qreal oldY, qreal newX, qreal newY)
{
    replace(QPointF(oldX, oldY), QPointF(newX, newY));
}

/*!
  Replaces the point specified by \a oldPoint with the one specified by
  \a newPoint.
  \sa pointReplaced()
*/
// 替换点
void QXYSeries::replace(const QPointF &oldPoint, const QPointF &newPoint)
{
    // 启用私有成员
    Q_D(QXYSeries);
    // 获取目标点索引
    int index = d->m_points.indexOf(oldPoint);
    // 获取索引失败
    if (index == -1)
        return;
    // 替换新点
    replace(index, newPoint);
}

/*!
  Replaces the point at the position specified by \a index with the point that
  has the coordinates \a newX and \a newY.
  \sa pointReplaced()
*/
// 替换点
void QXYSeries::replace(int index, qreal newX, qreal newY)
{
    // 替换
    replace(index, QPointF(newX, newY));
}

/*!
  Replaces the point at the position specified by \a index with the point
  specified by \a newPoint.
  \sa pointReplaced()
*/
// 替换点
void QXYSeries::replace(int index, const QPointF &newPoint)
{
    // 启用私有成员
    Q_D(QXYSeries);
    // 点有效
    if (isValidValue(newPoint)) {
        d->m_points[index] = newPoint; // 存储点
        emit pointReplaced(index); // 发送点信号
    }
}

/*!
  Replaces the current points with the points specified by \a points.
  \note This is much faster than replacing data points one by one,
  or first clearing all data, and then appending the new data. Emits QXYSeries::pointsReplaced()
  when the points have been replaced. However, note that using the overload that takes
  \c{QVector<QPointF>} as parameter is faster than using this overload.
  \sa pointsReplaced()
*/
// 替换点集
void QXYSeries::replace(QList<QPointF> points)
{
    // 替换
    replace(points.toVector());
}

/*!
  Replaces the current points with the points specified by \a points.
  \note This is much faster than replacing data points one by one,
  or first clearing all data, and then appending the new data. Emits QXYSeries::pointsReplaced()
  when the points have been replaced.
  \sa pointsReplaced()
*/
// 替换点集
void QXYSeries::replace(QVector<QPointF> points)
{
    // 启用点私有成员
    Q_D(QXYSeries);
    // 存储点集
    d->m_points = points;
    // 发送点集替换信号
    emit pointsReplaced();
}

/*!
  Removes the point that has the coordinates \a x and \a y from the series.
  \sa pointRemoved()
*/
// 移除点
void QXYSeries::remove(qreal x, qreal y)
{
    remove(QPointF(x, y));
}

/*!
  Removes the data point \a point from the series.
  \sa pointRemoved()
*/
// 移除点
void QXYSeries::remove(const QPointF &point)
{
    Q_D(QXYSeries);
    int index = d->m_points.indexOf(point);
    if (index == -1)
        return;
    remove(index);
}

/*!
  Removes the point at the position specified by \a index from the series.
  \sa pointRemoved()
*/
// 移除点
void QXYSeries::remove(int index)
{
    Q_D(QXYSeries);
    d->m_points.remove(index);
    emit pointRemoved(index);
}

/*!
  Removes the number of points specified by \a count from the series starting at
  the position specified by \a index.
  \sa pointsRemoved()
*/
// 移除点集
void QXYSeries::removePoints(int index, int count)
{
    // This function doesn't overload remove as there is chance for it to get mixed up with
    // remove(qreal, qreal) overload in some implicit casting cases.
    Q_D(QXYSeries);
    if (count > 0) {
        d->m_points.remove(index, count);
        emit pointsRemoved(index, count);
    }
}

/*!
  Inserts the data point \a point in the series at the position specified by
  \a index.
  \sa pointAdded()
*/
// 插入点
void QXYSeries::insert(int index, const QPointF &point)
{
    Q_D(QXYSeries);
    if (isValidValue(point)) {
        index = qMax(0, qMin(index, d->m_points.size()));
        d->m_points.insert(index, point);
        emit pointAdded(index);
    }
}

/*!
  Removes all points from the series.
  \sa pointsRemoved()
*/
// 清除点
void QXYSeries::clear()
{
    Q_D(QXYSeries);
    removePoints(0, d->m_points.size());
}

/*!
    Returns the points in the series as a list.
    Use pointsVector() for better performance.
*/
// 获取点集
QList<QPointF> QXYSeries::points() const
{
    Q_D(const QXYSeries);
    return d->m_points.toList();
}

/*!
    Returns the points in the series as a vector.
    This is more efficient than calling points().
*/
// 获取点集
QVector<QPointF> QXYSeries::pointsVector() const
{
    Q_D(const QXYSeries);
    return d->m_points;
}

/*!
    Returns the data point at the position specified by \a index in the internal
    points vector.
*/
// 取指定点
const QPointF &QXYSeries::at(int index) const
{
    Q_D(const QXYSeries);
    return d->m_points.at(index);
}

/*!
    Returns the number of data points in a series.
*/
// 点计数
int QXYSeries::count() const
{
    Q_D(const QXYSeries);
    return d->m_points.count();
}


/*!
    Sets the pen used for drawing points on the chart to \a pen. If the pen is
    not defined, the pen from the chart theme is used.
    \sa QChart::setTheme()
*/
// 设置画笔
void QXYSeries::setPen(const QPen &pen)
{
    Q_D(QXYSeries);
    if (d->m_pen != pen) { // 画笔变更
        bool emitColorChanged = d->m_pen.color() != pen.color(); // 检测颜色是否变更
        d->m_pen = pen; // 存储画笔
        emit d->updated(); // 发送更新信号
        if (emitColorChanged) // 颜色变更
            emit colorChanged(pen.color()); // 发送颜色变更信号
        emit penChanged(pen); // 发送画笔变更信号
    }
}

// 获取画笔
QPen QXYSeries::pen() const
{
    // 启用私有成员
    Q_D(const QXYSeries);
    // 默认画笔
    if (d->m_pen == QChartPrivate::defaultPen())
        return QPen();
    // 非默认画笔
    else
        return d->m_pen;
}

/*!
    Sets the brush used for drawing points on the chart to \a brush. If the
    brush is not defined, the brush from the chart theme setting is used.
    \sa QChart::setTheme()
*/
// 设置画刷
void QXYSeries::setBrush(const QBrush &brush)
{
    Q_D(QXYSeries);
    if (d->m_brush != brush) {
        d->m_brush = brush;
        emit d->updated();
    }
}

// 获取画刷
QBrush QXYSeries::brush() const
{
    Q_D(const QXYSeries);
    if (d->m_brush == QChartPrivate::defaultBrush())
        return QBrush();
    else
        return d->m_brush;
}

// 设置颜色
void QXYSeries::setColor(const QColor &color)
{
    QPen p = pen();
    if (p.color() != color) {
        p.setColor(color);
        setPen(p);
    }
}

// 获取颜色
QColor QXYSeries::color() const
{
    return pen().color();
}

// 设置点是否可见
void QXYSeries::setPointsVisible(bool visible)
{
    Q_D(QXYSeries);
    if (d->m_pointsVisible != visible) {
        d->m_pointsVisible = visible;
        emit d->updated();
    }
}

// 获取点是否可见
bool QXYSeries::pointsVisible() const
{
    Q_D(const QXYSeries);
    return d->m_pointsVisible;
}

// 设置点标签格式
void QXYSeries::setPointLabelsFormat(const QString &format)
{
    Q_D(QXYSeries);
    if (d->m_pointLabelsFormat != format) {
        d->m_pointLabelsFormat = format;
        emit pointLabelsFormatChanged(format);
    }
}

// 返回点标签格式
QString QXYSeries::pointLabelsFormat() const
{
    Q_D(const QXYSeries);
    return d->m_pointLabelsFormat;
}

// 设置点标签可见
void QXYSeries::setPointLabelsVisible(bool visible)
{
    Q_D(QXYSeries);
    if (d->m_pointLabelsVisible != visible) {
        d->m_pointLabelsVisible = visible;
        emit pointLabelsVisibilityChanged(visible);
    }
}

// 返回点标签可见
bool QXYSeries::pointLabelsVisible() const
{
    Q_D(const QXYSeries);
    return d->m_pointLabelsVisible;
}

// 设置点标签字体
void QXYSeries::setPointLabelsFont(const QFont &font)
{
    Q_D(QXYSeries);
    if (d->m_pointLabelsFont != font) {
        d->m_pointLabelsFont = font;
        emit pointLabelsFontChanged(font);
    }
}

// 返回点标签字体
QFont QXYSeries::pointLabelsFont() const
{
    Q_D(const QXYSeries);
    return d->m_pointLabelsFont;
}

// 设置点白标签颜色
void QXYSeries::setPointLabelsColor(const QColor &color)
{
    Q_D(QXYSeries);
    if (d->m_pointLabelsColor != color) {
        d->m_pointLabelsColor = color;
        emit pointLabelsColorChanged(color);
    }
}

// 返回点标签颜色
QColor QXYSeries::pointLabelsColor() const
{
    Q_D(const QXYSeries);
    if (d->m_pointLabelsColor == QChartPrivate::defaultPen().color())
        return QPen().color();
    else
        return d->m_pointLabelsColor;
}

// 设置点标签是否可以剪裁
void QXYSeries::setPointLabelsClipping(bool enabled)
{
    Q_D(QXYSeries);
    if (d->m_pointLabelsClipping != enabled) {
        d->m_pointLabelsClipping = enabled;
        emit pointLabelsClippingChanged(enabled);
    }
}

// 返回点标签是否可以剪裁
bool QXYSeries::pointLabelsClipping() const
{
    Q_D(const QXYSeries);
    return d->m_pointLabelsClipping;
}

/*!
    Stream operator for adding the data point \a point to the series.
    \sa append()
*/
// 追加点
QXYSeries &QXYSeries::operator<< (const QPointF &point)
{
    append(point);
    return *this;
}


/*!
    Stream operator for adding the list of data points specified by \a points
    to the series.
    \sa append()
*/
// 追加点集
QXYSeries &QXYSeries::operator<< (const QList<QPointF>& points)
{
    append(points);
    return *this;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 构造
QXYSeriesPrivate::QXYSeriesPrivate(QXYSeries *q)
    : QAbstractSeriesPrivate(q), // 所属序列
      m_pen(QChartPrivate::defaultPen()), // 画笔
      m_brush(QChartPrivate::defaultBrush()), // 画刷
      m_pointsVisible(false), // 点是否可见
      m_pointLabelsFormat(QLatin1String("@xPoint, @yPoint")), // 点标签格式
      m_pointLabelsVisible(false), // 标签是否可视
      m_pointLabelsFont(QChartPrivate::defaultFont()), // 标签字体
      m_pointLabelsColor(QChartPrivate::defaultPen().color()), // 标签颜色
      m_pointLabelsClipping(true) // 标签是否可以检测
{
}

// 初始化区域
void QXYSeriesPrivate::initializeDomain()
{
    // 初始化最小、最大x、y
    qreal minX(0);
    qreal minY(0);
    qreal maxX(1);
    qreal maxY(1);

    // 支持使用所属序列指针
    Q_Q(QXYSeries);

    // 获取所属序列点集
    const QVector<QPointF> &points = q->pointsVector();

    // 如果点集非空
    if (!points.isEmpty()) {

        // 记录最大、最小x、y
        minX = points[0].x();
        minY = points[0].y();
        maxX = minX;
        maxY = minY;

        // 统计点集中最大、最小x、y
        for (int i = 0; i < points.count(); i++) {
            qreal x = points[i].x();
            qreal y = points[i].y();
            minX = qMin(minX, x);
            minY = qMin(minY, y);
            maxX = qMax(maxX, x);
            maxY = qMax(maxY, y);
        }
    }

    // 设置区域范围
    domain()->setRange(minX, maxX, minY, maxY);
}

// 创建图例标记
QList<QLegendMarker*> QXYSeriesPrivate::createLegendMarkers(QLegend* legend)
{
    // 启用所属序列
    Q_Q(QXYSeries);

    // 创建并返回图例标记链表
    QList<QLegendMarker*> list;
    return list << new QXYLegendMarker(q,legend);
}

// 初始化轴
void QXYSeriesPrivate::initializeAxes()
{

}

// 默认轴的类型
QAbstractAxis::AxisType QXYSeriesPrivate::defaultAxisType(Qt::Orientation orientation) const
{
    Q_UNUSED(orientation);
    return QAbstractAxis::AxisTypeValue;
}

// 创建默认轴
QAbstractAxis* QXYSeriesPrivate::createDefaultAxis(Qt::Orientation orientation) const
{
    Q_UNUSED(orientation);
    return new QValueAxis;
}

// 初始化动画
void QXYSeriesPrivate::initializeAnimations(QtCharts::QChart::AnimationOptions options,
                                            int duration, QEasingCurve &curve)
{
    // 获取绘图项
    XYChart *item = static_cast<XYChart *>(m_item.data());
    // 确保绘图项合法
    Q_ASSERT(item);
    // 如果动画存在，停止并销毁
    if (item->animation())
        item->animation()->stopAndDestroyLater();
    // 重新设置动画
    if (options.testFlag(QChart::SeriesAnimations))
        item->setAnimation(new XYAnimation(item, duration, curve));
    else
        item->setAnimation(0);
    // 调用基类相应处理函数
    QAbstractSeriesPrivate::initializeAnimations(options, duration, curve);
}

// 绘制序列点标签
void QXYSeriesPrivate::drawSeriesPointLabels(QPainter *painter, const QVector<QPointF> &points,
                                             const int offset)
{
    // 如果点集为空
    if (points.size() == 0)
        return;

    // 格式标记字符串
    static const QString xPointTag(QLatin1String("@xPoint"));
    static const QString yPointTag(QLatin1String("@yPoint"));

    // 计算标签偏移
    const int labelOffset = offset + 2;

    // 设置标签字体、画笔
    painter->setFont(m_pointLabelsFont);
    painter->setPen(QPen(m_pointLabelsColor));
    QFontMetrics fm(painter->font());
    // m_points is used for the label here as it has the series point information
    // points variable passed is used for positioning because it has the coordinates
    // 计算点集数量
    const int pointCount = qMin(points.size(), m_points.size());
    // 遍历点集
    for (int i(0); i < pointCount; i++) {
        // 标签内容格式替换
        QString pointLabel = m_pointLabelsFormat;
        pointLabel.replace(xPointTag, presenter()->numberToString(m_points.at(i).x()));
        pointLabel.replace(yPointTag, presenter()->numberToString(m_points.at(i).y()));

        // Position text in relation to the point
        // 计算标签宽度
        int pointLabelWidth = fm.width(pointLabel);
        // 计算标签位置
        QPointF position(points.at(i));
        position.setX(position.x() - pointLabelWidth / 2);
        position.setY(position.y() - labelOffset);

        // 书写标签
        painter->drawText(position, pointLabel);
    }
}

#include "moc_qxyseries.cpp"
#include "moc_qxyseries_p.cpp"

QT_CHARTS_END_NAMESPACE
