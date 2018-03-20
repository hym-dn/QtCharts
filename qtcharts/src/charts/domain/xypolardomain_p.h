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

#ifndef XYPOLARDOMAIN_H
#define XYPOLARDOMAIN_H
#include <private/polardomain_p.h>
#include <QtCharts/private/qchartglobal_p.h>
#include <QtCore/QRectF>

QT_CHARTS_BEGIN_NAMESPACE

// x-y极坐标区域
class QT_CHARTS_PRIVATE_EXPORT XYPolarDomain: public PolarDomain
{
    Q_OBJECT
public:
    explicit XYPolarDomain(QObject *object = 0); // 构造
    virtual ~XYPolarDomain(); // 析构

    DomainType type(){ return AbstractDomain::XYPolarDomain;} // 返回区域类型

    void setRange(qreal minX, qreal maxX, qreal minY, qreal maxY); // 设置范围

    friend bool Q_AUTOTEST_EXPORT operator== (const XYPolarDomain &Domain1, const XYPolarDomain &Domain2); // 相等判定
    friend bool Q_AUTOTEST_EXPORT operator!= (const XYPolarDomain &Domain1, const XYPolarDomain &Domain2); // 不等判定
    friend QDebug Q_AUTOTEST_EXPORT operator<<(QDebug dbg, const XYPolarDomain &AbstractDomain); // 向指定Debug输出指定区域

    void zoomIn(const QRectF &rect); // 缩小
    void zoomOut(const QRectF &rect); // 放大
    void move(qreal dx, qreal dy); // 平移

    QPointF calculateDomainPoint(const QPointF &point) const; // 计算区域点

protected:
    qreal toAngularCoordinate(qreal value, bool &ok) const; // 转换为角度
    qreal toRadialCoordinate(qreal value, bool &ok) const; // 转换为极值
};

QT_CHARTS_END_NAMESPACE

#endif // XYPOLARDOMAIN_H
