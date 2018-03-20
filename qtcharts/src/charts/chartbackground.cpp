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

#include <private/chartbackground_p.h>
#include <private/chartconfig_p.h>
#include <QtGui/QPen>
#include <QtGui/QBrush>
#include <QtGui/QPainter>
#include <QGraphicsDropShadowEffect>

QT_CHARTS_BEGIN_NAMESPACE

// 构造函数
ChartBackground::ChartBackground(QGraphicsItem *parent)
    : QGraphicsRectItem(parent),
      m_diameter(5), // 直径
      m_dropShadow(0) // 下拉阴影效果
{
}

// 析构函数
ChartBackground::~ChartBackground()
{

}

// 设置下拉阴影效果
void ChartBackground::setDropShadowEnabled(bool enabled)
{
#ifdef QT_NO_GRAPHICSEFFECT
    Q_UNUSED(enabled)
#else
    if (enabled) { // 启用
        if (!m_dropShadow) { // 尚未创建
            m_dropShadow = new QGraphicsDropShadowEffect(); // 创建
#ifdef Q_OS_MAC
            m_dropShadow->setBlurRadius(15);
            m_dropShadow->setOffset(0, 0);
#elif defined(Q_OS_WIN)
            m_dropShadow->setBlurRadius(10);
            m_dropShadow->setOffset(0, 0);
#else
            m_dropShadow->setBlurRadius(10); // 设置半径
            m_dropShadow->setOffset(5, 5); // 设置偏移
#endif
            setGraphicsEffect(m_dropShadow); // 设置效果
        }
    } else { // 禁用
        delete m_dropShadow; // 删除
        m_dropShadow = 0; // 置零
    }
#endif
}

// 绘制
void ChartBackground::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->save(); // 存储
    painter->setPen(pen()); // 设置画笔
    painter->setBrush(brush()); // 设置画刷
    painter->drawRoundedRect(rect(), m_diameter, m_diameter); // 绘制
    painter->restore(); // 恢复
}

// 获取绘制直径
qreal ChartBackground::diameter() const
{
    return m_diameter;
}

// 设置绘制直径
void ChartBackground::setDiameter(qreal diameter)
{
    m_diameter = diameter;
    update();
}

QT_CHARTS_END_NAMESPACE
