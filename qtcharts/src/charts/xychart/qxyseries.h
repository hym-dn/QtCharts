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

#ifndef QXYSERIES_H
#define QXYSERIES_H

#include <QtCharts/QChartGlobal>
#include <QtCharts/QAbstractSeries>
#include <QtGui/QPen>
#include <QtGui/QBrush>

QT_BEGIN_NAMESPACE
class QModelIndex;
QT_END_NAMESPACE

QT_CHARTS_BEGIN_NAMESPACE

class QXYSeriesPrivate;
class QXYModelMapper;

// x、y序列
class QT_CHARTS_EXPORT QXYSeries : public QAbstractSeries
{
    Q_OBJECT
    Q_PROPERTY(bool pointsVisible READ pointsVisible WRITE setPointsVisible)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(QString pointLabelsFormat READ pointLabelsFormat WRITE setPointLabelsFormat NOTIFY pointLabelsFormatChanged)
    Q_PROPERTY(bool pointLabelsVisible READ pointLabelsVisible WRITE setPointLabelsVisible NOTIFY pointLabelsVisibilityChanged)
    Q_PROPERTY(QFont pointLabelsFont READ pointLabelsFont WRITE setPointLabelsFont NOTIFY pointLabelsFontChanged)
    Q_PROPERTY(QColor pointLabelsColor READ pointLabelsColor WRITE setPointLabelsColor NOTIFY pointLabelsColorChanged)
    Q_PROPERTY(bool pointLabelsClipping READ pointLabelsClipping WRITE setPointLabelsClipping NOTIFY pointLabelsClippingChanged)

protected:
    explicit QXYSeries(QXYSeriesPrivate &d, QObject *parent = Q_NULLPTR); // 构造

public:
    ~QXYSeries(); // 析构
    void append(qreal x, qreal y); // 追加点
    void append(const QPointF &point); // 追加点
    void append(const QList<QPointF> &points); // 追加点集
    void replace(qreal oldX, qreal oldY, qreal newX, qreal newY); // 替换点
    void replace(const QPointF &oldPoint, const QPointF &newPoint); // 替换点
    void replace(int index, qreal newX, qreal newY); // 替换点
    void replace(int index, const QPointF &newPoint); // 替换点
    void remove(qreal x, qreal y); // 删除点
    void remove(const QPointF &point); // 删除点
    void remove(int index); // 删除点
    void removePoints(int index, int count); // 删除点集
    void insert(int index, const QPointF &point); // 插入点
    void clear(); // 清除

    int count() const; // 计数
    QList<QPointF> points() const; // 获取点集
    QVector<QPointF> pointsVector() const; // 获取点集
    const QPointF &at(int index) const; // 获取点

    QXYSeries &operator << (const QPointF &point); // 插入点
    QXYSeries &operator << (const QList<QPointF> &points); // 插入点集

    virtual void setPen(const QPen &pen); // 设置画笔
    QPen pen() const; // 获取画笔

    virtual void setBrush(const QBrush &brush); // 设置画刷
    QBrush brush() const; // 返回画刷

    virtual void setColor(const QColor &color); // 设置颜色
    virtual QColor color() const; // 返回颜色

    void setPointsVisible(bool visible = true); // 设置点集是否可见
    bool pointsVisible() const; // 获取点集是否可见

    void setPointLabelsFormat(const QString &format); // 设置点集标签格式
    QString pointLabelsFormat() const; // 获取点集标签格式

    void setPointLabelsVisible(bool visible = true); // 设置点集标签是否可见
    bool pointLabelsVisible() const; // 获取点集标签是否可见

    void setPointLabelsFont(const QFont &font); // 设置点标签字体
    QFont pointLabelsFont() const; // 获取点标签字体

    void setPointLabelsColor(const QColor &color); // 设置点标签颜色
    QColor pointLabelsColor() const; // 获取点标签颜色

    void setPointLabelsClipping(bool enabled = true); // 设置点标签是否剪裁
    bool pointLabelsClipping() const; // 获取点标签是否剪裁

    void replace(QList<QPointF> points); // 替换点集
    void replace(QVector<QPointF> points); // 替换点集

Q_SIGNALS:
    void clicked(const QPointF &point);
    void hovered(const QPointF &point, bool state);
    void pressed(const QPointF &point);
    void released(const QPointF &point);
    void doubleClicked(const QPointF &point);
    void pointReplaced(int index);
    void pointRemoved(int index);
    void pointAdded(int index);
    void colorChanged(QColor color);
    void pointsReplaced();
    void pointLabelsFormatChanged(const QString &format);
    void pointLabelsVisibilityChanged(bool visible);
    void pointLabelsFontChanged(const QFont &font);
    void pointLabelsColorChanged(const QColor &color);
    void pointLabelsClippingChanged(bool clipping);
    void pointsRemoved(int index, int count);
    void penChanged(const QPen &pen);

private:
    Q_DECLARE_PRIVATE(QXYSeries)
    Q_DISABLE_COPY(QXYSeries)
    friend class QXYLegendMarkerPrivate;
    friend class XYLegendMarker;
    friend class XYChart;
};

QT_CHARTS_END_NAMESPACE

#endif // QXYSERIES_H
