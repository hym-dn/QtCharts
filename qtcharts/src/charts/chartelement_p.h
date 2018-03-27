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

#ifndef CHARTELEMENT_H
#define CHARTELEMENT_H

#include <QtCharts/QChartGlobal>
#include <QtCore/QObject>
#include <QtCore/QRect>
#include <QGraphicsObject>
#include <QtCharts/private/qchartglobal_p.h>

QT_CHARTS_BEGIN_NAMESPACE

class ChartAnimator;
class ChartPresenter;
class ChartAnimation;
class ChartThemeManager;
class AbstractDomain;
class ChartDataSet;

// 图表元素类，他是图表中全部元素的基类
class QT_CHARTS_PRIVATE_EXPORT ChartElement: public QGraphicsObject
{

public:
    explicit ChartElement(QGraphicsItem* item = 0); // 构造

    virtual ChartAnimation *animation() const { return 0; } // 获取动画
    virtual void setPresenter(ChartPresenter *presenter); // 设置主持
    ChartPresenter *presenter() const; // 返回主持
    virtual void setThemeManager(ChartThemeManager *manager); // 设置主题
    ChartThemeManager* themeManager() const; // 返回主题
    virtual void setDataSet(ChartDataSet *dataSet);
    ChartDataSet *dataSet() const;

private:
    ChartPresenter *m_presenter; // 图表主持
    ChartThemeManager *m_themeManager; // 图表主题
    ChartDataSet *m_dataSet; // 数据集
};

QT_CHARTS_END_NAMESPACE

#endif
