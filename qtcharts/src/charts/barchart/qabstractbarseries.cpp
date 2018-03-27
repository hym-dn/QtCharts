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

#include <QtCharts/QAbstractBarSeries>
#include <private/qabstractbarseries_p.h>
#include <QtCharts/QBarSet>
#include <private/qbarset_p.h>
#include <private/abstractdomain_p.h>
#include <private/chartdataset_p.h>
#include <private/charttheme_p.h>
#include <QtCharts/QValueAxis>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QBarLegendMarker>
#include <private/baranimation_p.h>
#include <private/abstractbarchartitem_p.h>
#include <private/qchart_p.h>

QT_CHARTS_BEGIN_NAMESPACE

/*!
    \class QAbstractBarSeries
    \inmodule Qt Charts
    \brief The QAbstractBarSeries class is an abstract parent class for all bar series classes.

    In bar charts, bars are defined as bar sets that contain one data value for each category.
    The position of a bar is specified by the category and its height by the data value. Bar
    series that contain multiple bar sets group together bars that belong to the same category.
    The way the bars are displayed is determined by the subclass of this class chosen to create
    the bar chart.

    If a QValueAxis is used instead of QBarCategoryAxis for the main bar axis, the bars are
    grouped around the index value of the category.

    See the \l {BarChart Example} {bar chart example} to learn how to use the QBarSeries class
    to create a simple bar chart.
    \image examples_barchart.png

    \sa QBarSet, QBarSeries, QStackedBarSeries, QPercentBarSeries
    \sa QHorizontalBarSeries, QHorizontalStackedBarSeries, QHorizontalPercentBarSeries
*/
/*!
    \qmltype AbstractBarSeries
    \instantiates QAbstractBarSeries
    \inqmlmodule QtCharts

    \inherits AbstractSeries

    \brief An abstract parent type for all bar series types.

    In bar charts, bars are defined as bar sets that contain one data value for each category.
    The position of a bar is specified by the category and its height by the data value. Bar
    series that contain multiple bar sets group together bars that belong to the same category.
    The way the bars are displayed is determined by the type chosen to create the bar chart:
    BarSeries, StackedBarSeries, PercentBarSeries, HorizontalBarSeries, HorizontalStackedBarSeries,
    or HorizontalPercentBarSeries.

    If a ValueAxis type is used instead of a BarCategoryAxis type for the main bar axis, the
    bars are grouped around the index value of the category.

    The following QML code snippet shows how to use the BarSeries and BarCategoryAxis type
    to create a simple bar chart:
    \snippet qmlchart/qml/qmlchart/View6.qml 1

    \beginfloatleft
    \image examples_qmlchart6.png
    \endfloat
    \clearfloat
*/

/*!
    \qmlproperty AbstractAxis AbstractBarSeries::axisX
    The x-axis used for the series. If you leave both axisX and axisXTop undefined, a
    BarCategoryAxis is created for the series.
    \sa axisXTop
*/

/*!
    \qmlproperty AbstractAxis AbstractBarSeries::axisY
    The y-axis used for the series. If you leave both axisY and axisYRight undefined, a
    ValueAxis is created for the series.
    \sa axisYRight
*/

/*!
    \qmlproperty AbstractAxis AbstractBarSeries::axisXTop
    The x-axis used for the series, drawn on top of the chart view.

    \note You can only provide either axisX or axisXTop, but not both.
    \sa axisX
*/

/*!
    \qmlproperty AbstractAxis AbstractBarSeries::axisYRight
    The y-axis used for the series, drawn to the right of the chart view.

    \note You can only provide either axisY or axisYRight, but not both.
    \sa axisY
*/

/*!
    \property QAbstractBarSeries::barWidth
    \brief The width of the bars of the series.

    The unit of width is the unit of the x-axis. The minimum width for bars is zero, and negative
    values are treated as zero. Setting the width to zero means that the width of the bar on the
    screen is one pixel regardless of the scale of the x-axis. Bars wider than zero are scaled
    using the x-axis scale.

    \note When used with QBarSeries, this value specifies the width of a group of bars instead of
    that of a single bar.
    \sa QBarSeries
*/
/*!
    \qmlproperty real AbstractBarSeries::barWidth
    The unit of width is the unit of the x-axis. The minimum width for bars is zero, and negative
    values are treated as zero. Setting the width to zero means that the width of the bar on the
    screen is one pixel regardless of the scale of the x-axis. Bars wider than zero are scaled
    using the x-axis scale.

    \note When used with the BarSeries type, this value specifies the width of a group of bars
    instead of that of a single bar.
*/

/*!
    \property QAbstractBarSeries::count
    \brief The number of bar sets in a bar series.
*/
/*!
    \qmlproperty int AbstractBarSeries::count
    The number of bar sets in a bar series.
*/

/*!
    \property QAbstractBarSeries::labelsVisible
    \brief The visibility of the labels in a bar series.
*/
/*!
    \qmlproperty bool AbstractBarSeries::labelsVisible
    The visibility of the labels in a bar series.
*/

/*!
    \property QAbstractBarSeries::labelsFormat
    \brief The format used for showing labels in a bar series.

    QAbstractBarSeries supports the following format tag:
    \table
      \row
        \li @value      \li The value of the bar
    \endtable

    For example, the following usage of the format tags would produce labels that show the value
    followed by the unit (u):
    \code
    series->setLabelsFormat("@value u");
    \endcode

    By default, the labels show the value of the bar. For the percent bar series, \e % is added
    after the value. The labels are shown on the plot area, whereas labels on the edge of the plot
    area are cut. If the bars are close to each other, the labels may overlap.

    \sa labelsVisible, labelsPosition, labelsPrecision
*/
/*!
    \qmlproperty string AbstractBarSeries::labelsFormat
    The format used for showing labels in a bar series.

    \sa QAbstractBarSeries::labelsFormat, labelsVisible, labelsPosition
*/
/*!
    \fn void QAbstractBarSeries::labelsFormatChanged(const QString &format)
    This signal is emitted when the \a format of data value labels changes.
*/

/*!
 \enum QAbstractBarSeries::LabelsPosition

 This enum value describes the position of the data value labels:

 \value LabelsCenter Label is located in the center of the bar.
 \value LabelsInsideEnd Label is located inside the bar at the top.
 \value LabelsInsideBase Label is located inside the bar at the bottom.
 \value LabelsOutsideEnd Label is located outside the bar at the top.
 */

/*!
    \property QAbstractBarSeries::labelsPosition
    \brief The position of value labels.

    \sa labelsVisible, labelsFormat
*/
/*!
    \qmlproperty enumeration AbstractBarSeries::labelsPosition

    The position of the data value labels:

    \value  AbstractBarSeries.LabelsCenter
            Label is located in the center of the bar.
    \value  AbstractBarSeries.LabelsInsideEnd
            Label is located inside the bar at the top.
    \value  AbstractBarSeries.LabelsInsideBase
            Label is located inside the bar at the bottom.
    \value  AbstractBarSeries.LabelsOutsideEnd
            Label is located outside the bar at the top.

    \sa labelsVisible, labelsFormat
*/
/*!
    \fn void QAbstractBarSeries::labelsPositionChanged(QAbstractBarSeries::LabelsPosition position)
    This signal is emitted when the \a position of value labels changes.
*/

/*!
    \property QAbstractBarSeries::labelsAngle
    \brief The angle of the value labels in degrees.
*/
/*!
    \qmlproperty real AbstractBarSeries::labelsAngle
    The angle of the value labels in degrees.
*/
/*!
    \fn void QAbstractBarSeries::labelsAngleChanged(qreal angle)
    This signal is emitted when the \a angle of the value labels changes.
*/

/*!
    \property QAbstractBarSeries::labelsPrecision
    \brief The maximum amount of significant digits shown in value labels.

    Default value is 6.
*/
/*!
    \qmlproperty real AbstractBarSeries::labelsPrecision
    The maximum amount of significant digits shown in value labels.

    Default value is 6.
*/
/*!
    \fn void QAbstractBarSeries::labelsPrecisionChanged(int precision)
    This signal is emitted when the \a precision of the value labels changes.
*/

/*!
    \fn void QAbstractBarSeries::clicked(int index, QBarSet *barset)
    This signal is emitted when the user clicks the bar specified by \a index
    in the bar set specified by \a barset.
*/
/*!
    \qmlsignal AbstractBarSeries::clicked(int index, BarSet barset)
    This signal is emitted when the user clicks the bar specified by \a index
    in the bar set specified by \a barset.

    The corresponding signal handler is \c onClicked.
*/

/*!
    \fn void QAbstractBarSeries::pressed(int index, QBarSet *barset)
    This signal is emitted when the user clicks the bar specified by \a index
    in the bar set specified by \a barset and holds down the mouse button.
*/
/*!
    \qmlsignal AbstractBarSeries::pressed(int index, BarSet barset)
    This signal is emitted when the user clicks the bar specified by \a index
    in the bar set specified by \a barset and holds down the mouse button.

    The corresponding signal handler is \c onPressed.
*/

/*!
    \fn void QAbstractBarSeries::released(int index, QBarSet *barset)
    This signal is emitted when the user releases the mouse press on the bar
    specified by \a index in the bar set specified by \a barset.
*/
/*!
    \qmlsignal AbstractBarSeries::released(int index, BarSet barset)
    This signal is emitted when the user releases the mouse press on the bar
    specified by \a index in the bar set specified by \a barset.

    The corresponding signal handler is \c onReleased.
*/

/*!
    \fn void QAbstractBarSeries::doubleClicked(int index, QBarSet *barset)
    This signal is emitted when the user double-clicks the bar specified by \a index
    in the bar set specified by \a barset.
*/
/*!
    \qmlsignal AbstractBarSeries::doubleClicked(int index, BarSet barset)
    This signal is emitted when the user double-clicks the bar specified by \a index
    in the bar set specified by \a barset.

    The corresponding signal handler is \c onDoubleClicked.
*/

/*!
    \fn void QAbstractBarSeries::hovered(bool status, int index, QBarSet* barset)

    This signal is emitted when a mouse is hovered over the bar specified by \a index in the
    bar set specified by \a barset. When the mouse moves over the bar, \a status turns \c true,
    and when the mouse moves away again, it turns \c false.
*/
/*!
    \qmlsignal AbstractBarSeries::hovered(bool status, int index, BarSet barset)

    This signal is emitted when a mouse is hovered over the bar specified by \a index in the
    bar set specified by \a barset. When the mouse moves over the bar, \a status turns \c true,
    and when the mouse moves away again, it turns \c false.

    The corresponding signal handler is \c onHovered.
*/

/*!
    \fn void QAbstractBarSeries::countChanged()
    This signal is emitted when the number of bar sets is changed, for example by append() or
    remove().
*/

/*!
    \fn void QAbstractBarSeries::labelsVisibleChanged()
    This signal is emitted when the labels' visibility changes.
    \sa isLabelsVisible(), setLabelsVisible()
*/

/*!
    \fn void QAbstractBarSeries::barsetsAdded(QList<QBarSet*> sets)
    This signal is emitted when the bar sets specified by \a sets are added to the series.
    \sa append(), insert()
*/
/*!
    \qmlsignal AbstractBarSeries::barsetsAdded()
    This signal is emitted when bar sets are added to the series.

    The corresponding signal handler is \c onBarsetsAdded.
*/

/*!
    \fn void QAbstractBarSeries::barsetsRemoved(QList<QBarSet*> sets)
    This signal is emitted when the bar sets specified by \a sets are removed from the series.
    \sa remove()
*/
/*!
    \qmlsignal AbstractBarSeries::barsetsRemoved()
    This signal is emitted when bar sets are removed from the series.

    The corresponding signal handler is \c onBarsetsRemoved.
*/

/*!
    \qmlmethod BarSet AbstractBarSeries::at(int index)
    Returns the bar set at \a index. Returns null if the index is not valid.
*/

/*!
    \qmlmethod BarSet AbstractBarSeries::append(string label, VariantList values)
    Adds a new bar set with \a label and \a values to the index. \a values is
    a list of real values.

    For example:
    \code
        myBarSeries.append("set 1", [0, 0.2, 0.2, 0.5, 0.4, 1.5, 0.9]);
    \endcode
*/

/*!
    \qmlmethod BarSet AbstractBarSeries::insert(int index, string label, VariantList values)
    Adds a new bar set with \a label and \a values to \a index. \a values can be a list
    of real values or a list of XYPoint types.

    If the index value is equal to or less than zero, the new bar set is prepended to the bar
    series. If the index value is equal to or greater than the number of bar sets in the bar
    series, the new bar set is appended to the bar series.

    \sa append()
*/

/*!
    \qmlmethod bool AbstractBarSeries::remove(BarSet barset)
    Removes the bar set specified by \a barset from the series. Returns \c true if successful,
    \c false otherwise.
*/

/*!
    \qmlmethod AbstractBarSeries::clear()
    Removes all bar sets from the series.
*/

/*!
    Removes the abstract bar series and the bar sets owned by it.
*/
QAbstractBarSeries::~QAbstractBarSeries()
{

}

/*!
    \internal
*/
QAbstractBarSeries::QAbstractBarSeries(QAbstractBarSeriesPrivate &o, QObject *parent)
    : QAbstractSeries(o, parent)
{
    Q_D(QAbstractSeries);
    QObject::connect(this, SIGNAL(countChanged()), d, SIGNAL(countChanged()));
}

/*!
    Sets the width of the bars of the series to \a width.
*/
void QAbstractBarSeries::setBarWidth(qreal width)
{
    Q_D(QAbstractBarSeries);
    d->setBarWidth(width);
}

/*!
    Returns the width of the bars of the series.
    \sa setBarWidth()
*/
qreal QAbstractBarSeries::barWidth() const
{
    Q_D(const QAbstractBarSeries);
    return d->barWidth();
}

/*!
    Adds a set of bars specified by \a set to the bar series and takes ownership of it. If the set
    is null or it already belongs to the series, it will not be appended.
    Returns \c true if appending succeeded.
*/
bool QAbstractBarSeries::append(QBarSet *set)
{
    Q_D(QAbstractBarSeries);
    bool success = d->append(set);
    if (success) {
        QList<QBarSet *> sets;
        sets.append(set);
        set->setParent(this);
        emit barsetsAdded(sets);
        emit countChanged();
    }
    return success;
}

/*!
    Removes the bar set specified by \a set from the series and permanently deletes it if
    the removal succeeds. Returns \c true if the set was removed.
*/
bool QAbstractBarSeries::remove(QBarSet *set)
{
    Q_D(QAbstractBarSeries);
    bool success = d->remove(set);
    if (success) {
        QList<QBarSet *> sets;
        sets.append(set);
        set->setParent(0);
        emit barsetsRemoved(sets);
        emit countChanged();
        delete set;
        set = 0;
    }
    return success;
}

/*!
    Takes a single \a set from the series. Does not delete the bar set object.
    \note The series remains the barset's parent object. You must set the
    parent object to take full ownership.

    Returns \c true if the take operation succeeds.
*/
bool QAbstractBarSeries::take(QBarSet *set)
{
    Q_D(QAbstractBarSeries);
    bool success = d->remove(set);
    if (success) {
        QList<QBarSet *> sets;
        sets.append(set);
        emit barsetsRemoved(sets);
        emit countChanged();
    }
    return success;
}

/*!
    Adds a list of bar sets specified by \a sets to a bar series and takes ownership of the sets.
    Returns \c true if all sets were appended successfully. If any of the sets is null or was
    previously appended to the series, nothing is appended and this function returns \c false.
    If any of the sets appears in the list more than once, nothing is appended and this function
    returns \c false.
*/
bool QAbstractBarSeries::append(QList<QBarSet *> sets)
{
    Q_D(QAbstractBarSeries);
    bool success = d->append(sets);
    if (success) {
        foreach (QBarSet *set, sets)
            set->setParent(this);
        emit barsetsAdded(sets);
        emit countChanged();
    }
    return success;
}

/*!
    Inserts a bar set specified by \a set to a series at the position specified by \a index
    and takes ownership of the set. If the set is null or already belongs to the series, it will
    not be appended. Returns \c true if inserting succeeds.
*/
bool QAbstractBarSeries::insert(int index, QBarSet *set)
{
    Q_D(QAbstractBarSeries);
    bool success = d->insert(index, set);
    if (success) {
        QList<QBarSet *> sets;
        sets.append(set);
        emit barsetsAdded(sets);
        emit countChanged();
    }
    return success;
}

/*!
    Removes all bar sets from the series and permanently deletes them.
*/
void QAbstractBarSeries::clear()
{
    Q_D(QAbstractBarSeries);
    QList<QBarSet *> sets = barSets();
    bool success = d->remove(sets);
    if (success) {
        emit barsetsRemoved(sets);
        emit countChanged();
        foreach (QBarSet *set, sets)
            delete set;
    }
}

/*!
    Returns the number of bar sets in a bar series.
*/
int QAbstractBarSeries::count() const
{
    Q_D(const QAbstractBarSeries);
    return d->m_barSets.count();
}

/*!
    Returns a list of bar sets in a bar series. Keeps the ownership of the bar sets.
 */
QList<QBarSet *> QAbstractBarSeries::barSets() const
{
    Q_D(const QAbstractBarSeries);
    return d->m_barSets;
}

/*!
    Sets the visibility of labels in a bar series to \a visible.
*/
void QAbstractBarSeries::setLabelsVisible(bool visible)
{
    Q_D(QAbstractBarSeries);
    if (d->m_labelsVisible != visible) {
        d->setLabelsVisible(visible);
        emit labelsVisibleChanged();
    }
}

/*!
    Returns the visibility of labels.
*/
bool QAbstractBarSeries::isLabelsVisible() const
{
    Q_D(const QAbstractBarSeries);
    return d->m_labelsVisible;
}

void QAbstractBarSeries::setLabelsFormat(const QString &format)
{
    Q_D(QAbstractBarSeries);
    if (d->m_labelsFormat != format) {
        d->m_labelsFormat = format;
        d->setLabelsDirty(true);
        emit labelsFormatChanged(format);
    }
}

QString QAbstractBarSeries::labelsFormat() const
{
    Q_D(const QAbstractBarSeries);
    return d->m_labelsFormat;
}

void QAbstractBarSeries::setLabelsAngle(qreal angle)
{
    Q_D(QAbstractBarSeries);
    if (d->m_labelsAngle != angle) {
        d->m_labelsAngle = angle;
        d->setLabelsDirty(true);
        emit labelsAngleChanged(angle);
    }
}

qreal QAbstractBarSeries::labelsAngle() const
{
    Q_D(const QAbstractBarSeries);
    return d->m_labelsAngle;
}

void QAbstractBarSeries::setLabelsPosition(QAbstractBarSeries::LabelsPosition position)
{
    Q_D(QAbstractBarSeries);
    if (d->m_labelsPosition != position) {
        d->m_labelsPosition = position;
        emit labelsPositionChanged(position);
    }
}

QAbstractBarSeries::LabelsPosition QAbstractBarSeries::labelsPosition() const
{
    Q_D(const QAbstractBarSeries);
    return d->m_labelsPosition;
}

void QAbstractBarSeries::setLabelsPrecision(int precision)
{
    Q_D(QAbstractBarSeries);
    if (d->m_labelsPrecision != precision) {
        d->m_labelsPrecision = precision;
        d->setLabelsDirty(true);
        emit labelsPrecisionChanged(precision);
    }
}

int QAbstractBarSeries::labelsPrecision() const
{
    Q_D(const QAbstractBarSeries);
    return d->m_labelsPrecision;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 构造函数
QAbstractBarSeriesPrivate::QAbstractBarSeriesPrivate(QAbstractBarSeries *q) :
    QAbstractSeriesPrivate(q), // 基类相应构造函数
    m_barWidth(0.5),  // Default value is 50% of category width // 柱状宽度
    m_labelsVisible(false), // 标签是否可见
    m_visible(true), // 是否可见
    m_blockBarUpdate(false), // 是否阻塞柱状图更新
    m_labelsFormat(), // 标签格式
    m_labelsPosition(QAbstractBarSeries::LabelsCenter), // 标签位置
    m_labelsAngle(0), // 标签角度
    m_labelsPrecision(6), // 标签轻
    m_visualsDirty(true), // ???
    m_labelsDirty(true) // ???
{
}

// 获取最大的柱状图集合计数
int QAbstractBarSeriesPrivate::categoryCount() const
{
    // No categories defined. return count of longest set.
    int count = 0;
    for (int i = 0; i < m_barSets.count(); i++) { // 遍历柱状集合
        if (m_barSets.at(i)->count() > count)
            count = m_barSets.at(i)->count(); // 获取最大的柱状图集合计数
    }

    return count; // 返回计数
}

// 设置柱状宽度
void QAbstractBarSeriesPrivate::setBarWidth(qreal width)
{
    // 确保宽度合法
    if (width < 0.0)
        width = 0.0;
    // 设置柱图宽度
    m_barWidth = width;
    // 发送更新布局信号
    emit updatedLayout();
}

// 获取柱状宽度
qreal QAbstractBarSeriesPrivate::barWidth() const
{
    return m_barWidth;
}

// 获取指定柱状集合指定内容
QBarSet *QAbstractBarSeriesPrivate::barsetAt(int index)
{
    return m_barSets.at(index);
}

// 设置是否可见
void QAbstractBarSeriesPrivate::setVisible(bool visible)
{
    // 设置相应标记
    m_visible = visible;
    // 发送可见变更信号
    emit visibleChanged();
}

// 设置标签是否可视
void QAbstractBarSeriesPrivate::setLabelsVisible(bool visible)
{
    // 设置相应标记
    m_labelsVisible = visible;
    // 发送标签可视变更信号
    emit labelsVisibleChanged(visible);
}

// 最小值
qreal QAbstractBarSeriesPrivate::min()
{
    // 如果柱状数据集为空
    if (m_barSets.count() <= 0)
        return 0;

    // 获取柱状图数据集中的最小值
    qreal min = INT_MAX;
    for (int i = 0; i < m_barSets.count(); i++) {
        int categoryCount = m_barSets.at(i)->count();
        for (int j = 0; j < categoryCount; j++) {
            qreal temp = m_barSets.at(i)->at(j);
            if (temp < min)
                min = temp;
        }
    }

    // 返回最小值
    return min;
}

// 最大值
qreal QAbstractBarSeriesPrivate::max()
{
    // 如果柱状图数据集为空
    if (m_barSets.count() <= 0)
        return 0;

    // 获取柱状图数据集中的最大值
    qreal max = INT_MIN;
    for (int i = 0; i < m_barSets.count(); i++) {
        int categoryCount = m_barSets.at(i)->count();
        for (int j = 0; j < categoryCount; j++) {
            qreal temp = m_barSets.at(i)->at(j);
            if (temp > max)
                max = temp;
        }
    }

    // 返回最大值
    return max;
}

// 获取指定的集合中的指定值
qreal QAbstractBarSeriesPrivate::valueAt(int set, int category)
{
    if ((set < 0) || (set >= m_barSets.count()))
        return 0; // No set, no value.
    else if ((category < 0) || (category >= m_barSets.at(set)->count()))
        return 0; // No category, no value.

    return m_barSets.at(set)->at(category);
}

// 获取集合中的百分比
qreal QAbstractBarSeriesPrivate::percentageAt(int set, int category)
{
    if ((set < 0) || (set >= m_barSets.count()))
        return 0; // No set, no value.
    else if ((category < 0) || (category >= m_barSets.at(set)->count()))
        return 0; // No category, no value.

    qreal value = m_barSets.at(set)->at(category);
    qreal sum = categorySum(category);
    if (qFuzzyCompare(sum, 0))
        return 0;

    return value / sum;
}

// 记录指定目录的和
qreal QAbstractBarSeriesPrivate::categorySum(int category)
{
    qreal sum(0);
    int count = m_barSets.count(); // Count sets
    for (int set = 0; set < count; set++) {
        if (category < m_barSets.at(set)->count())
            sum += m_barSets.at(set)->at(category);
    }
    return sum;
}

// 计算指定目录的绝对值和
qreal QAbstractBarSeriesPrivate::absoluteCategorySum(int category)
{
    qreal sum(0);
    int count = m_barSets.count(); // Count sets
    for (int set = 0; set < count; set++) {
        if (category < m_barSets.at(set)->count())
            sum += qAbs(m_barSets.at(set)->at(category));
    }
    return sum;
}

// 最大目录和
qreal QAbstractBarSeriesPrivate::maxCategorySum()
{
    qreal max = INT_MIN;
    int count = categoryCount();
    for (int i = 0; i < count; i++) {
        qreal sum = categorySum(i);
        if (sum > max)
            max = sum;
    }
    return max;
}

// 最小x
qreal QAbstractBarSeriesPrivate::minX()
{
    if (m_barSets.count() <= 0)
        return 0;

    qreal min = INT_MAX;

    for (int i = 0; i < m_barSets.count(); i++) {
        int categoryCount = m_barSets.at(i)->count();
        for (int j = 0; j < categoryCount; j++) {
            qreal temp = m_barSets.at(i)->d_ptr.data()->m_values.at(j).x();
            if (temp < min)
                min = temp;
        }
    }
    return min;
}

// 最大x
qreal QAbstractBarSeriesPrivate::maxX()
{
    if (m_barSets.count() <= 0)
        return 0;

    qreal max = INT_MIN;

    for (int i = 0; i < m_barSets.count(); i++) {
        int categoryCount = m_barSets.at(i)->count();
        for (int j = 0; j < categoryCount; j++) {
            qreal temp = m_barSets.at(i)->d_ptr.data()->m_values.at(j).x();
            if (temp > max)
                max = temp;
        }
    }

    return max;
}

// 目录顶部（所有正数的和）
qreal QAbstractBarSeriesPrivate::categoryTop(int category)
{
    // Returns top (sum of all positive values) of category.
    // Returns 0, if all values are negative
    qreal top(0);
    int count = m_barSets.count();
    for (int set = 0; set < count; set++) {
        if (category < m_barSets.at(set)->count()) {
            qreal temp = m_barSets.at(set)->at(category);
            if (temp > 0) {
                top += temp;
            }
        }
    }
    return top;
}

// 目录底部（所有负数的和）
qreal QAbstractBarSeriesPrivate::categoryBottom(int category)
{
    // Returns bottom (sum of all negative values) of category
    // Returns 0, if all values are positive
    qreal bottom(0);
    int count = m_barSets.count();
    for (int set = 0; set < count; set++) {
        if (category < m_barSets.at(set)->count()) {
            qreal temp = m_barSets.at(set)->at(category);
            if (temp < 0) {
                bottom += temp;
            }
        }
    }
    return bottom;
}

// 获取全部顶部的和
qreal QAbstractBarSeriesPrivate::top()
{
    // Returns top of all categories
    qreal top(0);
    int count = categoryCount();
    for (int i = 0; i < count; i++) {
        qreal temp = categoryTop(i);
        if (temp > top)
            top = temp;
    }
    return top;
}

// 获取全部底部的和
qreal QAbstractBarSeriesPrivate::bottom()
{
    // Returns bottom of all categories
    qreal bottom(0);
    int count = categoryCount();
    for (int i = 0; i < count; i++) {
        qreal temp = categoryBottom(i);
        if (temp < bottom)
            bottom = temp;
    }
    return bottom;
}

// 阻止柱状图更新
bool QAbstractBarSeriesPrivate::blockBarUpdate()
{
    return m_blockBarUpdate;
}

// 标签角度
qreal QAbstractBarSeriesPrivate::labelsAngle() const
{
    return m_labelsAngle;
}

// 初始化区域
void QAbstractBarSeriesPrivate::initializeDomain()
{
    // 初始化最大、最小x、y
    qreal minX(domain()->minX());
    qreal minY(domain()->minY());
    qreal maxX(domain()->maxX());
    qreal maxY(domain()->maxY());

    // 记录序列中最小、最大x
    qreal seriesMinX = this->minX();
    qreal seriesMaxX = this->maxX();

    // 获取最大y
    qreal y = max();

    // 计算最小、最大x、y
    minX = qMin(minX, seriesMinX - (qreal)0.5);
    minY = qMin(minY, y);
    maxX = qMax(maxX, seriesMaxX + (qreal)0.5);
    maxY = qMax(maxY, y);

    // 设置范围
    domain()->setRange(minX, maxX, minY, maxY);
}

// 创建图例标记
QList<QLegendMarker*> QAbstractBarSeriesPrivate::createLegendMarkers(QLegend* legend)
{
    // 启用所属序列
    Q_Q(QAbstractBarSeries);

    // 图列标记
    QList<QLegendMarker*> markers;

    // 创建、存储图例
    foreach(QBarSet* set, q->barSets()) {
        QBarLegendMarker* marker = new QBarLegendMarker(q,set,legend);
        markers << marker;
    }

    // 返回图例标记
    return markers;
}

// 追加图例序列
bool QAbstractBarSeriesPrivate::append(QBarSet *set)
{

    // 如果柱状图包含数据集，或数据集为空
    if ((m_barSets.contains(set)) || (set == 0))
        return false; // Fail if set is already in list or set is null.

    // 追加数据集
    m_barSets.append(set);

    // 捆绑信号
    QObject::connect(set->d_ptr.data(), &QBarSetPrivate::updatedBars,
                     this, &QAbstractBarSeriesPrivate::updatedBars);
    QObject::connect(set->d_ptr.data(), &QBarSetPrivate::valueChanged,
                     this, &QAbstractBarSeriesPrivate::handleSetValueChange);
    QObject::connect(set->d_ptr.data(), &QBarSetPrivate::valueAdded,
                     this, &QAbstractBarSeriesPrivate::handleSetValueAdd);
    QObject::connect(set->d_ptr.data(), &QBarSetPrivate::valueRemoved,
                     this, &QAbstractBarSeriesPrivate::handleSetValueRemove);

    // 发送信号
    emit restructuredBars(); // this notifies barchartitem

    // 返回结果
    return true;
}

// 删除
bool QAbstractBarSeriesPrivate::remove(QBarSet *set)
{
    // 如果不包含序列
    if (!m_barSets.contains(set))
        return false; // Fail if set is not in list

    // 删除指定序列
    m_barSets.removeOne(set);

    // 断开信号、槽
    QObject::disconnect(set->d_ptr.data(), &QBarSetPrivate::updatedBars,
                        this, &QAbstractBarSeriesPrivate::updatedBars);
    QObject::disconnect(set->d_ptr.data(), &QBarSetPrivate::valueChanged,
                        this, &QAbstractBarSeriesPrivate::handleSetValueChange);
    QObject::disconnect(set->d_ptr.data(), &QBarSetPrivate::valueAdded,
                        this, &QAbstractBarSeriesPrivate::handleSetValueAdd);
    QObject::disconnect(set->d_ptr.data(), &QBarSetPrivate::valueRemoved,
                        this, &QAbstractBarSeriesPrivate::handleSetValueRemove);

    // 发送信号
    emit restructuredBars(); // this notifies barchartitem

    // 返回
    return true;
}

// 追加序列
bool QAbstractBarSeriesPrivate::append(QList<QBarSet * > sets)
{
    // 遍历集合列表中的集合
    foreach (QBarSet *set, sets) {
        if ((set == 0) || (m_barSets.contains(set)))
            return false; // Fail if any of the sets is null or is already appended.
        if (sets.count(set) != 1)
            return false; // Also fail if same set is more than once in given list.
    }

    // 遍历集合绑定信号
    foreach (QBarSet *set, sets) {
        m_barSets.append(set);
        QObject::connect(set->d_ptr.data(), &QBarSetPrivate::updatedBars,
                         this, &QAbstractBarSeriesPrivate::updatedBars);
        QObject::connect(set->d_ptr.data(), &QBarSetPrivate::valueChanged,
                         this, &QAbstractBarSeriesPrivate::handleSetValueChange);
        QObject::connect(set->d_ptr.data(), &QBarSetPrivate::valueAdded,
                         this, &QAbstractBarSeriesPrivate::handleSetValueAdd);
        QObject::connect(set->d_ptr.data(), &QBarSetPrivate::valueRemoved,
                         this, &QAbstractBarSeriesPrivate::handleSetValueRemove);
    }

    // 发送信号
    emit restructuredBars(); // this notifies barchartitem

    // 返回
    return true;
}

// 移除集合
bool QAbstractBarSeriesPrivate::remove(QList<QBarSet * > sets)
{
    // 集合为空
    if (sets.count() == 0)
        return false;

    // 遍历集合
    foreach (QBarSet *set, sets) {
        if ((set == 0) || (!m_barSets.contains(set)))
            return false; // Fail if any of the sets is null or is not in series
        if (sets.count(set) != 1)
            return false; // Also fail if same set is more than once in given list.
    }

    // 遍历集合
    foreach (QBarSet *set, sets) {
        // 移除
        m_barSets.removeOne(set);
        // 断开信号槽
        QObject::disconnect(set->d_ptr.data(), &QBarSetPrivate::updatedBars,
                            this, &QAbstractBarSeriesPrivate::updatedBars);
        QObject::disconnect(set->d_ptr.data(), &QBarSetPrivate::valueChanged,
                            this, &QAbstractBarSeriesPrivate::handleSetValueChange);
        QObject::disconnect(set->d_ptr.data(), &QBarSetPrivate::valueAdded,
                            this, &QAbstractBarSeriesPrivate::handleSetValueAdd);
        QObject::disconnect(set->d_ptr.data(), &QBarSetPrivate::valueRemoved,
                            this, &QAbstractBarSeriesPrivate::handleSetValueRemove);
    }

    // 发送信号
    emit restructuredBars();        // this notifies barchartitem

    // 返回
    return true;
}

// 插入集合
bool QAbstractBarSeriesPrivate::insert(int index, QBarSet *set)
{
    // 如果包含集合或集合为0
    if ((m_barSets.contains(set)) || (set == 0))
        return false; // Fail if set is already in list or set is null.

    // 插入集合
    m_barSets.insert(index, set);

    // 连接集合
    QObject::connect(set->d_ptr.data(), &QBarSetPrivate::updatedBars,
                     this, &QAbstractBarSeriesPrivate::updatedBars);
    QObject::connect(set->d_ptr.data(), &QBarSetPrivate::valueChanged,
                     this, &QAbstractBarSeriesPrivate::handleSetValueChange);
    QObject::connect(set->d_ptr.data(), &QBarSetPrivate::valueAdded,
                     this, &QAbstractBarSeriesPrivate::handleSetValueAdd);
    QObject::connect(set->d_ptr.data(), &QBarSetPrivate::valueRemoved,
                     this, &QAbstractBarSeriesPrivate::handleSetValueRemove);

    // 发送信号
    emit restructuredBars();      // this notifies barchartitem

    // 返回
    return true;
}

// 初始化坐标轴
void QAbstractBarSeriesPrivate::initializeAxes()
{
    // 启用所属序列
    Q_Q(QAbstractBarSeries);

    // 遍历坐标轴
    foreach(QAbstractAxis* axis, m_axes) {
        // 坐标轴类型
        if (axis->type() == QAbstractAxis::AxisTypeBarCategory) {
            // 所属序列类型
            switch (q->type()) {
            // 序列类型
            case QAbstractSeries::SeriesTypeHorizontalBar:
            case QAbstractSeries::SeriesTypeHorizontalPercentBar:
            case QAbstractSeries::SeriesTypeHorizontalStackedBar:
                if (axis->orientation() == Qt::Vertical)
                populateCategories(qobject_cast<QBarCategoryAxis *>(axis));
            break;
            // 序列类型
            case QAbstractSeries::SeriesTypeBar:
            case QAbstractSeries::SeriesTypePercentBar:
            case QAbstractSeries::SeriesTypeStackedBar:
            case QAbstractSeries::SeriesTypeBoxPlot:
            case QAbstractSeries::SeriesTypeCandlestick:
                if (axis->orientation() == Qt::Horizontal)
                    populateCategories(qobject_cast<QBarCategoryAxis *>(axis));
            break;
            // 默认的
            default:
                qWarning() << "Unexpected series type";
            // 跳出
            break;
            }
        }
    }

    // 重置动画
    // Make sure series animations are reset when axes change
    AbstractBarChartItem *item = qobject_cast<AbstractBarChartItem *>(m_item.data());
    if (item)
        item->resetAnimation();
}

// 默认的轴类型
QAbstractAxis::AxisType QAbstractBarSeriesPrivate::defaultAxisType(Qt::Orientation orientation) const
{
    // 启用所属序列
    Q_Q(const QAbstractBarSeries);

    // 根据所属序列类型，返回轴类型
    switch (q->type()) {
    case QAbstractSeries::SeriesTypeHorizontalBar:
    case QAbstractSeries::SeriesTypeHorizontalPercentBar:
    case QAbstractSeries::SeriesTypeHorizontalStackedBar:
        if (orientation == Qt::Vertical)
            return QAbstractAxis::AxisTypeBarCategory;
        break;
    case QAbstractSeries::SeriesTypeBar:
    case QAbstractSeries::SeriesTypePercentBar:
    case QAbstractSeries::SeriesTypeStackedBar:
    case QAbstractSeries::SeriesTypeBoxPlot:
    case QAbstractSeries::SeriesTypeCandlestick:
        if (orientation == Qt::Horizontal)
            return QAbstractAxis::AxisTypeBarCategory;
        break;
    default:
        qWarning() << "Unexpected series type";
        break;
    }
    return QAbstractAxis::AxisTypeValue;

}

// 设置值范围信号响应槽
void QAbstractBarSeriesPrivate::handleSetValueChange(int index)
{
    QBarSetPrivate *priv = qobject_cast<QBarSetPrivate *>(sender());
    if (priv)
        emit setValueChanged(index, priv->q_ptr);
}

// 设置值添加信号响应槽
void QAbstractBarSeriesPrivate::handleSetValueAdd(int index, int count)
{
    QBarSetPrivate *priv = qobject_cast<QBarSetPrivate *>(sender());
    if (priv)
        emit setValueAdded(index, count, priv->q_ptr);
}

// 设置值删除信号响应槽
void QAbstractBarSeriesPrivate::handleSetValueRemove(int index, int count)
{
    QBarSetPrivate *priv = qobject_cast<QBarSetPrivate *>(sender());
    if (priv)
        emit setValueRemoved(index, count, priv->q_ptr);
}

// 为指定轴创建目录
void QAbstractBarSeriesPrivate::populateCategories(QBarCategoryAxis *axis)
{
    QStringList categories;
    if (axis->categories().isEmpty()) {
        for (int i(1); i < categoryCount() + 1; i++)
            categories << presenter()->numberToString(i);
        axis->append(categories);
    }
}

// 创建默认轴
QAbstractAxis* QAbstractBarSeriesPrivate::createDefaultAxis(Qt::Orientation orientation) const
{
    if (defaultAxisType(orientation) == QAbstractAxis::AxisTypeBarCategory)
        return new QBarCategoryAxis;
    else
        return new QValueAxis;
}

// 初始化主题
void QAbstractBarSeriesPrivate::initializeTheme(int index, ChartTheme* theme, bool forced)
{
    // 阻止更新
    m_blockBarUpdate = true; // Ensures that the bars are not updated before the theme is ready

    // 获取主题中的序列梯度
    const QList<QGradient> gradients = theme->seriesGradients();

    // Since each bar series uses different number of colors, we need to account for other
    // bar series in the chart that are also themed when choosing colors.
    // First series count is used to determine the color stepping to keep old applications
    // with single bar series with a lot of sets colored as they always have been.
    int actualIndex = 0;
    int firstSeriesSetCount = m_barSets.count();
    if (!m_item.isNull()) {
        auto seriesMap = m_item->themeManager()->seriesMap();
        int lowestSeries = index;
        for (auto it = seriesMap.cbegin(), end = seriesMap.cend(); it != end; ++it) {
            if (it.value() != index) {
                auto barSeries = qobject_cast<QAbstractBarSeries *>(it.key());
                if (barSeries) {
                    actualIndex += barSeries->count();
                    if (it.value() < lowestSeries) {
                        firstSeriesSetCount = qMax(barSeries->count(), gradients.count());
                        lowestSeries = it.value();
                    }
                }
            }
        }
    }


    qreal takeAtPos = 0.5;
    qreal step = 0.2;
    if (firstSeriesSetCount > 1) {
        step = 1.0 / qreal(firstSeriesSetCount);
        if (firstSeriesSetCount % gradients.count())
            step *= gradients.count();
        else
            step *= (gradients.count() - 1);
        if (index > 0) {
            // Take necessary amount of initial steps
            int initialStepper = actualIndex;
            while (initialStepper > gradients.count()) {
                initialStepper -= gradients.count();
                takeAtPos += step;
                if (takeAtPos == 1.0)
                    takeAtPos += step;
                takeAtPos -= int(takeAtPos);
            }
        }
    }

    for (int i(0); i < m_barSets.count(); i++) {
        int colorIndex = (actualIndex + i) % gradients.count();
        if ((actualIndex + i) > 0 && (actualIndex + i) % gradients.count() == 0) {
            // There is no dedicated base color for each sets, generate more colors
            takeAtPos += step;
            if (takeAtPos == 1.0)
            takeAtPos += step;
            takeAtPos -= (int) takeAtPos;
        }
        if (forced || QChartPrivate::defaultBrush() == m_barSets.at(i)->d_ptr->m_brush)
            m_barSets.at(i)->setBrush(ChartThemeManager::colorAt(gradients.at(colorIndex), takeAtPos));

        // Pick label color from the opposite end of the gradient.
        // 0.3 as a boundary seems to work well.
        if (forced || QChartPrivate::defaultBrush() == m_barSets.at(i)->d_ptr->m_labelBrush) {
            if (takeAtPos < 0.3)
                m_barSets.at(i)->setLabelBrush(ChartThemeManager::colorAt(gradients.at(actualIndex % gradients.size()), 1));
            else
                m_barSets.at(i)->setLabelBrush(ChartThemeManager::colorAt(gradients.at(actualIndex % gradients.size()), 0));
        }
        if (forced || QChartPrivate::defaultPen() == m_barSets.at(i)->d_ptr->m_pen) {
            QColor c = ChartThemeManager::colorAt(gradients.at(actualIndex % gradients.size()), 0.0);
            m_barSets.at(i)->setPen(c);
        }
    }
    m_blockBarUpdate = false;
    emit updatedBars();
}

// 初始化动画
void QAbstractBarSeriesPrivate::initializeAnimations(QChart::AnimationOptions options, int duration,
                                                     QEasingCurve &curve)
{
    AbstractBarChartItem *bar = static_cast<AbstractBarChartItem *>(m_item.data());
    Q_ASSERT(bar);
    if (bar->animation())
        bar->animation()->stopAndDestroyLater();

    if (options.testFlag(QChart::SeriesAnimations))
        bar->setAnimation(new BarAnimation(bar, duration, curve));
    else
        bar->setAnimation(0);
    QAbstractSeriesPrivate::initializeAnimations(options, duration, curve);
}

#include "moc_qabstractbarseries.cpp"
#include "moc_qabstractbarseries_p.cpp"

QT_CHARTS_END_NAMESPACE
