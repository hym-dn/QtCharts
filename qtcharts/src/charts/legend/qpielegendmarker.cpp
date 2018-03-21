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

#include <QtCharts/QPieLegendMarker>
#include <private/qpielegendmarker_p.h>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>

QT_CHARTS_BEGIN_NAMESPACE

/*!
    \class QPieLegendMarker
    \inmodule Qt Charts
    \brief The QPieLegendMarker class is a legend marker for a pie series.

    A pie legend marker is related to QPieSeries. With a pie series, each slice of the pie
    is related to one marker in the legend.

    \sa QLegend, QPieSeries, QPieSlice
*/

/*!
  \fn virtual LegendMarkerType QPieLegendMarker::type()
  \reimp
*/

/*!
  \internal
*/
// 构造
QPieLegendMarker::QPieLegendMarker(QPieSeries *series, QPieSlice *slice, QLegend *legend, QObject *parent) :
    QLegendMarker(*new QPieLegendMarkerPrivate(this,series,slice,legend), parent)
{
    d_ptr->updated(); // 更新
}

/*!
    Removes the legend marker for a pie series.
*/
// 析构
QPieLegendMarker::~QPieLegendMarker()
{
}

/*!
    \internal
*/
// 构造
QPieLegendMarker::QPieLegendMarker(QPieLegendMarkerPrivate &d, QObject *parent) :
    QLegendMarker(d, parent)
{
}

/*!
    \reimp
*/
// 所属序列
QPieSeries* QPieLegendMarker::series()
{
    Q_D(QPieLegendMarker);
    return d->m_series;
}

/*!
    Returns the slice of the pie related to the marker.
*/
// 切分
QPieSlice* QPieLegendMarker::slice()
{
    Q_D(QPieLegendMarker);
    return d->m_slice;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 构造
QPieLegendMarkerPrivate::QPieLegendMarkerPrivate(QPieLegendMarker *q, QPieSeries *series, QPieSlice *slice, QLegend *legend) :
    QLegendMarkerPrivate(q,legend),
    q_ptr(q), // 所属标记
    m_series(series), // 所属序列
    m_slice(slice) // 切分
{
    QObject::connect(m_slice, SIGNAL(labelChanged()), this, SLOT(updated()));
    QObject::connect(m_slice, SIGNAL(brushChanged()), this, SLOT(updated()));
    QObject::connect(m_slice, SIGNAL(penChanged()), this, SLOT(updated()));
}

// 析构
QPieLegendMarkerPrivate::~QPieLegendMarkerPrivate()
{
}

// 所属序列
QPieSeries* QPieLegendMarkerPrivate::series()
{
    return m_series;
}

// 相关对象
QObject* QPieLegendMarkerPrivate::relatedObject()
{
    return m_slice;
}

// 更新
void QPieLegendMarkerPrivate::updated()
{
    // 变更标记
    bool labelChanged = false;
    bool brushChanged = false;
    bool penChanged = false;
    // 画笔更新
    if (!m_customPen && (m_item->pen() != m_slice->pen())) {
        m_item->setPen(m_slice->pen());
        penChanged = true;
    }
    // 画刷更新
    if (!m_customBrush && (m_item->brush() != m_slice->brush())) {
        m_item->setBrush(m_slice->brush());
        brushChanged = true;
    }
    // 标签更新
    if (!m_customLabel && (m_item->label() != m_slice->label())) {
        m_item->setLabel(m_slice->label());
        labelChanged = true;
    }
    // 图例更新
    invalidateLegend();
    // 发送信号
    if (labelChanged)
        emit q_ptr->labelChanged();
    if (brushChanged)
        emit q_ptr->brushChanged();
    if (penChanged)
        emit q_ptr->penChanged();
}

#include "moc_qpielegendmarker.cpp"
#include "moc_qpielegendmarker_p.cpp"

QT_CHARTS_END_NAMESPACE
