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

#include <private/chartelement_p.h>
#include <private/chartpresenter_p.h>
#include <private/abstractdomain_p.h>
#include <private/chartdataset_p.h>

QT_CHARTS_BEGIN_NAMESPACE

// 构造函数
ChartElement::ChartElement(QGraphicsItem* item):
      QGraphicsObject(item), // 基类构造
      m_presenter(0), // 图表主持
      m_themeManager(0), // 图表主题
      m_dataSet(0) // 数据集
{
}

// 设置主持
void ChartElement::setPresenter(ChartPresenter *presenter)
{
    m_presenter = presenter;
}

// 获取主持
ChartPresenter *ChartElement::presenter() const
{
    return m_presenter;
}

// 设置主题
void ChartElement::setThemeManager(ChartThemeManager *manager)
{
    m_themeManager = manager;
}

// 返回主题
ChartThemeManager* ChartElement::themeManager() const
{
    return m_themeManager;
}

// 设置数据集
void ChartElement::setDataSet(ChartDataSet *dataSet)
{
    m_dataSet = dataSet;
}

// 返回数据集
ChartDataSet *ChartElement::dataSet() const
{
    return m_dataSet;
}

QT_CHARTS_END_NAMESPACE
