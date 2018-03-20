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

#ifndef ABSTRACTDOMAIN_H
#define ABSTRACTDOMAIN_H
#include <QtCharts/QChartGlobal>
#include <QtCharts/private/qchartglobal_p.h>
#include <QtCore/QRectF>
#include <QtCore/QSizeF>
#include <QtCore/QDebug>

QT_CHARTS_BEGIN_NAMESPACE

class QAbstractAxis;

class QT_CHARTS_PRIVATE_EXPORT AbstractDomain: public QObject
{
    Q_OBJECT
public:
    enum DomainType { UndefinedDomain,
                      XYDomain,
                      XLogYDomain,
                      LogXYDomain,
                      LogXLogYDomain,
                      XYPolarDomain,
                      XLogYPolarDomain,
                      LogXYPolarDomain,
                      LogXLogYPolarDomain }; // 区域类型
public:
    explicit AbstractDomain(QObject *object = 0); // 构造
    virtual ~AbstractDomain(); // 析构

    virtual void setSize(const QSizeF &size); // 设置尺寸
    QSizeF size() const; // 获取

    virtual DomainType type() = 0; // 获取区域类型

    virtual void setRange(qreal minX, qreal maxX, qreal minY, qreal maxY) = 0; // 设置范围
    void setRangeX(qreal min, qreal max); // 设置x方向范围
    void setRangeY(qreal min, qreal max); // 设置y方向范围
    void setMinX(qreal min); // 设置最小x
    void setMaxX(qreal max); // 设置最大x
    void setMinY(qreal min); // 设置最小y
    void setMaxY(qreal max); // 设置最大y

    qreal minX() const { return m_minX; } // 获取最小x
    qreal maxX() const { return m_maxX; } // 获取最大x
    qreal minY() const { return m_minY; } // 获取最小y
    qreal maxY() const { return m_maxY; } // 获取最大y

    qreal spanX() const; // 获取x范围
    qreal spanY() const; // 获取y范围
    bool isEmpty() const; // 区域是否为空

    void blockRangeSignals(bool block); // 阻塞范围信号
    bool rangeSignalsBlocked() const { return m_signalsBlocked; } // 检测范围信号是否被阻塞

    void zoomReset(); // 缩放重置
    void storeZoomReset(); // 存储缩放重置信息
    bool isZoomed() { return m_zoomed; } // 返回是否缩放

    friend bool Q_AUTOTEST_EXPORT operator== (const AbstractDomain &domain1, const AbstractDomain &domain2); // 判断区域是否相同
    friend bool Q_AUTOTEST_EXPORT operator!= (const AbstractDomain &domain1, const AbstractDomain &domain2); // 判断区域是否不同
    friend QDebug Q_AUTOTEST_EXPORT operator<<(QDebug dbg, const AbstractDomain &domain); // 向指定Debug输出指定区域

    virtual void zoomIn(const QRectF &rect) = 0; // 缩小
    virtual void zoomOut(const QRectF &rect) = 0; // 放大
    virtual void move(qreal dx, qreal dy) = 0; // 平移

    virtual QPointF calculateGeometryPoint(const QPointF &point, bool &ok) const = 0; // 计算几何点
    virtual QPointF calculateDomainPoint(const QPointF &point) const = 0; // 计算区域点
    virtual QVector<QPointF> calculateGeometryPoints(const QVector<QPointF> &vector) const = 0; // 计算几何点集

    virtual bool attachAxis(QAbstractAxis *axis); // 捆绑坐标轴
    virtual bool detachAxis(QAbstractAxis *axis); // 松绑坐标轴

    static void looseNiceNumbers(qreal &min, qreal &max, int &ticksCount); // 合适的数
    static qreal niceNumber(qreal x, bool ceiling); // 合适的数

    void setReverseX(bool reverse) { m_reverseX = reverse; } // 设置逆向x
    void setReverseY(bool reverse) { m_reverseY = reverse; } // 设置逆向y

    bool isReverseX() const { return m_reverseX; } // 获取逆向x
    bool isReverseY() const { return m_reverseY; } // 获取逆向y

Q_SIGNALS:
    void updated();
    void rangeHorizontalChanged(qreal min, qreal max);
    void rangeVerticalChanged(qreal min, qreal max);

public Q_SLOTS:
    void handleVerticalAxisRangeChanged(qreal min,qreal max); // 垂直轴范围改变信号响应槽
    void handleHorizontalAxisRangeChanged(qreal min,qreal max); // 水平轴范围改变信号相应槽
    void handleReverseXChanged(bool reverse); // 逆向x改变信号响应槽
    void handleReverseYChanged(bool reverse); // 逆向y改变信号相应槽

protected:
    void adjustLogDomainRanges(qreal &min, qreal &max); // 调整Log区域范围
    QRectF fixZoomRect(const QRectF &rect); //

    qreal m_minX; // 最小X
    qreal m_maxX; // 最大X
    qreal m_minY; // 最小Y
    qreal m_maxY; // 最大Y
    QSizeF m_size; // 尺寸
    bool m_signalsBlocked; // ???
    bool m_zoomed; // 缩放标志
    qreal m_zoomResetMinX; // 缩放重置最小X
    qreal m_zoomResetMaxX; // 缩放重置最大X
    qreal m_zoomResetMinY; // 缩放重置最小Y
    qreal m_zoomResetMaxY; // 缩放重置最大X
    bool m_reverseX; // 是否反向X
    bool m_reverseY; // 是否反向Y
};

QT_CHARTS_END_NAMESPACE

#endif // ABSTRACTDOMAIN_H
