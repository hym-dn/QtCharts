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

#include <QtCharts/QAreaLegendMarker>
#include <private/qarealegendmarker_p.h>
#include <private/qareaseries_p.h>
#include <QtCharts/QAreaSeries>

QT_CHARTS_BEGIN_NAMESPACE

/*!
    \class QAreaLegendMarker
    \inmodule Qt Charts
    \brief The QAreaLegendMarker class is a legend marker for an area series.

    An area legend marker is related to a QAreaSeries object, so that one area series
    results in one marker.

    \sa QLegend, QAreaSeries
*/

/*!
  \fn virtual LegendMarkerType QAreaLegendMarker::type()
  \reimp
*/

/*!
  \internal
*/
// 构造
QAreaLegendMarker::QAreaLegendMarker(QAreaSeries *series, QLegend *legend, QObject *parent) :
    QLegendMarker(*new QAreaLegendMarkerPrivate(this,series,legend), parent)
{
    d_ptr->updated(); // 更新
}

/*!
    Removes the legend marker for an area series.
*/
// 析构
QAreaLegendMarker::~QAreaLegendMarker()
{
}

/*!
    \internal
*/
// 构造
QAreaLegendMarker::QAreaLegendMarker(QAreaLegendMarkerPrivate &d, QObject *parent) :
    QLegendMarker(d, parent)
{
}

/*!
  \reimp
*/
// 获取所属序列
QAreaSeries* QAreaLegendMarker::series()
{
    Q_D(QAreaLegendMarker);
    return d->m_series;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 构造
QAreaLegendMarkerPrivate::QAreaLegendMarkerPrivate(QAreaLegendMarker *q, QAreaSeries *series, QLegend *legend) :
    QLegendMarkerPrivate(q,legend),
    q_ptr(q), // 所属图例标记
    m_series(series) // 所属序列
{
    // 连接信号、槽
    QObject::connect(m_series->d_func(),SIGNAL(updated()), this, SLOT(updated()));
    QObject::connect(m_series, SIGNAL(nameChanged()), this, SLOT(updated()));
}

// 析构
QAreaLegendMarkerPrivate::~QAreaLegendMarkerPrivate()
{
}

// 获取所属序列
QAreaSeries* QAreaLegendMarkerPrivate::series()
{
    return m_series;
}

// 获取相关对象
QObject* QAreaLegendMarkerPrivate::relatedObject()
{
    return m_series;
}

// 更新
void QAreaLegendMarkerPrivate::updated()
{
    // 更新标记
    bool labelChanged = false;
    bool brushChanged = false;
    // 画刷更新
    if (!m_customBrush && (m_item->brush() != m_series->brush())) {
        m_item->setBrush(m_series->brush());
        brushChanged = true;
    }
    // 标签更新
    if (!m_customLabel && (m_item->label() != m_series->name())) {
        m_item->setLabel(m_series->name());
        labelChanged = true;
    }
    // 更新图例
    invalidateLegend();
    // 发送更新信号
    if (labelChanged)
        emit q_ptr->labelChanged();
    if (brushChanged)
        emit q_ptr->brushChanged();
}

#include "moc_qarealegendmarker.cpp"
#include "moc_qarealegendmarker_p.cpp"

QT_CHARTS_END_NAMESPACE
