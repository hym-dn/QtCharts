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

#ifndef SCROLLER_P_H
#define SCROLLER_P_H

#include <QtCharts/QChartGlobal>
#include <QtCore/QObject>
#include <QtCore/QBasicTimer>
#include <QtCore/QTime>
#include <QtCore/QPointF>
#include <QtCharts/private/qchartglobal_p.h>

QT_BEGIN_NAMESPACE
class QGraphicsSceneMouseEvent;
QT_END_NAMESPACE

QT_CHARTS_BEGIN_NAMESPACE

class Scroller;
class QLegend;

// 滚动追踪器
class QT_CHARTS_PRIVATE_EXPORT ScrollTicker : public QObject
{
    Q_OBJECT
public:
    explicit ScrollTicker(Scroller *scroller, QObject *parent = 0); // 构造
    void start(int interval); // 启动
    void stop(); // 停止
protected:
    void timerEvent(QTimerEvent *event); // 定时器

private:
    QBasicTimer m_timer; // 基础定时器
    Scroller *m_scroller; // 所属滚动类s
};

// 滚动类，实现图表中滚动操作
class QT_CHARTS_PRIVATE_EXPORT Scroller
{
public:
    enum State {
        Idle,
        Pressed,
        Move,
        Scroll
    }; // 状态

    Scroller(); // 构造
    virtual ~Scroller(); // 析构

    virtual void setOffset(const QPointF &point) = 0; // 设置偏移
    virtual QPointF offset() const = 0; // 获取偏移

    void move(const QPointF &delta); // 移动
    void scrollTo(const QPointF &delta); // 滚动到

    void handleMousePressEvent(QGraphicsSceneMouseEvent *event); // 鼠标按下事件
    void handleMouseMoveEvent(QGraphicsSceneMouseEvent *event); // 鼠标移动事件
    void handleMouseReleaseEvent(QGraphicsSceneMouseEvent *event); // 鼠标释放事件

    void scrollTick(); // 滚动追踪

private:
    void startTicker(int interval); // 启动追踪
    void stopTicker(); // 停止追踪

private:
    void calculateSpeed(const QPointF &position); // 计算速度
    void lowerSpeed(QPointF &speed, qreal maxSpeed = 100); // 降低速度

private:
    ScrollTicker m_ticker; // 滚动追踪
    QTime m_timeStamp; // 时间戳
    QPointF m_speed; // 速度
    QPointF m_fraction; // 小数
    int m_timeTresholdMin; // 最小时间阈值
    int m_timeTresholdMax; // 最大时间阈值

    State m_state; // 状态
    QPointF m_pressPos; // 按压位置
    QPointF m_lastPos; // 最后位置
    qreal m_treshold; // 阈值
};

QT_CHARTS_END_NAMESPACE

#endif /* SCROLLER_P_H */
