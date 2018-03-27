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

#include <QtCharts/QAreaSeries>
#include <private/qareaseries_p.h>
#include <QtCharts/QLineSeries>
#include <private/areachartitem_p.h>
#include <private/abstractdomain_p.h>
#include <private/chartdataset_p.h>
#include <private/charttheme_p.h>
#include <QtCharts/QValueAxis>
#include <QtCharts/QAreaLegendMarker>
#include <private/qchart_p.h>

QT_CHARTS_BEGIN_NAMESPACE

/*!
    \class QAreaSeries
    \inmodule Qt Charts
    \brief The QAreaSeries class presents data in area charts.

    An area series is used to show quantitative data. It is based on a line series, in the way that
    the area between the boundary lines is emphasized with color. Since the area series is based on
    the line series, the QAreaSeries constructor needs a QLineSeries instance, which defines the
    \e upper boundary of the area. The area chart is drawn using the bottom of the plot area as the
    \e lower boundary by default. Instead of the bottom of the plot area, the lower boundary can be
    specified by another line. In that case, QAreaSeries should be initialized with two QLineSeries
    instances.

    \note The terms \e upper and \e lower boundary can be misleading in cases where the value of the
    lower boundary is greater than that of the upper boundary. The main point is that the
    area between these two boundary lines will be filled.

    See the \l {AreaChart Example} {area chart example} to learn how to create a simple area chart.
    \image examples_areachart.png
*/

/*!
    \qmltype AreaSeries
    \instantiates QAreaSeries
    \inqmlmodule QtCharts

    \inherits AbstractSeries

    \brief Presents data in area charts.

    An area series is used to show quantitative data. It is based on a line
    series, in the way that the area between the boundary lines is emphasized
    with color. The LineSeries type defines the \e upper boundary of the
    area. The area chart is drawn using the bottom of the plot area as the
    \e lower boundary by default. Instead of the bottom of the plot area, the
    lower boundary can be specified by another line. In that case, the
    AreaSeries should use two LineSeries types.

    \note The terms \e upper and \e lower boundary can be misleading in cases
    where the value of the lower boundary is greater than that of the upper
    boundary. The main point is that the area between these two boundary lines
    will be filled.

    \image examples_qmlchart4.png

    The following QML shows how to create a simple area chart:
    \snippet qmlchart/qml/qmlchart/View4.qml 1

    \note Adding the same line series to a chart and area series is not supported. The series used as
    boundary lines should be defined only for the area series.
*/

/*!
    \qmlproperty AbstractAxis AreaSeries::axisX
    The x-axis used for the series. If you leave both axisX and axisXTop
    undefined, a value axis is created for the series.
    \sa axisXTop, ValueAxis
*/

/*!
    \qmlproperty AbstractAxis AreaSeries::axisY
    The y-axis used for the series. If you leave both axisY and axisYRight
    undefined, a value axis is created for the series.
    \sa axisYRight, ValueAxis
*/

/*!
    \qmlproperty AbstractAxis AreaSeries::axisXTop
    The x-axis used for the series, drawn on top of the chart view.

    \note You can only provide either axisX or axisXTop, not both.
    \sa axisX
*/

/*!
    \qmlproperty AbstractAxis AreaSeries::axisYRight
    The y-axis used for the series, drawn to the right on the chart view.

    \note You can only provide either axisY or axisYRight, not both.
    \sa axisY
*/

/*!
    \qmlproperty AbstractAxis AreaSeries::axisAngular
    The angular axis used for the series, drawn around the polar chart view.
    \sa axisX, PolarChartView
*/

/*!
    \qmlproperty AbstractAxis AreaSeries::axisRadial
    The radial axis used for the series, drawn inside the polar chart view.
    \sa axisY, PolarChartView
*/

/*!
   \property QAreaSeries::upperSeries
   \brief The upper one of the two line series used to define area series boundaries.
*/
/*!
   \qmlproperty LineSeries AreaSeries::upperSeries
   The upper one of the two line series used to define area series boundaries.
*/

/*!
    \property QAreaSeries::lowerSeries
    \brief The lower one of the two line series used to define area series boundaries.

    \note If QAreaSeries was constructed without a\ lowerSeries, this is null.
*/
/*!
    \qmlproperty LineSeries AreaSeries::lowerSeries
    The lower one of the two line series used to define area series boundaries.

    \note If AreaSeries was constructed without a\ lowerSeries, this is null.
*/

/*!
    \property QAreaSeries::color
    \brief The fill (brush) color of the series. This is a convenience property for modifying the
    color of the brush.
    \sa QAreaSeries::brush()
*/
/*!
    \qmlproperty color AreaSeries::color
    Fill (brush) color of the series.
*/

/*!
    \property QAreaSeries::borderColor
    \brief The line (pen) color of the series. This is a convenience property for modifying the
    color of the pen.
    \sa QAreaSeries::pen()
*/
/*!
    \qmlproperty color AreaSeries::borderColor
    Line (pen) color of the series.
*/

/*!
    \qmlproperty real AreaSeries::borderWidth
    The width of the border line. By default, the width is 2.0.
*/

/*!
   \fn QPen QAreaSeries::pen() const
   \brief Returns the pen used to draw the line for this series.
    \sa setPen()
*/

/*!
   \fn QPen QAreaSeries::brush() const
   \brief Returns the brush used to draw the line for this series.
    \sa setBrush()
*/

/*!
    \qmlproperty QString AreaSeries::brushFilename
    The name of the file used as a brush image for the series.
*/

/*!
    \fn void QAreaSeries::colorChanged(QColor color)
    \brief This signal is emitted when the fill (brush) color changes to \a color.
*/

/*!
    \fn void QAreaSeries::borderColorChanged(QColor color)
    \brief This signal is emitted when the line (pen) color changes to \a color.
*/

/*!
    \fn void QAreaSeries::clicked(const QPointF& point)
    \brief This signal is emitted when the user triggers a press on \a point by clicking it in an
    area chart.
    \sa pressed, released, doubleClicked
*/
/*!
    \qmlsignal AreaSeries::clicked(point point)
    This signal is emitted when the user triggers a press on \a point by clicking it in an
    area chart.

    The corresponding signal handler is \c {onClicked}.

    \sa pressed, released, doubleClicked
*/

/*!
    \fn void QAreaSeries::hovered(const QPointF &point, bool state)
    This signal is emitted when the user hovers the mouse cursor over a series or moves it away from
    the series. \a point shows the origin (coordinate) of the hover event. \a state is \c true when
    the cursor hovers over the series and turns \e false when it moves away from the series.
*/
/*!
    \qmlsignal AreaSeries::hovered(point point, bool state)
    This signal is emitted when the user hovers the mouse cursor over a series or moves it away from
    the series. \a point shows the origin (coordinate) of the hover event. \a state is \c true when
    the cursor hovers over the series and turns \e false when it moves away from the series.

    The corresponding signal handler is \c {onHovered}.
*/

/*!
    \fn void QAreaSeries::pressed(const QPointF& point)
    \brief This signal is emitted when the user presses the point specified by \a point
    in an area chart.
    \sa clicked, released, doubleClicked
*/
/*!
    \qmlsignal AreaSeries::pressed(point point)
    This signal is emitted when the user presses the point specified by \a point in an area chart.

    The corresponding signal handler is \c {onPressed}.

    \sa clicked, released, doubleClicked
*/

/*!
    \fn void QAreaSeries::released(const QPointF& point)
    \brief This signal is emitted when the user releases a press that was triggered on
    \a point in an area chart.
    \sa pressed, clicked, doubleClicked
*/
/*!
    \qmlsignal AreaSeries::released(point point)
    This signal is emitted when the user releases a press that was triggered on
    \a point in an area chart.

    The corresponding signal handler is \c {onReleased}.

    \sa pressed, clicked, doubleClicked
*/

/*!
    \fn void QAreaSeries::doubleClicked(const QPointF& point)
    \brief This signal is emitted when the user triggers the first press in an area chart
    by doubleclicking \a point.
    \sa pressed, released, clicked
*/
/*!
    \qmlsignal AreaSeries::doubleClicked(point point)
    This signal is emitted when the user triggers the first press in an area chart
    by doubleclicking \a point.

    The corresponding signal handler is \c {onDoubleClicked}.

    \sa pressed, released, clicked
*/

/*!
    \fn void QAreaSeries::selected()
    \internal
*/

/*!
    \fn void QAreaSeriesPrivate::updated()
    \internal
*/

/*!
    \property QAreaSeries::pointLabelsFormat
    \brief The format used for showing labels with series points.

    QAreaSeries supports the following format tags:
    \table
      \row
        \li @xPoint      \li The x value of the data point
      \row
        \li @yPoint      \li The y value of the data point
    \endtable

    For example, the following usage of the format tags would produce labels that have the data
    point (x, y) shown inside brackets separated by a comma:
    \code
    series->setPointLabelsFormat("(@xPoint, @yPoint)");
    \endcode

    By default, the label format is set to \c{@xPoint, @yPoint}. The labels are shown on the plot
    area, whereas labels on the edge of the plot area are cut. If the points are close to each
    other, the labels may overlap.

    \sa QAreaSeries::pointLabelsVisible, QAreaSeries::pointLabelsFont, QAreaSeries::pointLabelsColor
*/
/*!
    \qmlproperty string AreaSeries::pointLabelsFormat
    The format used for showing labels with series points.

    \sa QAreaSeries::pointLabelsFormat, pointLabelsVisible, pointLabelsFont, pointLabelsColor
*/
/*!
    \fn void QAreaSeries::pointLabelsFormatChanged(const QString &format)
    This signal is emitted when the \a format of data point labels is changed.
*/

/*!
    \property QAreaSeries::pointLabelsVisible
    \brief The visibility of data point labels. False by default.

    \sa QAreaSeries::pointLabelsFormat, QAreaSeries::pointLabelsClipping
*/
/*!
    \qmlproperty bool AreaSeries::pointLabelsVisible
    Defines the visibility of data point labels.

    \sa pointLabelsFormat, pointLabelsClipping
*/
/*!
    \fn void QAreaSeries::pointLabelsVisibilityChanged(bool visible)
    This signal is emitted when the visibility of the data point labels changes to \a visible.
*/

/*!
    \property QAreaSeries::pointLabelsFont
    \brief The font used for data point labels.

    \sa QAreaSeries::pointLabelsFormat
*/
/*!
    \qmlproperty font AreaSeries::pointLabelsFont
    Defines the font used for data point labels.

    \sa pointLabelsFormat
*/
/*!
    \fn void QAreaSeries::pointLabelsFontChanged(const QFont &font);
    This signal is emitted when the font used for data point labels changes to \a font.
*/

/*!
    \property QAreaSeries::pointLabelsColor
    \brief The color used for data point labels. By default, the color is the color of the brush
    defined for labels in the theme.

    \sa QAreaSeries::pointLabelsFormat
*/
/*!
    \qmlproperty font AreaSeries::pointLabelsColor
    Defines the color used for data point labels. By default, the color is the color of the brush
    defined for labels in the theme.

    \sa pointLabelsFormat
*/
/*!
    \fn void QAreaSeries::pointLabelsColorChanged(const QColor &color);
    This signal is emitted when the color used for data point labels changes to \a color.
*/

/*!
    \property QAreaSeries::pointLabelsClipping
    \brief The clipping for data point labels. True by default. The labels on the edge of the plot
    area are cut when clipping is enabled.

    \sa pointLabelsVisible
*/
/*!
    \qmlproperty bool AreaSeries::pointLabelsClipping
    Defines the clipping for data point labels. True by default. The labels on the edge of the plot
    area are cut when clipping is enabled.

    \sa pointLabelsVisible
*/
/*!
    \fn void QAreaSeries::pointLabelsClippingChanged(bool clipping)
    This signal is emitted when the clipping of the data point labels changes to \a clipping.
*/

/*!
    Constructs an area series object that will be spanned between an \a upperSeries line and a
    \a lowerSeries line.  If no \a lowerSeries is passed to the constructor, the x-axis is used
    as the lower bound instead.

    The QAreaSeries does not own the upper or lower series, but the ownership stays with the caller.
    When the series object is added to QChartView or QChart, the instance ownership is transferred.
*/
// 构造
QAreaSeries::QAreaSeries(QLineSeries *upperSeries, QLineSeries *lowerSeries)
    : QAbstractSeries(*new QAreaSeriesPrivate(upperSeries, lowerSeries, this), upperSeries)
{
    // 上限存在
    if (upperSeries)
        upperSeries->d_ptr->setBlockOpenGL(true); // 阻止OpenGL
    // 下限存在
    if (lowerSeries)
        lowerSeries->d_ptr->setBlockOpenGL(true); // 阻止OpenGL
}

/*!
    Constructs an area series object without an upper or a lower series with the \a parent object.
*/
// 序列
QAreaSeries::QAreaSeries(QObject *parent)
    : QAbstractSeries(*new QAreaSeriesPrivate(0, 0, this), parent)
{
}

/*!
    Destroys the object.
*/
// 析构
QAreaSeries::~QAreaSeries()
{
    // 启用区域序列私有成员
    Q_D(QAreaSeries);

    // 图表存在
    if (d->m_chart)
        d->m_chart->removeSeries(this); // 释放序列
}

/*!
    Returns QAbstractSeries::SeriesTypeArea.
*/
// 获取序列类型
QAbstractSeries::SeriesType QAreaSeries::type() const
{
    return QAbstractSeries::SeriesTypeArea;
}

/*!
    Sets the \a series that is to be used as the area chart upper series.
    If the upper series is null, the area chart is not drawn, even if it has a lower series.
*/
// 设置上限序列
void QAreaSeries::setUpperSeries(QLineSeries *series)
{
    // 获取区域序列私有成员
    Q_D(QAreaSeries);

    // 上限序列与目标序列不同
    if (d->m_upperSeries != series) {
        // 如果序列存在
        if (series)
            series->d_ptr->setBlockOpenGL(true);
        // 保存上限序列
        d->m_upperSeries = series;
        // 图表项目存在
        if (!d->m_item.isNull())
            static_cast<AreaChartItem *>(d->m_item.data())->setUpperSeries(series); // 设置上限
    }
}

// 获取上限序列
QLineSeries *QAreaSeries::upperSeries() const
{
    // 启用区域序列私有成员
    Q_D(const QAreaSeries);

    // 返回上限序列
    return d->m_upperSeries;
}

/*!
    Sets the \a series that is to be used as the area chart lower series.
*/
// 设置下限序列
void QAreaSeries::setLowerSeries(QLineSeries *series)
{
    // 启用区域序列私有成员
    Q_D(QAreaSeries);

    // 下限序列发生变更
    if (d->m_lowerSeries != series) {
        // 序列存在
        if (series)
            series->d_ptr->setBlockOpenGL(true); // 设置阻塞OpenGL
        // 设置下限序列
        d->m_lowerSeries = series;
        // 图表项存在
        if (!d->m_item.isNull())
            static_cast<AreaChartItem *>(d->m_item.data())->setLowerSeries(series); // 设置下限序列
    }
}

// 获取下限序列
QLineSeries *QAreaSeries::lowerSeries() const
{
    // 启用区域序列私有成员
    Q_D(const QAreaSeries);

    // 返回下限序列
    return d->m_lowerSeries;
}

/*!
    Sets the \a pen used for drawing the area outline.
*/
// 设置画笔
void QAreaSeries::setPen(const QPen &pen)
{
    // 获取区域序列私有成员
    Q_D(QAreaSeries);

    // 如果画笔发生变更
    if (d->m_pen != pen) {
        // 是否发送颜色变更信号
        bool emitColorChanged = pen.color() != d->m_pen.color();
        // 保存画笔
        d->m_pen = pen;
        // 发送更新信号
        emit d->updated();
        // 如果需要发送颜色变更信号
        if (emitColorChanged)
            emit borderColorChanged(pen.color()); // 发送信号
    }
}

// 获取画笔
QPen QAreaSeries::pen() const
{
    // 启用区域序列私有成员
    Q_D(const QAreaSeries);

    // 如果画笔为默认画笔
    if (d->m_pen == QChartPrivate::defaultPen())
        return QPen();
    // 如果画笔为非默认画笔
    else
        return d->m_pen;
}

/*!
    Sets the \a brush used for filling the area.
*/
// 设置画刷
void QAreaSeries::setBrush(const QBrush &brush)
{
    // 启用区域序列私有成员
    Q_D(QAreaSeries);

    // 如果画刷发生变化
    if (d->m_brush != brush) {

        // 是否颜色发生变化
        bool emitColorChanged = brush.color() != d->m_brush.color();
        // 保存画刷
        d->m_brush = brush;
        // 发送更新信号
        emit d->updated();
        // 如果颜色变更
        if (emitColorChanged)
            emit colorChanged(brush.color());
    }
}

// 获取画刷
QBrush QAreaSeries::brush() const
{
    // 获取区域序列私有成员
    Q_D(const QAreaSeries);
    // 如果画刷是默认画刷
    if (d->m_brush == QChartPrivate::defaultBrush())
        return QBrush();
    // 返回相应画刷
    else
        return d->m_brush;
}

// 设置颜色
void QAreaSeries::setColor(const QColor &color)
{
    // 获取画刷
    QBrush b = brush();
    // 画刷为空
    if (b == QBrush())
        b.setStyle(Qt::SolidPattern);
    // 设置颜色
    b.setColor(color);
    // 设置画刷
    setBrush(b);
}

// 获取颜色
QColor QAreaSeries::color() const
{
    return brush().color();
}

// 设置边界颜色
void QAreaSeries::setBorderColor(const QColor &color)
{
    // 获取画笔
    QPen p = pen();
    // 设置画笔颜色
    p.setColor(color);
    // 设置画笔
    setPen(p);
}

// 获取边界颜色
QColor QAreaSeries::borderColor() const
{
    return pen().color();
}

/*!
    Determines whether data points are \a visible and should be drawn on the line.
*/
// 设置点集是否可见
void QAreaSeries::setPointsVisible(bool visible)
{
    // 启用区域序列私有成员
    Q_D(QAreaSeries);
    // 如果点集是否可视与目标不符合
    if (d->m_pointsVisible != visible) {
        // 设置可视
        d->m_pointsVisible = visible;
        // 发送更新信号
        emit d->updated();
    }
}

/*!
    Returns whether the points are drawn for this series.
    \sa setPointsVisible()
*/
// 获取点集是否可视
bool QAreaSeries::pointsVisible() const
{
    // 启用区域序列私有成员
    Q_D(const QAreaSeries);
    // 返回点集是否可视
    return d->m_pointsVisible;
}

// 设置点标签格式
void QAreaSeries::setPointLabelsFormat(const QString &format)
{
    // 启用区域序列私有成员
    Q_D(QAreaSeries);
    // 设置点标签格式
    if (d->m_pointLabelsFormat != format) {
        d->m_pointLabelsFormat = format;
        emit pointLabelsFormatChanged(format);
    }
}

// 获取点标签格式
QString QAreaSeries::pointLabelsFormat() const
{
    // 启用区域序列私有成员
    Q_D(const QAreaSeries);
    // 获取点标签格式
    return d->m_pointLabelsFormat;
}

// 设置点标签是否可视
void QAreaSeries::setPointLabelsVisible(bool visible)
{
    // 启用区域序列私有成员
    Q_D(QAreaSeries);
    // 设置点
    if (d->m_pointLabelsVisible != visible) {
        d->m_pointLabelsVisible = visible;
        emit pointLabelsVisibilityChanged(visible);
    }
}

// 点标签是否可见
bool QAreaSeries::pointLabelsVisible() const
{
    // 启用区域序列私有成员
    Q_D(const QAreaSeries);
    // 获取标签是否可见
    return d->m_pointLabelsVisible;
}

// 设置标签字体
void QAreaSeries::setPointLabelsFont(const QFont &font)
{
    // 启用区域序列私有成员
    Q_D(QAreaSeries);
    // 如果字体发生变化，设置相应字体
    if (d->m_pointLabelsFont != font) {
        d->m_pointLabelsFont = font;
        emit pointLabelsFontChanged(font);
    }
}

// 获取点标签字体
QFont QAreaSeries::pointLabelsFont() const
{
    // 启用区域序列私有成员
    Q_D(const QAreaSeries);
    // 获取点标签字体
    return d->m_pointLabelsFont;
}

// 设置点标签颜色
void QAreaSeries::setPointLabelsColor(const QColor &color)
{
    // 启用区域序列私有成员
    Q_D(QAreaSeries);
    // 如果点标签颜色变更
    if (d->m_pointLabelsColor != color) {
        d->m_pointLabelsColor = color;
        emit pointLabelsColorChanged(color);
    }
}

// 获取点标签颜色
QColor QAreaSeries::pointLabelsColor() const
{
    // 启用区域序列私有成员
    Q_D(const QAreaSeries);
    // 如果当前标签颜色为默认颜色
    if (d->m_pointLabelsColor == QChartPrivate::defaultPen().color())
        return QPen().color();
    // 如果当前标签颜色为非默认颜色
    else
        return d->m_pointLabelsColor;
}

// 设置点是否可以剪裁
void QAreaSeries::setPointLabelsClipping(bool enabled)
{
    // 启用区域序列私有成员
    Q_D(QAreaSeries);
    // 点标签是否支持剪裁
    if (d->m_pointLabelsClipping != enabled) {
        d->m_pointLabelsClipping = enabled;
        emit pointLabelsClippingChanged(enabled);
    }
}

// 获取点标签是否支持剪裁
bool QAreaSeries::pointLabelsClipping() const
{
    Q_D(const QAreaSeries);
    return d->m_pointLabelsClipping;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 构造
QAreaSeriesPrivate::QAreaSeriesPrivate(QLineSeries *upperSeries, QLineSeries *lowerSeries, QAreaSeries *q)
    : QAbstractSeriesPrivate(q), // 所属序列
      m_brush(QChartPrivate::defaultBrush()), // 画刷
      m_pen(QChartPrivate::defaultPen()), // 画笔
      m_upperSeries(upperSeries), // 上界
      m_lowerSeries(lowerSeries), // 下界
      m_pointsVisible(false), // 点集是否可视
      m_pointLabelsFormat(QLatin1String("@xPoint, @yPoint")), // 标签格式
      m_pointLabelsVisible(false), // 点标签是否可见
      m_pointLabelsFont(QChartPrivate::defaultFont()), // 点标签字体
      m_pointLabelsColor(QChartPrivate::defaultPen().color()), // 点标签颜色
      m_pointLabelsClipping(true) // 点标签是否可剪裁
{
}

// 初始化区域
void QAreaSeriesPrivate::initializeDomain()
{
    //启用区域序列私有成员
    Q_Q(QAreaSeries);

    // 初始化最大、最小x、y
    qreal minX(0.0);
    qreal minY(0.0);
    qreal maxX(1.0);
    qreal maxY(1.0);

    // 取上、下限
    QLineSeries *upperSeries = q->upperSeries();
    QLineSeries *lowerSeries = q->lowerSeries();

    // 上限存在
    if (upperSeries) {

        // 获取上限点集
        const QVector<QPointF> &points = upperSeries->pointsVector();

        // 点集非空
        if (!points.isEmpty()) {

            // 计算上限最大、最小x、y
            minX = points[0].x();
            minY = points[0].y();
            maxX = minX;
            maxY = minY;

            for (int i = 0; i < points.count(); i++) {
                qreal x = points[i].x();
                qreal y = points[i].y();
                minX = qMin(minX, x);
                minY = qMin(minY, y);
                maxX = qMax(maxX, x);
                maxY = qMax(maxY, y);
            }
        }
    }

    // 下限存在
    if (lowerSeries) {

        // 下限点集
        const QVector<QPointF> &points = lowerSeries->pointsVector();

        // 点集为空
        if (!points.isEmpty()) {

            // 上限为空
            if (!upperSeries) {

                // 初始化最大、最小x、y
                minX = points[0].x();
                minY = points[0].y();
                maxX = minX;
                maxY = minY;

            }

            // 计算最大、最小x、y
            for (int i = 0; i < points.count(); i++) {
                qreal x = points[i].x();
                qreal y = points[i].y();
                minX = qMin(minX, x);
                minY = qMin(minY, y);
                maxX = qMax(maxX, x);
                maxY = qMax(maxY, y);
            }
        }
    }

    // 设置区域范围
    domain()->setRange(minX, maxX, minY, maxY);
}

// 初始化图像
void QAreaSeriesPrivate::initializeGraphics(QGraphicsItem* parent)
{
    // 启用区域序列私有成员
    Q_Q(QAreaSeries);
    // 创建区域
    AreaChartItem *area = new AreaChartItem(q,parent);
    // 设置区域
    m_item.reset(area);
    // 调用基类相应处理函数
    QAbstractSeriesPrivate::initializeGraphics(parent);
}

// 初始化动画
void  QAreaSeriesPrivate::initializeAnimations(QChart::AnimationOptions options, int duration,
                                               QEasingCurve &curve)
{
    // 启用区域序列私有成员
    Q_Q(QAreaSeries);

    // 获取当前区域图表项
    AreaChartItem *area = static_cast<AreaChartItem *>(m_item.data());

    // 区域上限存在且动画存在
    if (q->upperSeries() && area->upperLineItem()->animation())
        // 停止并且销毁
        area->upperLineItem()->animation()->stopAndDestroyLater();
    // 区域下限存在且动画存在
    if (q->lowerSeries() && area->lowerLineItem()->animation())
        // 停止并且销毁
        area->lowerLineItem()->animation()->stopAndDestroyLater();

    // 为上限、下限创建动画
    if (options.testFlag(QChart::SeriesAnimations)) {
        area->upperLineItem()->setAnimation(new XYAnimation(area->upperLineItem(), duration,
                                                            curve));
        if (q->lowerSeries())
            area->lowerLineItem()->setAnimation(new XYAnimation(area->lowerLineItem(), duration,
                                                                curve));
    } else {
        if (q->upperSeries())
            area->upperLineItem()->setAnimation(0);
        if (q->lowerSeries())
               area->lowerLineItem()->setAnimation(0);
    }

    // 初始化动画
    QAbstractSeriesPrivate::initializeAnimations(options, duration, curve);
}

// 创建图例标记
QList<QLegendMarker*> QAreaSeriesPrivate::createLegendMarkers(QLegend* legend)
{
    // 启用区域序列私有成员
    Q_Q(QAreaSeries);
    // 创建图列标记，并存储到列表
    QList<QLegendMarker*> list;
    return list << new QAreaLegendMarker(q,legend);
}

// 初始化轴
void QAreaSeriesPrivate::initializeAxes()
{

}

// 默认轴类型
QAbstractAxis::AxisType QAreaSeriesPrivate::defaultAxisType(Qt::Orientation orientation) const
{
    Q_UNUSED(orientation);
    return QAbstractAxis::AxisTypeValue;
}

// 创建默认轴
QAbstractAxis* QAreaSeriesPrivate::createDefaultAxis(Qt::Orientation orientation) const
{
    Q_UNUSED(orientation);
    return new QValueAxis;
}

// 初始化主题
void QAreaSeriesPrivate::initializeTheme(int index, ChartTheme* theme, bool forced)
{
    // 启用区域序列
    Q_Q(QAreaSeries);

    // 获取序列梯度
    const QList<QGradient> gradients = theme->seriesGradients();
    // 获取序列颜色
    const QList<QColor> colors = theme->seriesColors();

    // 设置画笔
    if (forced || QChartPrivate::defaultPen() == m_pen) {
        QPen pen;
        pen.setColor(ChartThemeManager::colorAt(gradients.at(index % gradients.size()), 0.0));
        pen.setWidthF(2);
        q->setPen(pen);
    }

    // 设置画刷
    if (forced || QChartPrivate::defaultBrush() == m_brush) {
        QBrush brush(colors.at(index % colors.size()));
        q->setBrush(brush);
    }

    // 设置点标签颜色
    if (forced || QChartPrivate::defaultPen().color() == m_pointLabelsColor) {
        QColor color = theme->labelBrush().color();
        q->setPointLabelsColor(color);
    }
}


#include "moc_qareaseries.cpp"
#include "moc_qareaseries_p.cpp"

QT_CHARTS_END_NAMESPACE
