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

#ifndef QCANDLESTICKLEGENDMARKER_H
#define QCANDLESTICKLEGENDMARKER_H

#include <QtCharts/QCandlestickSeries>
#include <QtCharts/QLegendMarker>

QT_CHARTS_BEGIN_NAMESPACE

class QCandlestickLegendMarkerPrivate;

// 烛台图例标记
class QT_CHARTS_EXPORT QCandlestickLegendMarker : public QLegendMarker
{
    Q_OBJECT

public:
    explicit QCandlestickLegendMarker(QCandlestickSeries *series, QLegend *legend,
                                      QObject *parent = nullptr); // 构造
    virtual ~QCandlestickLegendMarker(); // 析构

    virtual LegendMarkerType type(); // 图例标记类型

    // Related series
    virtual QCandlestickSeries* series(); // 所属序列

private:
    Q_DECLARE_PRIVATE(QCandlestickLegendMarker)
    Q_DISABLE_COPY(QCandlestickLegendMarker)
};

QT_CHARTS_END_NAMESPACE

#endif // QCANDLESTICKLEGENDMARKER_H
