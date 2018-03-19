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

#ifndef CHARTTHEME_H
#define CHARTTHEME_H

#include <private/chartthememanager_p.h>
#include <QtGui/QColor>
#include <QtGui/QGradientStops>
#include <QtCharts/private/qchartglobal_p.h>

QT_CHARTS_BEGIN_NAMESPACE

// 图表主题，负责维护、管理一项主题。
class QT_CHARTS_PRIVATE_EXPORT ChartTheme
{

public:
    // 背景模式
    enum BackgroundShadesMode {
        BackgroundShadesNone = 0,
        BackgroundShadesVertical,
        BackgroundShadesHorizontal,
        BackgroundShadesBoth
    };

protected:
    // 构造函数
    explicit ChartTheme(QChart::ChartTheme id = QChart::ChartThemeLight):m_id(id),
        m_backgroundShadesBrush(Qt::SolidPattern),
        m_backgroundShades(BackgroundShadesNone),
        m_backgroundDropShadowEnabled(false)
    {};
public:
    QChart::ChartTheme id() const { return m_id; } // 主题ID
    QList<QGradient> seriesGradients() const { return m_seriesGradients; } // 梯度序列
    QList<QColor> seriesColors() const { return m_seriesColors; } // 颜色序列
    QLinearGradient chartBackgroundGradient() const { return m_chartBackgroundGradient; } // 背景梯度
    QFont masterFont() const { return m_masterFont; } // 主字体
    QFont labelFont() const { return m_labelFont; } // 标签字体
    QBrush labelBrush() const { return m_labelBrush; } // 标签画刷
    QPen axisLinePen() const { return m_axisLinePen; } // 轴线画笔
    QPen backgroundShadesPen() const { return m_backgroundShadesPen; } // 背景阴影画笔
    QPen outlinePen() const { return m_outlinePen; } // 轮廓画笔
    QBrush backgroundShadesBrush() const { return m_backgroundShadesBrush; } // 背景阴影画笔
    BackgroundShadesMode backgroundShades() const { return m_backgroundShades; } // 背景阴影模式
    bool isBackgroundDropShadowEnabled() const { return m_backgroundDropShadowEnabled; }  // ???
    QPen gridLinePen() const { return m_gridLinePen; } // 网格画笔
    QPen minorGridLinePen() const { return m_minorGridLinePen; } // 副网格画笔

protected:
    QChart::ChartTheme m_id; // 主题ID
    QList<QColor> m_seriesColors; // 颜色序列
    QList<QGradient> m_seriesGradients; // 梯度序列
    QLinearGradient m_chartBackgroundGradient; // 背景梯度

    QFont m_masterFont; // 主字体
    QFont m_labelFont; // 标签字体
    QBrush m_labelBrush; // 标签画刷
    QPen m_axisLinePen; // 轴线画笔
    QPen m_backgroundShadesPen; // 背景阴影画笔
    QPen m_outlinePen; // 轮廓画笔
    QBrush m_backgroundShadesBrush; // 背景阴影画刷
    BackgroundShadesMode m_backgroundShades; // 背景阴影模式
    bool m_backgroundDropShadowEnabled; // ???
    QPen m_gridLinePen; // 网格画笔
    QPen m_minorGridLinePen; // 副网格画笔

};

QT_CHARTS_END_NAMESPACE

#endif // CHARTTHEME_H
