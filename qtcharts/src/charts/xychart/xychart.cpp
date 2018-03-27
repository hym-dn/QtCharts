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

#include <private/xychart_p.h>
#include <QtCharts/QXYSeries>
#include <private/qxyseries_p.h>
#include <private/chartpresenter_p.h>
#include <private/abstractdomain_p.h>
#include <private/chartdataset_p.h>
#include <private/glxyseriesdata_p.h>
#include <QtCharts/QXYModelMapper>
#include <private/qabstractaxis_p.h>
#include <QtGui/QPainter>
#include <QtCore/QAbstractItemModel>

QT_CHARTS_BEGIN_NAMESPACE

// 构造
XYChart::XYChart(QXYSeries *series, QGraphicsItem *item):
      ChartItem(series->d_func(),item), // 基类构造函数
      m_series(series), // 所属序列
      m_animation(0), // 动画
      m_dirty(true) // 是否脏数据
{
    // 连接信、槽
    QObject::connect(series, SIGNAL(pointReplaced(int)), this, SLOT(handlePointReplaced(int)));
    QObject::connect(series, SIGNAL(pointsReplaced()), this, SLOT(handlePointsReplaced()));
    QObject::connect(series, SIGNAL(pointAdded(int)), this, SLOT(handlePointAdded(int)));
    QObject::connect(series, SIGNAL(pointRemoved(int)), this, SLOT(handlePointRemoved(int)));
    QObject::connect(series, SIGNAL(pointsRemoved(int, int)), this, SLOT(handlePointsRemoved(int, int)));
    QObject::connect(this, SIGNAL(clicked(QPointF)), series, SIGNAL(clicked(QPointF)));
    QObject::connect(this, SIGNAL(hovered(QPointF,bool)), series, SIGNAL(hovered(QPointF,bool)));
    QObject::connect(this, SIGNAL(pressed(QPointF)), series, SIGNAL(pressed(QPointF)));
    QObject::connect(this, SIGNAL(released(QPointF)), series, SIGNAL(released(QPointF)));
    QObject::connect(this, SIGNAL(doubleClicked(QPointF)), series, SIGNAL(doubleClicked(QPointF)));
    QObject::connect(series, &QAbstractSeries::useOpenGLChanged,this, &XYChart::handleDomainUpdated);
}

// 设置几何点集
void XYChart::setGeometryPoints(const QVector<QPointF> &points)
{
    m_points = points;
}

// 设置动画
void XYChart::setAnimation(XYAnimation *animation)
{
    m_animation = animation;
}

// 设置脏数据
void XYChart::setDirty(bool dirty)
{
    m_dirty = dirty;
}

// Returns a vector with same size as geometryPoints vector, indicating
// the off grid status of points.
// 标记范围外的点
QVector<bool> XYChart::offGridStatusVector()
{
    // 获取区域最小、最大x、y
    qreal minX = domain()->minX();
    qreal maxX = domain()->maxX();
    qreal minY = domain()->minY();
    qreal maxY = domain()->maxY();

    // 声明返回向量
    QVector<bool> returnVector;
    // 重置返回向量尺寸
    returnVector.resize(m_points.size());

    // During remove animation series may have different number of points,
    // so ensure we don't go over the index. No need to check for zero points, this
    // will not be called in such a situation.
    // 最后一个序列索引
    const int seriesLastIndex = m_series->count() - 1;

    // 遍历点集
    for (int i = 0; i < m_points.size(); i++) {
        // 获取序列点
        const QPointF &seriesPoint = m_series->at(qMin(seriesLastIndex, i));
        // 如果点在范围外
        if (seriesPoint.x() < minX
            || seriesPoint.x() > maxX
            || seriesPoint.y() < minY
            || seriesPoint.y() > maxY) {
            returnVector[i] = true;
        }
        // 如果点未在范围内
        else {
            returnVector[i] = false;
        }
    }

    // 返回带范围标记的点
    return returnVector;
}

// 更新图表
void XYChart::updateChart(QVector<QPointF> &oldPoints, QVector<QPointF> &newPoints, int index)
{
    // 如果动画存在
    if (m_animation) {
        // 设置动画
        m_animation->setup(oldPoints, newPoints, index);
        // 记录点集
        m_points = newPoints;
        // 关闭脏数据开、关
        setDirty(false);
        // 启动动画
        presenter()->startAnimation(m_animation);
    }
    // 如果动画不存在
    else {
        // 存储新点集
        m_points = newPoints;
        // 更新
        updateGeometry();
    }
}

// 更新 OpenGL 图表
void XYChart::updateGlChart()
{
    dataSet()->glXYSeriesDataManager()->setPoints(m_series, domain());
    presenter()->updateGLWidget();
    updateGeometry();
}

// Doesn't update gl geometry, but refreshes the chart
// 刷新 OpenGL 图表
void XYChart::refreshGlChart()
{
    if (presenter())
        presenter()->updateGLWidget();
}

//handlers
// 增加点信号相应槽
void XYChart::handlePointAdded(int index)
{
    Q_ASSERT(index < m_series->count());
    Q_ASSERT(index >= 0);

    // 如果使用OpenGL
    if (m_series->useOpenGL()) {
        updateGlChart(); // 更新OpenGL图表
    }
    // 如果不使用OpenGL
    else {
        QVector<QPointF> points;
        // 如果是脏数据或点集为空
        if (m_dirty || m_points.isEmpty()) {
            points = domain()->calculateGeometryPoints(m_series->pointsVector()); // 计算点集位置
        }
        // 如果不是脏数据且点集非空
        else {
            // 记录点集
            points = m_points;
            // 换算点的位置
            QPointF point = domain()->calculateGeometryPoint(m_series->pointsVector().at(index),
                                                             m_validData);
            // 如果数据无效
            if (!m_validData)
                m_points.clear();
            // 如果数据有效
            else
                points.insert(index, point);
        }
        // 更新图表
        updateChart(m_points, points, index);
    }
}

// 点移除信号相应槽
void XYChart::handlePointRemoved(int index)
{
    Q_ASSERT(index <= m_series->count());
    Q_ASSERT(index >= 0);

    // 如果使用OpenGL
    if (m_series->useOpenGL()) {
        updateGlChart();
    }
    // 如果没有使用OpenGL
    else {
        QVector<QPointF> points;
        // 脏数据或者点集为空
        if (m_dirty || m_points.isEmpty()) {
            // 更新点集
            points = domain()->calculateGeometryPoints(m_series->pointsVector());
        } else {
            // 删除点集
            points = m_points;
            points.remove(index);
        }
        // 更新图表
        updateChart(m_points, points, index);
    }
}

// 点集移除信号相应槽
void XYChart::handlePointsRemoved(int index, int count)
{
    Q_ASSERT(index <= m_series->count());
    Q_ASSERT(index >= 0);

    // 如果使用OpenGL
    if (m_series->useOpenGL()) {
        updateGlChart(); // 更新图表
    } else {
        QVector<QPointF> points;
        // 脏数据，点集为空
        if (m_dirty || m_points.isEmpty()) {
            points = domain()->calculateGeometryPoints(m_series->pointsVector());
        }
        // 非脏数据，点集非空
        else {
            points = m_points;
            points.remove(index, count);
        }
        // 更新图表
        updateChart(m_points, points, index);
    }
}

// 点更换信号相应槽
void XYChart::handlePointReplaced(int index)
{
    Q_ASSERT(index < m_series->count());
    Q_ASSERT(index >= 0);

    // 如果使用OpenGL
    if (m_series->useOpenGL()) {
        updateGlChart(); // 更新图表
    }
    // 如果不使用OpenGL
    else {
        QVector<QPointF> points;
        // 脏数据、点集为空
        if (m_dirty || m_points.isEmpty()) {
            points = domain()->calculateGeometryPoints(m_series->pointsVector());
        }
        // 非脏数据、点击非空
        else {
            QPointF point = domain()->calculateGeometryPoint(m_series->pointsVector().at(index),
                                                             m_validData);
            if (!m_validData)
                m_points.clear();
            points = m_points;
            if (m_validData)
                points.replace(index, point);
        }
        // 更新图表
        updateChart(m_points, points, index);
    }
}

// 点集替换信号相应槽
void XYChart::handlePointsReplaced()
{
    // 使用OpenGL
    if (m_series->useOpenGL()) {
        updateGlChart();
    }
    // 为使用OpenGL
    else {
        // All the points were replaced -> recalculate
        QVector<QPointF> points = domain()->calculateGeometryPoints(m_series->pointsVector());
        updateChart(m_points, points, -1);
    }
}

// 区域更新信号响应槽
void XYChart::handleDomainUpdated()
{
    // 使用OpenGL
    if (m_series->useOpenGL()) {
        updateGlChart();
    }
    // 未使用OpenGL
    else {
        if (isEmpty()) return;
        QVector<QPointF> points = domain()->calculateGeometryPoints(m_series->pointsVector());
        updateChart(m_points, points);
    }
}

// 图表项是否为空
bool XYChart::isEmpty()
{
    return domain()->isEmpty() || m_series->points().isEmpty();
}

#include "moc_xychart_p.cpp"

QT_CHARTS_END_NAMESPACE
