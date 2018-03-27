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

#ifndef QABSTRACTBARSERIES_P_H
#define QABSTRACTBARSERIES_P_H

#include <QtCharts/QAbstractBarSeries>
#include <private/qabstractseries_p.h>
#include <QtCore/QStringList>
#include <QtCharts/QAbstractSeries>
#include <QtCharts/private/qchartglobal_p.h>

QT_CHARTS_BEGIN_NAMESPACE

class QBarModelMapper;
class QBarCategoryAxis;
class QLegendMarker;

// 柱状序列私有成员
class QT_CHARTS_PRIVATE_EXPORT QAbstractBarSeriesPrivate : public QAbstractSeriesPrivate
{
    Q_OBJECT
public:
    QAbstractBarSeriesPrivate(QAbstractBarSeries *parent); // 构造
    int categoryCount() const; // 目录计数

    void setBarWidth(qreal width); // 设置柱状图宽度
    qreal barWidth() const; // 获取柱状图宽度

    void setVisible(bool visible); // 设置是否可视
    void setLabelsVisible(bool visible); // 设置标签是否可视

    void initializeDomain(); // 初始化区域
    void initializeAxes(); // 初始化轴
    void initializeAnimations(QChart::AnimationOptions options, int duration, QEasingCurve &curve); // 初始化动画
    void initializeTheme(int index, ChartTheme* theme, bool forced = false); // 初始化主题

    QList<QLegendMarker*> createLegendMarkers(QLegend *legend); // 创建图例标志

    virtual QAbstractAxis::AxisType defaultAxisType(Qt::Orientation orientation) const; // 默认的坐标轴类型
    QAbstractAxis* createDefaultAxis(Qt::Orientation orientation) const; // 创建默认坐标轴

    bool append(QBarSet *set); // 增加数据集
    bool remove(QBarSet *set); // 删除数据集
    bool append(QList<QBarSet *> sets); // 追加数据集合
    bool remove(QList<QBarSet *> sets); // 删除数据集合
    bool insert(int index, QBarSet *set); // 插入数据集

    QBarSet *barsetAt(int index); // 获取指定数据集
    qreal min(); // 获取最小值
    qreal max(); // 获取最大值
    qreal valueAt(int set, int category); // 获取指定值
    qreal percentageAt(int set, int category); // 获取百分比
    qreal categorySum(int category); // 获取指定目录和
    qreal absoluteCategorySum(int category); // 获取相对目录和
    qreal maxCategorySum(); // 最大目录和
    qreal minX(); // 最小x
    qreal maxX(); // 最大x
    qreal categoryTop(int category); // 目录顶部
    qreal categoryBottom(int category); // 目录底部
    qreal top(); // 顶部
    qreal bottom(); // 底部

    bool blockBarUpdate(); // 阻止更新

    qreal labelsAngle() const; // 标签角度
    void setVisualsDirty(bool dirty) { m_visualsDirty = dirty; } // ???
    bool visualsDirty() const { return m_visualsDirty; } // ???
    void setLabelsDirty(bool dirty) { m_labelsDirty = dirty; } // ???
    bool labelsDirty() const { return m_labelsDirty; } // ???

Q_SIGNALS:
    void clicked(int index, QBarSet *barset);
    void pressed(int index, QBarSet *barset);
    void released(int index, QBarSet *barset);
    void doubleClicked(int index, QBarSet *barset);
    void updatedBars();
    void updatedLayout();
    void restructuredBars();
    void labelsVisibleChanged(bool visible);
    void visibleChanged();
    void setValueChanged(int index, QBarSet *barset);
    void setValueAdded(int index, int count, QBarSet *barset);
    void setValueRemoved(int index, int count, QBarSet *barset);

private Q_SLOTS:
    void handleSetValueChange(int index); // 值变化信号改变槽
    void handleSetValueAdd(int index, int count); // 值增加信号响应槽
    void handleSetValueRemove(int index, int count); // 值删除信号响应槽

private:
    void populateCategories(QBarCategoryAxis *axis); // 为轴增加目录

protected:
    QList<QBarSet *> m_barSets; // 柱状数据集
    qreal m_barWidth; // 柱子宽度
    bool m_labelsVisible; // 标签是否可见
    bool m_visible; // 是否可见
    bool m_blockBarUpdate; // 是否阻塞更新
    QString m_labelsFormat; // 标签格式
    QAbstractBarSeries::LabelsPosition m_labelsPosition; // 标签位置
    qreal m_labelsAngle; // 标签角度
    int m_labelsPrecision; // 标签精度
    bool m_visualsDirty; // ???
    bool m_labelsDirty; // ???

private:
    Q_DECLARE_PUBLIC(QAbstractBarSeries)
    friend class HorizontalBarChartItem;
    friend class BarChartItem;
};

QT_CHARTS_END_NAMESPACE

#endif // QABSTRACTBARSERIES_P_H
