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

#ifndef QAREASERIES_P_H
#define QAREASERIES_P_H

#include <private/qabstractseries_p.h>
#include <QtCharts/private/qchartglobal_p.h>

QT_CHARTS_BEGIN_NAMESPACE

class QAreaSeries;
class QLineSeries;

// 区域序列私有成员
class QT_CHARTS_PRIVATE_EXPORT QAreaSeriesPrivate: public QAbstractSeriesPrivate
{
    Q_OBJECT

public:
    QAreaSeriesPrivate(QLineSeries *upperSeries, QLineSeries *lowerSeries, QAreaSeries *q); // 构造

    void initializeDomain(); // 初始化区域
    void initializeAxes(); // 初始化轴
    void initializeGraphics(QGraphicsItem* parent); // 初始化图像
    void initializeTheme(int index, ChartTheme* theme, bool forced = false); // 初始化主题
    void initializeAnimations(QChart::AnimationOptions options, int duration,
                              QEasingCurve &curve); // 初始化动画

    QList<QLegendMarker *> createLegendMarkers(QLegend *legend); // 创建图例标记

    QAbstractAxis::AxisType defaultAxisType(Qt::Orientation orientation) const; // 默认轴类型
    QAbstractAxis* createDefaultAxis(Qt::Orientation) const; // 创建默认轴

Q_SIGNALS:
    void updated();

protected:
    QBrush m_brush; // 画刷
    QPen m_pen; // 画笔
    QLineSeries *m_upperSeries; // 序列上界
    QLineSeries *m_lowerSeries; // 序列下界
    bool m_pointsVisible; // 点集是否可视
    QString m_pointLabelsFormat; // 点标签格式
    bool m_pointLabelsVisible; // 点标签是否可见
    QFont m_pointLabelsFont; // 点标签字体
    QColor m_pointLabelsColor; // 点标签颜色
    bool m_pointLabelsClipping; // 点标签是否剪裁

private:
    Q_DECLARE_PUBLIC(QAreaSeries);
};

QT_CHARTS_END_NAMESPACE

#endif
