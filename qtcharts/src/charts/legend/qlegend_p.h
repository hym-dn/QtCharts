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

#ifndef QLEGEND_P_H
#define QLEGEND_P_H

#include <QtCharts/QLegend>
#include <QtCharts/private/qchartglobal_p.h>

QT_CHARTS_BEGIN_NAMESPACE

class QChart;
class ChartPresenter;
class QAbstractSeries;
class LegendLayout;
class QLegendMarker;

// 图例私有成员类
class QT_CHARTS_PRIVATE_EXPORT QLegendPrivate : public QObject
{
    Q_OBJECT
public:
    QLegendPrivate(ChartPresenter *presenter, QChart *chart, QLegend *q); // 构造
    ~QLegendPrivate(); // 析构

    void setOffset(const QPointF &offset); // 设置偏移
    QPointF offset() const;
    int roundness(qreal size);

    QGraphicsItemGroup* items() { return m_items; }

    QList<QLegendMarker*> markers(QAbstractSeries *series = 0);
    qreal maxMarkerWidth() const;

public Q_SLOTS:
    void handleSeriesAdded(QAbstractSeries *series);
    void handleSeriesRemoved(QAbstractSeries *series);
    void handleSeriesVisibleChanged();
    void handleCountChanged();

private:
    // Internal helpers
    void addMarkers(QList<QLegendMarker *> markers);
    void removeMarkers(QList<QLegendMarker *> markers);
    void decorateMarkers(QList<QLegendMarker *> markers);
    void updateToolTips();

private:
    QLegend *q_ptr; // 源图例
    ChartPresenter *m_presenter; // 图表主持
    LegendLayout *m_layout; // 图表布局
    QChart *m_chart; // 图表
    QGraphicsItemGroup *m_items; // 项目组
    Qt::Alignment m_alignment; // 停靠方式
    QBrush m_brush; // 画刷
    QPen m_pen; // 画笔
    QFont m_font; // 字体
    QBrush m_labelBrush; // 标签画刷

    qreal m_diameter; // 直径
    bool m_attachedToChart; // 是否捆绑到图表
    bool m_backgroundVisible; // 背景是否可见
    bool m_reverseMarkers; // 标记是否逆向
    bool m_showToolTips; // 是否显示提示
    QLegend::MarkerShape m_markerShape; // 标记形状

    QList<QLegendMarker *> m_markers; // 标记链表
    QList<QAbstractSeries *> m_series; // 序列链表

    QHash<QGraphicsItem *, QLegendMarker *> m_markerHash; // 标记映射

    friend class QLegend;
    friend class LegendMarkerItem;
    friend class LegendLayout;
    friend class QLegendMarkerPrivate;
    friend class LegendScroller;
};

QT_CHARTS_END_NAMESPACE

#endif
