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

//  W A R N I N G
//  -------------
//
// This file is not part of the Qt Chart API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.

#ifndef QCANDLESTICKLEGENDMARKER_P_H
#define QCANDLESTICKLEGENDMARKER_P_H

#include <private/qlegendmarker_p.h>
#include <QtCharts/private/qchartglobal_p.h>

QT_CHARTS_BEGIN_NAMESPACE

class QCandlestickLegendMarker;
class QCandlestickSeries;

// 烛台图例标记私有成员
class QT_CHARTS_PRIVATE_EXPORT QCandlestickLegendMarkerPrivate : public QLegendMarkerPrivate
{
    Q_OBJECT
public:
    explicit QCandlestickLegendMarkerPrivate(QCandlestickLegendMarker *q,
                                             QCandlestickSeries *series, QLegend *legend); // 构造
    virtual ~QCandlestickLegendMarkerPrivate(); // 析构

    virtual QAbstractSeries *series(); // 所属序列
    virtual QObject *relatedObject(); // 相关对象

public Q_SLOTS:
    virtual void updated(); // 更新

private:
    QCandlestickLegendMarker *q_ptr; // 所属烛台标记
    QCandlestickSeries *m_series; // 所属序列

    Q_DECLARE_PUBLIC(QCandlestickLegendMarker)
};

QT_CHARTS_END_NAMESPACE

#endif // QCANDLESTICKLEGENDMARKER_P_H
