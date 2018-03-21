﻿/****************************************************************************
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

#include <QtCharts/QCandlestickLegendMarker>
#include <private/legendmarkeritem_p.h>
#include <private/qcandlesticklegendmarker_p.h>
#include <private/qcandlestickseries_p.h>

QT_CHARTS_BEGIN_NAMESPACE

/*!
    \class QCandlestickLegendMarker
    \inmodule Qt Charts
    \since 5.8
    \brief The QCandlestickLegendMarker class is a legend marker for a candlestick series.

    QCandlestickLegendMarker is related to QCandlestickSeries, so that one candlestick series
    results in one marker.

    \sa QLegend, QCandlestickSeries
*/

/*!
    \internal
*/
// 构造
QCandlestickLegendMarker::QCandlestickLegendMarker(QCandlestickSeries *series, QLegend *legend,
                                                   QObject *parent)
    : QLegendMarker(*new QCandlestickLegendMarkerPrivate(this, series, legend), parent)
{
    Q_D(QCandlestickLegendMarker);

    d->updated(); // 更新s
}

// 析构
QCandlestickLegendMarker::~QCandlestickLegendMarker()
{
}

/*!
    \reimp
*/
// 类型
QLegendMarker::LegendMarkerType QCandlestickLegendMarker::type()
{
    return LegendMarkerTypeCandlestick;
}

/*!
    \reimp
*/
// 所属序列
QCandlestickSeries* QCandlestickLegendMarker::series()
{
    Q_D(QCandlestickLegendMarker);

    return d->m_series;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

// 构造
QCandlestickLegendMarkerPrivate::QCandlestickLegendMarkerPrivate(QCandlestickLegendMarker *q,
                                                                 QCandlestickSeries *series,
                                                                 QLegend *legend)
    : QLegendMarkerPrivate(q, legend),
      q_ptr(q), // 所属标记
      m_series(series) // 所属序列
{
    // 信号、槽
    QObject::connect(m_item, SIGNAL(markerRectChanged()), this, SLOT(updated()));
    QObject::connect(m_series, SIGNAL(nameChanged()), this, SLOT(updated()));
    QObject::connect(m_series->d_func(), SIGNAL(updated()), this, SLOT(updated()));
}

// 析构
QCandlestickLegendMarkerPrivate::~QCandlestickLegendMarkerPrivate()
{
}

// 所属序列
QAbstractSeries* QCandlestickLegendMarkerPrivate::series()
{
    return m_series;
}

// 相关对象
QObject* QCandlestickLegendMarkerPrivate::relatedObject()
{
    return m_series;
}

// 更新
void QCandlestickLegendMarkerPrivate::updated()
{
    // 标记
    bool labelChanged = false;
    bool brushChanged = false;
    // 标记更新
    if (!m_customLabel && (m_item->label() != m_series->name())) {
        m_item->setLabel(m_series->name());
        labelChanged = true;
    }
    // 画刷更新
    if (!m_customBrush) {
        QLinearGradient gradient;
        gradient.setStart(0.0, 0.0);
        gradient.setFinalStop(m_item->markerRect().width(), m_item->markerRect().height());
        gradient.setColorAt(0.0, m_series->increasingColor());
        gradient.setColorAt(0.49, m_series->increasingColor());
        gradient.setColorAt(0.50, m_series->decreasingColor());
        gradient.setColorAt(1.0, m_series->decreasingColor());

        QBrush brush(gradient);
        if (m_item->brush() != brush) {
            m_item->setBrush(brush);
            brushChanged = true;
        }
    }
    // 图例更新
    invalidateLegend();
    // 发送信号
    if (labelChanged)
        emit q_ptr->labelChanged();
    if (brushChanged)
        emit q_ptr->brushChanged();
}

#include "moc_qcandlesticklegendmarker.cpp"
#include "moc_qcandlesticklegendmarker_p.cpp"

QT_CHARTS_END_NAMESPACE
