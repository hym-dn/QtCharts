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

#ifndef CHARTTHEMEMANAGER_H
#define CHARTTHEMEMANAGER_H

#include <QtCharts/QChartGlobal>
#include <QtCharts/QChart>
#include <QtGui/QColor>
#include <QtGui/QGradientStops>
#include <QtCharts/private/qchartglobal_p.h>

QT_CHARTS_BEGIN_NAMESPACE
class ChartTheme;

// 主题管理器、负责维护系统中的主题信息。
class QT_CHARTS_PRIVATE_EXPORT ChartThemeManager: public QObject
{
    Q_OBJECT
public:
    // 背景阴影模式
    enum BackgroundShadesMode {
        BackgroundShadesNone = 0,
        BackgroundShadesVertical,
        BackgroundShadesHorizontal,
        BackgroundShadesBoth
    };

public:
    explicit ChartThemeManager(QChart* chart); // 构造
    void setTheme(QChart::ChartTheme theme); // 设置主题
    ChartTheme* theme() const { return m_theme.data(); } // 获取主题
    void decorateChart(QChart *chart, ChartTheme* theme) const; // 布置图表
    void decorateLegend(QLegend *legend, ChartTheme* theme) const; // 布置图例
    void updateSeries(QAbstractSeries *series); // 更新指定序列
    QMap<QAbstractSeries *, int> seriesMap() const { return m_seriesMap; } // 获取序列映射

public:
    static QList<QGradient> generateSeriesGradients(const QList<QColor>& colors); // 生成颜色梯度
    static QColor colorAt(const QColor &start, const QColor &end, qreal pos); // 获取指定位置颜色
    static QColor colorAt(const QGradient &gradient, qreal pos); // 获取梯度中指定位置颜色

private:
    int createIndexKey(QList<int> keys) const; // 创建键值
    int seriesCount(QAbstractSeries::SeriesType type) const; // 指定类型的序列计数

public Q_SLOTS:
    void handleSeriesAdded(QAbstractSeries *series); // 序列增加响应槽
    void handleSeriesRemoved(QAbstractSeries *series); // 序列移除响应槽
    void handleAxisAdded(QAbstractAxis *axis); // 坐标轴增加响应槽
    void handleAxisRemoved(QAbstractAxis *axis); // 坐标轴移除响应槽

protected:
    QScopedPointer<ChartTheme> m_theme; // 主题
    QMap<QAbstractSeries *,int> m_seriesMap; // 序列映射
    QList<QAbstractAxis *> m_axisList; // 轴链表
    QChart* m_chart; // 图表
};

QT_CHARTS_END_NAMESPACE

#endif // CHARTTHEME_H
