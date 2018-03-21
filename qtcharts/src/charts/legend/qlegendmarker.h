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

#ifndef QLEGENDMARKER_H
#define QLEGENDMARKER_H

#include <QtCharts/QChartGlobal>
#include <QtCharts/QLegend>
#include <QtCore/QObject>
#include <QtGui/QPen>
#include <QtGui/QBrush>
#include <QtGui/QFont>

QT_CHARTS_BEGIN_NAMESPACE

class QLegendMarkerPrivate;
class QAbstractSeries;
class QLegend;

// 抽象描述图例上的一项标记
class QT_CHARTS_EXPORT QLegendMarker : public QObject
{
    Q_OBJECT

public:

    // 标记类型
    enum LegendMarkerType {
        LegendMarkerTypeArea,
        LegendMarkerTypeBar,
        LegendMarkerTypePie,
        LegendMarkerTypeXY,
        LegendMarkerTypeBoxPlot,
        LegendMarkerTypeCandlestick
    };

    Q_PROPERTY(QString label READ label WRITE setLabel NOTIFY labelChanged)
    Q_PROPERTY(QBrush labelBrush READ labelBrush WRITE setLabelBrush NOTIFY labelBrushChanged)
    Q_PROPERTY(QFont font READ font WRITE setFont NOTIFY fontChanged)
    Q_PROPERTY(QPen pen READ pen WRITE setPen NOTIFY penChanged)
    Q_PROPERTY(QBrush brush READ brush WRITE setBrush NOTIFY brushChanged)
    Q_PROPERTY(bool visible READ isVisible WRITE setVisible NOTIFY visibleChanged)
    Q_PROPERTY(QLegend::MarkerShape shape READ shape WRITE setShape NOTIFY shapeChanged)
    Q_ENUMS(LegendMarkerType)

public:
    virtual ~QLegendMarker(); // 析构函数
    virtual LegendMarkerType type() = 0; // 标记类型

    QString label() const; // 获取标签
    void setLabel(const QString &label); // 设置标签

    QBrush labelBrush() const; // 标签画刷
    void setLabelBrush(const QBrush &brush); // 设置标签画刷

    QFont font() const; // 字体
    void setFont(const QFont &font); // 设置字体

    QPen pen() const; // 画笔
    void setPen(const QPen &pen); // 设置画笔

    QBrush brush() const; // 画刷
    void setBrush(const QBrush &brush); // 设置画刷

    bool isVisible() const; // 是否可见
    void setVisible(bool visible); // 设置可见

    QLegend::MarkerShape shape() const; // 形状
    void setShape(QLegend::MarkerShape shape); // 设置形状

    virtual QAbstractSeries* series() = 0; // 获取所属序列

Q_SIGNALS:
    void clicked();
    void hovered(bool status);
    void labelChanged();
    void labelBrushChanged();
    void fontChanged();
    void penChanged();
    void brushChanged();
    void visibleChanged();
    void shapeChanged();

protected:
    explicit QLegendMarker(QLegendMarkerPrivate &d, QObject *parent = nullptr); // 构造

    QScopedPointer<QLegendMarkerPrivate> d_ptr; // 私有成员
    friend class QLegendPrivate;
    friend class QLegendMarkerPrivate;
    friend class LegendMarkerItem;
    friend class LegendLayout;
    friend class LegendScroller;

private:
    Q_DISABLE_COPY(QLegendMarker)
};

QT_CHARTS_END_NAMESPACE

#endif // QLEGENDMARKER_H
