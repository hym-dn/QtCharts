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

#ifndef LEGENDLAYOUT_H
#define LEGENDLAYOUT_H
#include <QtWidgets/QGraphicsLayout>
#include <QtCharts/QChartGlobal>
#include <QtCharts/private/qchartglobal_p.h>

QT_CHARTS_BEGIN_NAMESPACE

class QLegend;
class LegendMarkerItem;

// 图列布局
class QT_CHARTS_PRIVATE_EXPORT LegendLayout : public QGraphicsLayout
{
public:
    LegendLayout(QLegend *legend); // 构造
    virtual ~LegendLayout(); // 析构

    void setGeometry(const QRectF &rect); // 设置几何尺寸

    void setOffset(qreal x, qreal y); // 设置偏移
    QPointF offset() const; // 获取偏移

    void invalidate(); // 更新
protected:
    QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint = QSizeF()) const; // 默认尺寸
    int count() const { return 0; } // 计数
    QGraphicsLayoutItem *itemAt(int) const { return 0; }; // 指定项目
    void removeAt(int) {}; // 删除项目

private:
    void setAttachedGeometry(const QRectF &rect); // 设置绑定几何尺寸
    void setDettachedGeometry(const QRectF &rect); // 设置松绑几何尺寸

    // 标记项、宽度结构体
    struct LegendWidthStruct {
        LegendMarkerItem *item;
        qreal width;
    };
    static bool widthLongerThan(const LegendWidthStruct *item1,
                                const LegendWidthStruct *item2); // 标记项、宽度结构体比较函数

private:
    QLegend *m_legend; // 所属图例
    qreal m_offsetX; // x偏移
    qreal m_offsetY; // y偏移
    qreal m_minOffsetX; // 最小x偏移
    qreal m_minOffsetY; // 最小y偏移
    qreal m_maxOffsetX; // 最大x偏移
    qreal m_maxOffsetY; // 最大y偏移
    qreal m_width; // 宽度
    qreal m_height; // 高度
};

QT_CHARTS_END_NAMESPACE

#endif
