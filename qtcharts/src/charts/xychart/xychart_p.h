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

#ifndef XYCHART_H
#define XYCHART_H

#include <QtCharts/QChartGlobal>
#include <private/chartitem_p.h>
#include <private/xyanimation_p.h>
#include <QtCharts/QValueAxis>
#include <QtCharts/private/qchartglobal_p.h>
#include <QtGui/QPen>

QT_CHARTS_BEGIN_NAMESPACE

class ChartPresenter;
class QXYSeries;

// x、y图表项
class QT_CHARTS_PRIVATE_EXPORT XYChart :  public ChartItem
{
    Q_OBJECT
public:
    explicit XYChart(QXYSeries *series,QGraphicsItem *item = 0); // 构造
    ~XYChart() {} // 析构

    void setGeometryPoints(const QVector<QPointF> &points); // 设置几何点集
    QVector<QPointF> geometryPoints() const { return m_points; } // 获取几何点集

    void setAnimation(XYAnimation *animation); // 设置动画
    ChartAnimation *animation() const { return m_animation; } // 获取动画
    virtual void updateGeometry() = 0; // 更新几何

    bool isDirty() const { return m_dirty; } // 是否为脏数据
    void setDirty(bool dirty); // 设置脏数据

    void getSeriesRanges(qreal &minX, qreal &maxX, qreal &minY, qreal &maxY); // 设置序列范围
    QVector<bool> offGridStatusVector(); // 获取点集的范围标记

public Q_SLOTS:
    void handlePointAdded(int index); // 点增加信号响应槽
    void handlePointRemoved(int index); // 点移除信号响应槽
    void handlePointsRemoved(int index, int count); // 点集移除信号响应槽
    void handlePointReplaced(int index); // 点替换信号响应槽
    void handlePointsReplaced(); // 点集替换信号响应槽
    void handleDomainUpdated(); // 区域更新信号响应槽

Q_SIGNALS:
    void clicked(const QPointF &point);
    void hovered(const QPointF &point, bool state);
    void pressed(const QPointF &point);
    void released(const QPointF &point);
    void doubleClicked(const QPointF &point);

protected:
    virtual void updateChart(QVector<QPointF> &oldPoints, QVector<QPointF> &newPoints, int index = -1); // 更新图表
    virtual void updateGlChart(); // 更新OpenGL图表
    virtual void refreshGlChart(); // 刷新OpenGL图表

private:
    inline bool isEmpty(); // 是否为空

protected:
    QXYSeries *m_series; // 所属序列
    QVector<QPointF> m_points; // 存储点
    XYAnimation *m_animation; // 动画
    bool m_dirty; // 是否为脏数据

    friend class AreaChartItem;
};

QT_CHARTS_END_NAMESPACE

#endif
