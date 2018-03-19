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
#include <private/qabstractseries_p.h>
#include <private/qabstractaxis_p.h>
//themes
#include <private/chartthemesystem_p.h>
#include <private/chartthemelight_p.h>
#include <private/chartthemebluecerulean_p.h>
#include <private/chartthemedark_p.h>
#include <private/chartthemebrownsand_p.h>
#include <private/chartthemebluencs_p.h>
#include <private/chartthemehighcontrast_p.h>
#include <private/chartthemeblueicy_p.h>
#include <private/chartthemeqt_p.h>

QT_CHARTS_BEGIN_NAMESPACE

// 构造函数
ChartThemeManager::ChartThemeManager(QChart* chart) :
    m_chart(chart)
{
}


// 设置当前主题
void ChartThemeManager::setTheme(QChart::ChartTheme theme)
{
    // 如果主题为空并与目标主题不同
    if (m_theme.isNull() || theme != m_theme->id()) {
        // 区分主题，并创建相应的主题类
        switch (theme) {
        case QChart::ChartThemeLight:
            m_theme.reset(new ChartThemeLight());
            break;
        case QChart::ChartThemeBlueCerulean:
            m_theme.reset(new ChartThemeBlueCerulean());
            break;
        case QChart::ChartThemeDark:
            m_theme.reset(new ChartThemeDark());
            break;
        case QChart::ChartThemeBrownSand:
            m_theme.reset(new ChartThemeBrownSand());
            break;
        case QChart::ChartThemeBlueNcs:
            m_theme.reset(new ChartThemeBlueNcs());
            break;
        case QChart::ChartThemeHighContrast:
            m_theme.reset(new ChartThemeHighContrast());
            break;
        case QChart::ChartThemeBlueIcy:
            m_theme.reset(new ChartThemeBlueIcy());
            break;
        case QChart::ChartThemeQt:
            m_theme.reset(new ChartThemeQt());
            break;
        default:
            m_theme.reset(new ChartThemeSystem());
            break;
        }
        // 如果主题
        if (!m_theme.isNull()) {
            decorateChart(m_chart,m_theme.data()); // 利用指定主题，布置指定图表
            decorateLegend(m_chart->legend(),m_theme.data()); // 利用指定主题，布置指定图例
            foreach (QAbstractAxis* axis, m_axisList) // 重新初始化轴的主题
                axis->d_ptr->initializeTheme(m_theme.data(), true);
            foreach (QAbstractSeries* series, m_seriesMap.keys()) // 重新初始化序列的主题
                series->d_ptr->initializeTheme(m_seriesMap[series], m_theme.data(), true);
        }
    }
}

// decorateChart is only called when theme is forcibly initialized
// 利用指定主题，布置指定图表
void ChartThemeManager::decorateChart(QChart *chart, ChartTheme *theme) const
{
    chart->setBackgroundBrush(theme->chartBackgroundGradient());

    QPen pen(Qt::transparent);
    QBrush brush;
    chart->setPlotAreaBackgroundBrush(brush);
    chart->setPlotAreaBackgroundPen(pen);
    chart->setPlotAreaBackgroundVisible(false);

    chart->setTitleFont(theme->masterFont());
    chart->setTitleBrush(theme->labelBrush());
    chart->setDropShadowEnabled(theme->isBackgroundDropShadowEnabled());
}

// decorateLegend is only called when theme is forcibly initialized
// 利用指定主题，布置指定图例
void ChartThemeManager::decorateLegend(QLegend *legend, ChartTheme *theme) const
{
    legend->setPen(theme->axisLinePen());
    legend->setBrush(theme->chartBackgroundGradient());
    legend->setFont(theme->labelFont());
    legend->setLabelBrush(theme->labelBrush());
}

// 创建索引键
int ChartThemeManager::createIndexKey(QList<int> keys) const
{
    // 从小到达排序
    std::sort(keys.begin(), keys.end());

    int key = 0;
    QList<int>::iterator i;
    i = keys.begin();
    // 挑选出不在链表中最小的键值
    while (i != keys.end()) {
        if (*i != key)
            break;
        key++;
        i++;
    }
    // 返回键值
    return key;
}

// 指定类型的序列计数
int ChartThemeManager::seriesCount(QAbstractSeries::SeriesType type) const
{
    int count = 0;
    QList<QAbstractSeries *> series =   m_seriesMap.keys();
    foreach(QAbstractSeries *s, series) {
        if (s->type() == type)
            count++;
    }
    return count;
}

// 序列增加相应槽
// 为序列分配主键并存储，同时初始化序列主题
void ChartThemeManager::handleSeriesAdded(QAbstractSeries *series)
{
    int key = createIndexKey(m_seriesMap.values());
    m_seriesMap.insert(series,key);
    series->d_ptr->initializeTheme(key,m_theme.data(),false);
}

// 序列被移除相应槽
void ChartThemeManager::handleSeriesRemoved(QAbstractSeries *series)
{
    m_seriesMap.remove(series); // 移除序列
}

// 坐标抽增加相应槽
void ChartThemeManager::handleAxisAdded(QAbstractAxis *axis)
{
    // 增加坐标轴
    m_axisList.append(axis);
    // 初始化主题
    axis->d_ptr->initializeTheme(m_theme.data(),false);
}

// 坐标轴移除相应槽
void ChartThemeManager::handleAxisRemoved(QAbstractAxis *axis)
{
    // 移除坐标轴
    m_axisList.removeAll(axis);
}

// 更新指定序列
void ChartThemeManager::updateSeries(QAbstractSeries *series)
{
    if(m_seriesMap.contains(series)){
        series->d_ptr->initializeTheme(m_seriesMap[series],m_theme.data(),false);
    }
}


// 生成序列梯度
QList<QGradient> ChartThemeManager::generateSeriesGradients(const QList<QColor>& colors)
{
    // 创建临时梯度
    QList<QGradient> result;
    // 生成HSV颜色空间的梯度
    // Generate gradients in HSV color space
    foreach (const QColor &color, colors) {
        QLinearGradient g;
        qreal h = color.hsvHueF();
        qreal s = color.hsvSaturationF();

        QColor start = color;
        start.setHsvF(h, 0.0, 1.0);
        g.setColorAt(0.0, start);

        g.setColorAt(0.5, color);

        QColor end = color;
        end.setHsvF(h, s, 0.25);
        g.setColorAt(1.0, end);

        result << g;
    }
    // 返回HSV颜色空间梯度
    return result;
}

// 获取指定位置的颜色
// strat - 起始颜色
// end - 结束颜色
// pos - 指定位置
QColor ChartThemeManager::colorAt(const QColor &start, const QColor &end, qreal pos)
{
    Q_ASSERT(pos >= 0.0 && pos <= 1.0);
    qreal r = start.redF() + ((end.redF() - start.redF()) * pos);
    qreal g = start.greenF() + ((end.greenF() - start.greenF()) * pos);
    qreal b = start.blueF() + ((end.blueF() - start.blueF()) * pos);
    QColor c;
    c.setRgbF(r, g, b);
    return c;
}

// 获取梯度中指定位置的颜色
// gradient - 指定梯度
// pos - 指定位置
QColor ChartThemeManager::colorAt(const QGradient &gradient, qreal pos)
{
    Q_ASSERT(pos >= 0 && pos <= 1.0);

    QGradientStops stops = gradient.stops();
    int count = stops.count();

    // find previous stop relative to position
    QGradientStop prev = stops.first();
    for (int i = 0; i < count; i++) {
        QGradientStop stop = stops.at(i);
        if (pos > stop.first)
            prev = stop;

        // given position is actually a stop position?
        if (pos == stop.first) {
            //qDebug() << "stop color" << pos;
            return stop.second;
        }
    }

    // find next stop relative to position
    QGradientStop next = stops.last();
    for (int i = count - 1; i >= 0; i--) {
        QGradientStop stop = stops.at(i);
        if (pos < stop.first)
            next = stop;
    }

    //qDebug() << "prev" << prev.first << "pos" << pos << "next" << next.first;

    qreal range = next.first - prev.first;
    qreal posDelta = pos - prev.first;
    qreal relativePos = posDelta / range;

    //qDebug() << "range" << range << "posDelta" << posDelta << "relativePos" << relativePos;

    return colorAt(prev.second, next.second, relativePos);
}

#include "moc_chartthememanager_p.cpp"

QT_CHARTS_END_NAMESPACE
