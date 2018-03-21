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

#include <QtCharts/QBoxPlotLegendMarker>
#include <private/qboxplotlegendmarker_p.h>
#include <QtCharts/QBoxPlotSeries>
#include <private/qboxplotseries_p.h>

QT_CHARTS_BEGIN_NAMESPACE

/*!
    \class QBoxPlotLegendMarker
    \inmodule Qt Charts
    \brief The QBoxPlotLegendMarker class is a legend marker for a box plot series.

    A box plot legend marker is related to a QBoxPlotSeries object.

    \sa QLegend, QBoxPlotSeries
*/

/*!
  \fn virtual LegendMarkerType QBoxPlotLegendMarker::type()
  \reimp
*/

/*!
  \internal
*/
// 构造
QBoxPlotLegendMarker::QBoxPlotLegendMarker(QBoxPlotSeries *series, QLegend *legend, QObject *parent) :
    QLegendMarker(*new QBoxPlotLegendMarkerPrivate(this,series,legend), parent)
{
    d_ptr->updated(); // 更新
}

/*!
    Removes the legend marker for a box plot series.
*/
// 析构
QBoxPlotLegendMarker::~QBoxPlotLegendMarker()
{
}

/*!
    \internal
*/
// 构造
QBoxPlotLegendMarker::QBoxPlotLegendMarker(QBoxPlotLegendMarkerPrivate &d, QObject *parent) :
    QLegendMarker(d, parent)
{
}

/*!
    \reimp
*/
// 获取所属序列
QBoxPlotSeries* QBoxPlotLegendMarker::series()
{
    Q_D(QBoxPlotLegendMarker);
    return d->m_series;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 构造
QBoxPlotLegendMarkerPrivate::QBoxPlotLegendMarkerPrivate(QBoxPlotLegendMarker *q, QBoxPlotSeries *series, QLegend *legend) :
    QLegendMarkerPrivate(q,legend),
    q_ptr(q), // 所属图列标记
    m_series(series) // 所属序列
{
    // 信号、槽
    QObject::connect(m_series, SIGNAL(nameChanged()), this, SLOT(updated()));
    QObject::connect(m_series->d_func(), SIGNAL(updated()), this, SLOT(updated()));
}

// 析构
QBoxPlotLegendMarkerPrivate::~QBoxPlotLegendMarkerPrivate()
{
}

// 所属序列
QAbstractSeries* QBoxPlotLegendMarkerPrivate::series()
{
    return m_series;
}

// 相关对象
QObject* QBoxPlotLegendMarkerPrivate::relatedObject()
{
    return m_series;
}

// 更新
void QBoxPlotLegendMarkerPrivate::updated()
{
    // 更新标记
    bool labelChanged = false;
    bool brushChanged = false;
    // 更新标签
    if (!m_customLabel && (m_item->label() != m_series->name())) {
        m_item->setLabel(m_series->name());
        labelChanged = true;
    }
    // 更新画刷
    if (!m_customBrush && (m_item->brush() != m_series->brush())) {
        m_item->setBrush(m_series->brush());
        brushChanged = true;
    }
    // 更新图例
    invalidateLegend();
    // 发送信号
    if (labelChanged)
        emit q_ptr->labelChanged();
    if (brushChanged)
        emit q_ptr->brushChanged();
}

#include "moc_qboxplotlegendmarker.cpp"
#include "moc_qboxplotlegendmarker_p.cpp"

QT_CHARTS_END_NAMESPACE

