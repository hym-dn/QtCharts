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

#ifndef QABSTRACTSERIES_P_H
#define QABSTRACTSERIES_P_H

#include <QtCharts/QAbstractSeries>
#include <QtCharts/QChart>
#include <private/abstractdomain_p.h>
#include <QtCharts/private/qchartglobal_p.h>

QT_BEGIN_NAMESPACE
class QGraphicsItem;
QT_END_NAMESPACE

QT_CHARTS_BEGIN_NAMESPACE

class ChartPresenter;
class ChartElement;
class LegendMarker;
class QLegend;
class ChartDataSet;
class QAbstractAxis;
class QLegendMarker;
class ChartTheme;
class ChartAnimation;
class ChartItem;
class BoxPlotChartItem;

// 序列私有成员，各种序列的私有成员均继承于此
class QT_CHARTS_PRIVATE_EXPORT QAbstractSeriesPrivate : public QObject
{
    Q_OBJECT
public:
    QAbstractSeriesPrivate(QAbstractSeries *q); // 构造函数
    ~QAbstractSeriesPrivate(); // 析构函数

    virtual void initializeDomain() = 0; // 初始化区域
    virtual void initializeAxes() = 0; // 初始化轴
    virtual void initializeTheme(int index, ChartTheme* theme, bool forced = false) = 0; // 初始化主题
    virtual void initializeGraphics(QGraphicsItem* parent) = 0; // 初始化图像
    virtual void initializeAnimations(QChart::AnimationOptions options, int duration,
                                      QEasingCurve &curve) = 0; // 初始化动画

    virtual QList<QLegendMarker*> createLegendMarkers(QLegend* legend) = 0; // 创建图例标志

    virtual QAbstractAxis::AxisType defaultAxisType(Qt::Orientation) const = 0; // 默认轴类型
    virtual QAbstractAxis* createDefaultAxis(Qt::Orientation) const = 0; // 创建默认轴

    ChartItem* chartItem() { return m_item.data(); } // 返回图表项

    virtual void setDomain(AbstractDomain* domain); // 设置区域
    AbstractDomain* domain() { return m_domain.data(); } // 返回区域

    virtual void setPresenter(ChartPresenter *presenter); // 设置主持人
    ChartPresenter *presenter() const; // 返回主持人

    QChart* chart() { return m_chart; } // 返回所属图表

    void setBlockOpenGL(bool enable); // ???

Q_SIGNALS:
    void countChanged(); // 计数变更信号

protected:
    QAbstractSeries *q_ptr; // 源序列指针
    QChart *m_chart; // 源图表指针
    QScopedPointer<ChartItem> m_item; // 图表项
    QList<QAbstractAxis*> m_axes; // 坐标轴

private:
    QScopedPointer<AbstractDomain> m_domain; // 区域
    QString m_name; // 名称
    bool m_visible; // 是否可见
    qreal m_opacity;  // 不透明
    ChartPresenter *m_presenter; // 主持人
    bool m_useOpenGL; // 是否使用OpenGL
    bool m_blockOpenGL; // 是否阻止OpenGL

    friend class QAbstractSeries;
    friend class ChartDataSet;
    friend class ChartPresenter;
    friend class QLegendPrivate;
    friend class BoxPlotChartItem;
};

QT_CHARTS_END_NAMESPACE

#endif
