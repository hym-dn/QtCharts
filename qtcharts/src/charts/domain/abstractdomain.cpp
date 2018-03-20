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

#include <private/abstractdomain_p.h>
#include <private/qabstractaxis_p.h>
#include <QtCore/QtMath>
#include <cmath>

QT_CHARTS_BEGIN_NAMESPACE

// 构造
AbstractDomain::AbstractDomain(QObject *parent)
    : QObject(parent),
      m_minX(0),
      m_maxX(0),
      m_minY(0),
      m_maxY(0),
      m_signalsBlocked(false),
      m_zoomed(false),
      m_zoomResetMinX(0),
      m_zoomResetMaxX(0),
      m_zoomResetMinY(0),
      m_zoomResetMaxY(0),
      m_reverseX(false),
      m_reverseY(false)

{
}

// 析构
AbstractDomain::~AbstractDomain()
{
}

// 设置尺寸
void AbstractDomain::setSize(const QSizeF &size)
{
    if (m_size != size) {
        m_size=size;
        emit updated(); // 发送更新信息号
    }
}

// 获取区域尺寸
QSizeF AbstractDomain::size() const
{
    return m_size;
}

// 设置x方向范围
void AbstractDomain::setRangeX(qreal min, qreal max)
{
    setRange(min, max, m_minY, m_maxY);
}

// 设置y方向范围
void AbstractDomain::setRangeY(qreal min, qreal max)
{
    setRange(m_minX, m_maxX, min, max);
}

// 设置最小x
void AbstractDomain::setMinX(qreal min)
{
    setRange(min, m_maxX, m_minY, m_maxY);
}

// 设置最大x
void AbstractDomain::setMaxX(qreal max)
{
    setRange(m_minX, max, m_minY, m_maxY);
}

// 设置最小y
void AbstractDomain::setMinY(qreal min)
{
    setRange(m_minX, m_maxX, min, m_maxY);
}

// 设置最大y
void AbstractDomain::setMaxY(qreal max)
{
    setRange(m_minX, m_maxX, m_minY, max);
}

// 获取x方向范围
qreal AbstractDomain::spanX() const
{
    Q_ASSERT(m_maxX >= m_minX);
    return m_maxX - m_minX;
}

// 获取y方向范围
qreal AbstractDomain::spanY() const
{
    Q_ASSERT(m_maxY >= m_minY);
    return m_maxY - m_minY;
}

// 检测区域是否为空
bool AbstractDomain::isEmpty() const
{
    // x范围或y范围或尺寸为空时，区域为空
    return qFuzzyCompare(spanX(), 0) || qFuzzyCompare(spanY(), 0) || m_size.isEmpty();
}

// handlers
// 垂直轴范围变化信号响应槽
void AbstractDomain::handleVerticalAxisRangeChanged(qreal min, qreal max)
{
    setRangeY(min, max);
}

// 水平轴范围变化信号响应槽
void AbstractDomain::handleHorizontalAxisRangeChanged(qreal min, qreal max)
{
    setRangeX(min, max);
}

// 逆向x改变信号响应槽
void AbstractDomain::handleReverseXChanged(bool reverse)
{
    m_reverseX = reverse;
    emit updated();
}

// 逆向y改变信号响应槽
void AbstractDomain::handleReverseYChanged(bool reverse)
{
    m_reverseY = reverse;
    emit updated();
}

// 阻塞范围信号
void AbstractDomain::blockRangeSignals(bool block)
{
    if (m_signalsBlocked!=block) { // 信号阻塞标志改变
        m_signalsBlocked=block; // 赋值
        if (!block) { // 不阻塞
            emit rangeHorizontalChanged(m_minX,m_maxX); // 水平范围改变信号
            emit rangeVerticalChanged(m_minY,m_maxY); // 垂直范围改变信号
        }
    }
}

// 重置缩放
void AbstractDomain::zoomReset()
{
    if (m_zoomed) { // 进行了缩放
        setRange(m_zoomResetMinX,
                 m_zoomResetMaxX,
                 m_zoomResetMinY,
                 m_zoomResetMaxY); // 重置范围
        m_zoomed = false; // 重置标志
    }
}

// 存储缩放重置信息
void AbstractDomain::storeZoomReset()
{
    if (!m_zoomed) { // 当前没有进行缩放
        m_zoomed = true; // 标记缩放
        // 存储重置坐标
        m_zoomResetMinX = m_minX;
        m_zoomResetMaxX = m_maxX;
        m_zoomResetMinY = m_minY;
        m_zoomResetMaxY = m_maxY;
    }
}

//algorithm defined by Paul S.Heckbert GraphicalGems I
// 宽松时合适的数
void AbstractDomain::looseNiceNumbers(qreal &min, qreal &max, int &ticksCount)
{
    qreal range = niceNumber(max - min, true); //range with ceiling 范围合适的数
    qreal step = niceNumber(range / (ticksCount - 1), false); // 步长合适的数
    min = qFloor(min / step);
    max = qCeil(max / step);
    ticksCount = int(max - min) + 1;
    min *= step;
    max *= step;
}

//nice numbers can be expressed as form of 1*10^n, 2* 10^n or 5*10^n
// 合适的数
qreal AbstractDomain::niceNumber(qreal x, bool ceiling)
{
    // 计算最接近x的10^n的值
    qreal z = qPow(10, qFloor(std::log10(x))); //find corresponding number of the form of 10^n than is smaller than x
    // 计算比值
    qreal q = x / z; //q<10 && q>=1;

    if (ceiling) { // 上限
        if (q <= 1.0) q = 1;
        else if (q <= 2.0) q = 2;
        else if (q <= 5.0) q = 5;
        else q = 10;
    } else { // 非上限
        if (q < 1.5) q = 1;
        else if (q < 3.0) q = 2;
        else if (q < 7.0) q = 5;
        else q = 10;
    }
    return q * z;
}

// 捆绑坐标轴
bool AbstractDomain::attachAxis(QAbstractAxis *axis)
{
    if (axis->orientation() == Qt::Vertical) { // 纵向轴
        // 连接信号、槽
        QObject::connect(axis->d_ptr.data(), SIGNAL(rangeChanged(qreal,qreal)), this, SLOT(handleVerticalAxisRangeChanged(qreal,qreal)));
        QObject::connect(this, SIGNAL(rangeVerticalChanged(qreal,qreal)), axis->d_ptr.data(), SLOT(handleRangeChanged(qreal,qreal)));
        QObject::connect(axis, &QAbstractAxis::reverseChanged,
                         this, &AbstractDomain::handleReverseYChanged);
        // 设置逆向y标志
        m_reverseY = axis->isReverse();
    }

    if (axis->orientation() == Qt::Horizontal) { // 水平轴
        // 连接信号、槽
        QObject::connect(axis->d_ptr.data(), SIGNAL(rangeChanged(qreal,qreal)), this, SLOT(handleHorizontalAxisRangeChanged(qreal,qreal)));
        QObject::connect(this, SIGNAL(rangeHorizontalChanged(qreal,qreal)), axis->d_ptr.data(), SLOT(handleRangeChanged(qreal,qreal)));
        QObject::connect(axis, &QAbstractAxis::reverseChanged,
                         this, &AbstractDomain::handleReverseXChanged);
        // 设置逆向x标志
        m_reverseX = axis->isReverse();
    }

    return true;
}

// 松绑坐标轴
bool AbstractDomain::detachAxis(QAbstractAxis *axis)
{
    if (axis->orientation() == Qt::Vertical) { // 垂直轴
        // 断开信号、槽
		QObject::disconnect(axis->d_ptr.data(), SIGNAL(rangeChanged(qreal,qreal)), this, SLOT(handleVerticalAxisRangeChanged(qreal,qreal)));
		QObject::disconnect(this, SIGNAL(rangeVerticalChanged(qreal,qreal)), axis->d_ptr.data(), SLOT(handleRangeChanged(qreal,qreal)));
        QObject::disconnect(axis, &QAbstractAxis::reverseChanged,
                            this, &AbstractDomain::handleReverseYChanged);
    }

    if (axis->orientation() == Qt::Horizontal) { // 水平轴
        // 断开信号、槽
		QObject::disconnect(axis->d_ptr.data(), SIGNAL(rangeChanged(qreal,qreal)), this, SLOT(handleHorizontalAxisRangeChanged(qreal,qreal)));
		QObject::disconnect(this, SIGNAL(rangeHorizontalChanged(qreal,qreal)), axis->d_ptr.data(), SLOT(handleRangeChanged(qreal,qreal)));
        QObject::disconnect(axis, &QAbstractAxis::reverseChanged,
                            this, &AbstractDomain::handleReverseXChanged);
    }

	return true;
}

// operators
// 判断区域是否相同
bool Q_AUTOTEST_EXPORT operator== (const AbstractDomain &domain1, const AbstractDomain &domain2)
{
    return (qFuzzyIsNull(domain1.m_maxX - domain2.m_maxX)
            && qFuzzyIsNull(domain1.m_maxY - domain2.m_maxY)
            && qFuzzyIsNull(domain1.m_minX - domain2.m_minX)
            && qFuzzyIsNull(domain1.m_minY - domain2.m_minY));
}

// 判断区域是否不同
bool Q_AUTOTEST_EXPORT operator!= (const AbstractDomain &domain1, const AbstractDomain &domain2)
{
    return !(domain1 == domain2);
}

// 输出区域
QDebug Q_AUTOTEST_EXPORT operator<<(QDebug dbg, const AbstractDomain &domain)
{
#ifdef QT_NO_TEXTSTREAM
    Q_UNUSED(domain)
#else
    dbg.nospace() << "AbstractDomain(" << domain.m_minX << ',' << domain.m_maxX << ',' << domain.m_minY << ',' << domain.m_maxY << ')' << domain.m_size;
#endif
    return dbg.maybeSpace();
}

// This function adjusts min/max ranges to failsafe values if negative/zero values are attempted.
// 调整Log区域范围
void AbstractDomain::adjustLogDomainRanges(qreal &min, qreal &max)
{
    if (min <= 0) { // 最小值小于0
       min = 1.0;
       if (max <= min)
           max = min + 1.0;
    }
}

// This function fixes the zoom rect based on axis reversals
// 以坐标轴逆向为基础，修复缩放矩形
QRectF AbstractDomain::fixZoomRect(const QRectF &rect)
{
    QRectF fixRect = rect;
    if (m_reverseX || m_reverseY) { // 如果存在逆向
        QPointF center = rect.center(); // 矩形中心
        if (m_reverseX) // x逆向
            center.setX(m_size.width() - center.x());
        if (m_reverseY) // y逆向
            center.setY(m_size.height() - center.y());
        fixRect.moveCenter(QPointF(center.x(), center.y())); // 重置中心
    }

    return fixRect; // 返回
}


#include "moc_abstractdomain_p.cpp"

QT_CHARTS_END_NAMESPACE
