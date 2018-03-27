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

#include <QtCharts/QBarSet>
#include <private/qbarset_p.h>
#include <private/charthelpers_p.h>
#include <private/qchart_p.h>

QT_CHARTS_BEGIN_NAMESPACE

/*!
    \class QBarSet
    \inmodule Qt Charts
    \brief The QBarSet class represents one set of bars in a bar chart.

    A bar set contains one data value for each category. The first value of a set is assumed to
    belong to the first category, the second one to the second category, and so on. If the set has
    fewer values than there are categories, the missing values are assumed to be located at the end
    of the set. For missing values in the middle of a set, the numerical value of zero is used.
    Labels for zero value sets are not shown.

    \sa QAbstractBarSeries, QBarSeries, QStackedBarSeries, QPercentBarSeries
    \sa QHorizontalBarSeries, QHorizontalStackedBarSeries, QHorizontalPercentBarSeries
*/
/*!
    \qmltype BarSet
    \instantiates QBarSet
    \inqmlmodule QtCharts

    \brief Represents one set of bars in a bar chart.

    A bar set contains one data value for each category. The first value of a set is assumed to
    belong to the first category, the second one to the second category, and so on. If the set has
    fewer values than there are categories, the missing values are assumed to be located at the end
    of the set. For missing values in the middle of a set, the numerical value of zero is used.
    Labels for zero value sets are not shown.

    \sa AbstractBarSeries, BarSeries, StackedBarSeries, PercentBarSeries
    \sa HorizontalBarSeries, HorizontalStackedBarSeries, HorizontalPercentBarSeries

*/

/*!
    \property QBarSet::label
    \brief The label of the bar set.
*/
/*!
    \qmlproperty string BarSet::label
    The label of the bar set.
*/

/*!
    \property QBarSet::pen
    \brief The pen used to draw the lines of bars in the bar set.
*/

/*!
    \property QBarSet::brush
    \brief The brush used to fill the bars in the bar set.
*/

/*!
    \qmlproperty string BarSet::brushFilename
    The name of the file used as a brush for the set.
*/

/*!
    \property QBarSet::labelBrush
    \brief The brush used to draw the bar set's label.
*/

/*!
    \property QBarSet::labelFont
    \brief The font used to draw the bar set's label.
*/

/*!
    \qmlproperty font BarSet::labelFont
    The font used to draw the bar set's label.

    For more information, see \l [QML]{font}.
*/

/*!
    \property QBarSet::color
    \brief The fill (brush) color of the bar set.
*/
/*!
    \qmlproperty color BarSet::color
    The fill (brush) color of the bar set.
*/

/*!
    \property QBarSet::borderColor
    \brief The line (pen) color of the bar set.
*/
/*!
    \qmlproperty color BarSet::borderColor
    The line (pen) color of the bar set.
*/

/*!
    \qmlproperty real BarSet::borderWidth
    The width of the border line. By default, the width is 2.0.
*/

/*!
    \property QBarSet::labelColor
    \brief The text (label) color of the bar set.
*/
/*!
    \qmlproperty color BarSet::labelColor
    The text (label) color of the bar set.
*/

/*!
    \fn void QBarSet::clicked(int index)

    This signal is emitted when the user clicks the bar specified by \a index in a bar set.
*/

/*!
    \qmlmethod BarSet::clicked(int index)

    This signal is emitted when the user clicks the bar specified by \a index
    in a bar set.

    The corresponding signal handler is \c onClicked.
*/

/*!
    \fn void QBarSet::pressed(int index)

    This signal is emitted when the user clicks the bar specified by \a index in a bar set
    and holds down the mouse button.
*/

/*!
    \qmlmethod BarSet::pressed(int index)

    This signal is emitted when the user clicks the bar specified by \a index in
    a bar set and holds down the mouse button.

    The corresponding signal handler is \c onPressed.
*/

/*!
    \fn void QBarSet::released(int index)

    This signal is emitted when the user releases the mouse press on the bar specified by
    \a index in a bar set.
*/

/*!
    \qmlmethod BarSet::released(int index)

    This signal is emitted when the user releases the mouse press on the bar
    specified by \a index in a bar set.

    The corresponding signal handler is \c onReleased.
*/

/*!
    \fn void QBarSet::doubleClicked(int index)

    This signal is emitted when the user double-clicks the bar specified by \a index in a bar set.
*/

/*!
    \qmlmethod BarSet::doubleClicked(int index)

    This signal is emitted when the user double-clicks the bar specified by
    \a index in a bar set.

    The corresponding signal handler is \c onDoubleClicked.
*/

/*!
    \fn void QBarSet::hovered(bool status, int index)

    This signal is emitted when a mouse is hovered over the bar specified by \a index in a bar set.
    When the mouse moves over the bar, \a status turns \c true, and when the mouse moves away again,
    it turns \c false.
*/


/*!
    \fn void QBarSet::labelChanged()
    This signal is emitted when the label of the bar set changes.
    \sa label
*/

/*!
    \fn void QBarSet::penChanged()
    This signal is emitted when the pen used to draw the bar set changes.
    \sa pen
*/

/*!
    \fn void QBarSet::brushChanged()
    This signal is emitted when the brush used to draw the bar set changes.
    \sa brush
*/

/*!
    \fn void QBarSet::labelBrushChanged()
    This signal is emitted when the brush used to draw the bar set's label changes.
    \sa labelBrush
*/

/*!
    \fn void QBarSet::labelFontChanged()
    This signal is emitted when the font of the bar set's label changes.
    \sa labelBrush
*/

/*!
    \fn void QBarSet::colorChanged(QColor)
    This signal is emitted when the fill (brush) color of the bar set changes to \a color.
*/

/*!
    \fn void QBarSet::borderColorChanged(QColor)
    This signal is emitted when the line (pen) color of the bar set changes to \a color.
*/

/*!
    \fn void QBarSet::labelColorChanged(QColor)
    This signal is emitted when the text (label) color of the bar set changes to \a color.
*/

/*!
    \fn void QBarSet::valuesAdded(int index, int count)
    This signal is emitted when new values are added to the bar set.
    \a index indicates the position of the first inserted value, and \a count is the number
    of inserted values.
    \sa append(), insert()
*/
/*!
    \qmlsignal BarSet::valuesAdded(int index, int count)
    This signal is emitted when new values are added to the bar set.
    \a index indicates the position of the first inserted value, and \a count is the number
    of inserted values.

    The corresponding signal handler is \c onValuesAdded.
*/

/*!
    \fn void QBarSet::valuesRemoved(int index, int count)
    This signal is emitted when values are removed from the bar set.
    \a index indicates the position of the first removed value, and \a count is the number
    of removed values.
    \sa remove()
*/
/*!
    \qmlsignal BarSet::valuesRemoved(int index, int count)
    This signal is emitted when values are removed from the bar set.
    \a index indicates the position of the first removed value, and \a count is the number
    of removed values.

    The corresponding signal handler is \c onValuesRemoved.
*/

/*!
    \fn void QBarSet::valueChanged(int index)
    This signal is emitted when the value at the position specified by \a index is modified.
    \sa at()
*/
/*!
    \qmlsignal BarSet::valueChanged(int index)
    This signal is emitted when the value at the position specified by \a index is modified.

    The corresponding signal handler is \c onValueChanged.
*/

/*!
    \qmlproperty int BarSet::count
    The number of values in the bar set.
*/

/*!
    \qmlproperty QVariantList BarSet::values
    The values of the bar set. You can set a list of either \l [QML]{real} or \l [QML]{point}
    types as values.

    If you set a list of real types as values, they directly define the bar set values.

    If you set a list of point types as values, the x-coordinate of the point specifies its
    zero-based index in the bar set. The size of the bar set is the highest x-coordinate value + 1.
    If a point is missing for any x-coordinate between zero and the highest value,
    it gets the value zero.

    For example, the following bar sets have equal values:
    \code
        myBarSet1.values = [5, 0, 1, 5];
        myBarSet2.values = [Qt.point(0, 5), Qt.point(2, 1), Qt.point(3, 5)];
    \endcode
*/

/*!
    Constructs a bar set with the label \a label and the parent \a parent.
*/
// 构造函数
QBarSet::QBarSet(const QString label, QObject *parent)
    : QObject(parent)
    , d_ptr(new QBarSetPrivate(label, this))
{
}

/*!
    Removes the bar set.
*/
// 析构函数
QBarSet::~QBarSet()
{
    // NOTE: d_ptr destroyed by QObject
}

/*!
    Sets \a label as the new label for the bar set.
*/
// 设置标签
void QBarSet::setLabel(const QString label)
{
    // 设置标签
    d_ptr->m_label = label;
    // ???
    d_ptr->setLabelsDirty(true);
    // 发送标签变更信号
    emit labelChanged();
}

/*!
    Returns the label of the bar set.
*/
// 获取标签
QString QBarSet::label() const
{
    return d_ptr->m_label;
}

/*!
    \qmlmethod BarSet::append(real value)
    Appends the new value specified by \a value to the end of the bar set.
*/

/*!
    Appends the new value specified by \a value to the end of the bar set.
*/
// 追加值
void QBarSet::append(const qreal value)
{
    // Convert to QPointF
    // 记录计数
    int index = d_ptr->m_values.count();
    // 追加点
    d_ptr->append(QPointF(d_ptr->m_values.count(), value));
    // 发送增加值信号
    emit valuesAdded(index, 1);
}

/*!
    Appends the list of real values specified by \a values to the end of the bar set.

    \sa append()
*/
// 追加值集
void QBarSet::append(const QList<qreal> &values)
{
    // 获取值集合数量
    int index = d_ptr->m_values.count();
    // 追加值集合
    d_ptr->append(values);
    // 发送值添加信号
    emit valuesAdded(index, values.count());
}

/*!
    A convenience operator for appending the real value specified by \a value to the end of the
    bar set.

    \sa append()
*/
// 追加值
QBarSet &QBarSet::operator << (const qreal &value)
{
    append(value);
    return *this;
}

/*!
    Inserts \a value in the position specified by \a index.
    The values following the inserted value are moved up one position.

    \sa remove()
*/
// 插入值
void QBarSet::insert(const int index, const qreal value)
{
    d_ptr->insert(index, value);
    emit valuesAdded(index, 1);
}

/*!
    \qmlmethod BarSet::remove(int index, int count)
    Removes the number of values specified by \a count from the bar set starting
    with the value specified by \a index.

    If you leave out \a count, only the value specified by \a index is removed.
*/

/*!
    Removes the number of values specified by \a count from the bar set starting with
    the value specified by \a index.
    \sa insert()
*/
// 删除
void QBarSet::remove(const int index, const int count)
{
    int removedCount = d_ptr->remove(index, count);
    if (removedCount > 0)
        emit valuesRemoved(index, removedCount);
    return;
}

/*!
    \qmlmethod BarSet::replace(int index, real value)
    Adds the value specified by \a value to the bar set at the position
    specified by \a index.
*/

/*!
    Adds the value specified by \a value to the bar set at the position specified by \a index.
*/
// 替换
void QBarSet::replace(const int index, const qreal value)
{
    if (index >= 0 && index < d_ptr->m_values.count()) {
        d_ptr->replace(index, value);
        emit valueChanged(index);
    }
}

/*!
    \qmlmethod BarSet::at(int index)
    Returns the value specified by \a index from the bar set.
    If the index is out of bounds, 0.0 is returned.
*/

/*!
    Returns the value specified by \a index from the bar set.
    If the index is out of bounds, 0.0 is returned.
*/
// 获取指定值
qreal QBarSet::at(const int index) const
{
    if (index < 0 || index >= d_ptr->m_values.count())
        return 0;
    return d_ptr->m_values.at(index).y();
}

/*!
    Returns the value of the bar set specified by \a index.
    If the index is out of bounds, 0.0 is returned.
*/
// 获取指定值
qreal QBarSet::operator [](const int index) const
{
    return at(index);
}

/*!
    Returns the number of values in a bar set.
*/
// 值计数
int QBarSet::count() const
{
    return d_ptr->m_values.count();
}

/*!
    Returns the sum of all values in the bar set.
*/
// 计算值和
qreal QBarSet::sum() const
{
    qreal total(0);
    for (int i = 0; i < d_ptr->m_values.count(); i++)
        total += d_ptr->m_values.at(i).y();
    return total;
}

/*!
    Sets the pen used to draw the lines in the bar set to \a pen.
*/
// 设置画笔
void QBarSet::setPen(const QPen &pen)
{
    if (d_ptr->m_pen != pen) {
        d_ptr->m_pen = pen;
        d_ptr->setVisualsDirty(true);
        emit d_ptr->updatedBars();
        emit penChanged();
    }
}

/*!
    Returns the pen used to draw the lines in the bar set.
*/
// 获取画笔
QPen QBarSet::pen() const
{
    if (d_ptr->m_pen == QChartPrivate::defaultPen())
        return QPen();
    else
        return d_ptr->m_pen;
}

/*!
    Sets the brush used to fill the bars in the bar set to \a brush.
*/
// 设置画刷
void QBarSet::setBrush(const QBrush &brush)
{
    if (d_ptr->m_brush != brush) {
        d_ptr->m_brush = brush;
        d_ptr->setVisualsDirty(true);
        emit d_ptr->updatedBars();
        emit brushChanged();
    }
}

/*!
    Returns the brush used to fill the bars in the bar set.
*/
// 获取画刷
QBrush QBarSet::brush() const
{
    if (d_ptr->m_brush == QChartPrivate::defaultBrush())
        return QBrush();
    else
        return d_ptr->m_brush;
}

/*!
    Sets the brush used to draw values on top of this bar set to \a brush.
*/
// 设置标签画刷
void QBarSet::setLabelBrush(const QBrush &brush)
{
    if (d_ptr->m_labelBrush != brush) {
        d_ptr->m_labelBrush = brush;
        d_ptr->setLabelsDirty(true);
        emit d_ptr->updatedBars();
        emit labelBrushChanged();
    }
}

/*!
    Returns the brush used to draw values on top of this bar set.
*/
// 获取标签画刷
QBrush QBarSet::labelBrush() const
{
    if (d_ptr->m_labelBrush == QChartPrivate::defaultBrush())
        return QBrush();
    else
        return d_ptr->m_labelBrush;
}

/*!
    Sets the font used to draw values on top of this bar set to \a font.
*/
// 设置标签字体
void QBarSet::setLabelFont(const QFont &font)
{
    if (d_ptr->m_labelFont != font) {
        d_ptr->m_labelFont = font;
        d_ptr->setLabelsDirty(true);
        emit d_ptr->updatedBars();
        emit labelFontChanged();
    }
}

/*!
    Returns the pen used to draw values on top of this bar set.
*/
// 获取标签字体
QFont QBarSet::labelFont() const
{
    return d_ptr->m_labelFont;
}

/*!
    Returns the fill color for the bar set.
*/
// 获取颜色
QColor QBarSet::color()
{
    return brush().color();
}

/*!
    Sets the fill color for the bar set to \a color.
*/
// 设置颜色
void QBarSet::setColor(QColor color)
{
    QBrush b = brush();
    if ((b.color() != color) || (b.style() == Qt::NoBrush)) {
        b.setColor(color);
        if (b.style() == Qt::NoBrush) {
            // Set tyle to Qt::SolidPattern. (Default is Qt::NoBrush)
            // This prevents theme to override color defined in QML side:
            // BarSet { label: "Bob"; color:"red"; values: [1,2,3] }
            // The color must be obeyed, since user wanted it.
            b.setStyle(Qt::SolidPattern);
        }
        setBrush(b);
        emit colorChanged(color);
    }
}

/*!
    Returns the line color for the bar set.
*/
// 获取边框颜色
QColor QBarSet::borderColor()
{
    return pen().color();
}

/*!
    Sets the line color for the bar set to \a color.
*/
// 设置边框颜色
void QBarSet::setBorderColor(QColor color)
{
    QPen p = pen();
    if (p.color() != color) {
        p.setColor(color);
        setPen(p);
        emit borderColorChanged(color);
    }
}

/*!
    Returns the text color for the bar set.
*/
// 获取标签颜色
QColor QBarSet::labelColor()
{
    return labelBrush().color();
}

/*!
    Sets the text color for the bar set to \a color.
*/
// 设置标签颜色
void QBarSet::setLabelColor(QColor color)
{
    QBrush b = labelBrush();
    if (b == QBrush())
        b.setStyle(Qt::SolidPattern);

    if (d_ptr->m_labelBrush.color() != color) {
        b.setColor(color);
        setLabelBrush(b);
        emit labelColorChanged(color);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 构造函数
QBarSetPrivate::QBarSetPrivate(const QString label, QBarSet *parent) : QObject(parent),
    q_ptr(parent), // 所属柱状图集合
    m_label(label), // 标签
    m_pen(QChartPrivate::defaultPen()), // 画笔
    m_brush(QChartPrivate::defaultBrush()), // 画刷
    m_labelBrush(QChartPrivate::defaultBrush()), // 标签画刷
    m_visualsDirty(true) // ???
{
}

// 析构函数
QBarSetPrivate::~QBarSetPrivate()
{
}

// 追加一个点
void QBarSetPrivate::append(QPointF value)
{
    if (isValidValue(value)) { // 有效值
        m_values.append(value); // 追加一个点
        emit valueAdded(m_values.size() - 1, 1); // 发送追加信号
    }
}

// 追加点集
void QBarSetPrivate::append(QList<QPointF> values)
{
    // 点集计数
    int originalIndex = m_values.count();
    // 遍历点集
    for (int i = 0; i < values.count(); i++) {
        if (isValidValue(values.at(i))) // 数据有效
            m_values.append(values.at(i)); // 追加点
    }
    // 发送信号
    emit valueAdded(originalIndex, values.size());
}

// 追加点集
void QBarSetPrivate::append(QList<qreal> values)
{
    // 点集计数
    int originalIndex = m_values.count();
    // 记录下标
    int index = originalIndex;
    // 遍历值集
    for (int i = 0; i < values.count(); i++) {
        if (isValidValue(values.at(i))) { // 值有效
            m_values.append(QPointF(index, values.at(i))); // 追加点集
            index++; // 下标自增
        }
    }
    // 发送信号
    emit valueAdded(originalIndex, values.size());
}

// 插入值
void QBarSetPrivate::insert(const int index, const qreal value)
{
    m_values.insert(index, QPointF(index, value));
    emit valueAdded(index, 1);
}

// 插入值
void QBarSetPrivate::insert(const int index, const QPointF value)
{
    m_values.insert(index, value);
    emit valueAdded(index, 1);
}

// 删除指定值
int QBarSetPrivate::remove(const int index, const int count)
{
    // 记录删除计数
    int removeCount = count;
    // 如果索引非法，值计数为空
    if ((index < 0) || (m_values.count() == 0))
        return 0; // Invalid index or not values in list, remove nothing.
    // 如果计数超出范围
    else if ((index + count) > m_values.count())
        removeCount = m_values.count() - index; // Trying to remove more items than list has. Limit amount to be removed.

    // 逐个删除
    int c = 0;
    while (c < removeCount) {
        m_values.removeAt(index);
        c++;
    }
    // 发送信号
    emit valueRemoved(index, removeCount);
    // 返回删除计数
    return removeCount;
}

// 替换指定值
void QBarSetPrivate::replace(const int index, const qreal value)
{
    // 替换指定索引指定值
    m_values.replace(index, QPointF(index, value));
    // 发送信号
    emit valueChanged(index);
}

// 获取指定索引的x位置
qreal QBarSetPrivate::pos(const int index)
{
    // 索引小于0或者索引大于等于值集计数
    if (index < 0 || index >= m_values.count())
        return 0;
    // 获取指定x位置
    return m_values.at(index).x();
}

// 获取指定值
qreal QBarSetPrivate::value(const int index)
{
    // 如果索引非法
    if (index < 0 || index >= m_values.count())
        return 0;
    // 获取指定y值
    return m_values.at(index).y();
}

#include "moc_qbarset.cpp"
#include "moc_qbarset_p.cpp"

QT_CHARTS_END_NAMESPACE
