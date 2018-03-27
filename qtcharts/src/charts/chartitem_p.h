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

#ifndef CHARTITEM_H
#define CHARTITEM_H

#include <private/chartelement_p.h>
#include <private/chartpresenter_p.h>
#include <QtCharts/private/qchartglobal_p.h>
#include <QtWidgets/QGraphicsItem>

QT_CHARTS_BEGIN_NAMESPACE

// 图表项，负责描述图标中的项目，它是全部图表项的基类
class QT_CHARTS_PRIVATE_EXPORT ChartItem : public ChartElement
{
    Q_OBJECT
    enum ChartItemTypes { AXIS_ITEM = UserType + 1, XYLINE_ITEM }; // 图表项类型
public:

    // 构造
    ChartItem(QAbstractSeriesPrivate *series,QGraphicsItem* item);
    AbstractDomain*  domain() const; // 序列所属区域
    virtual void cleanup(); // 清除断开与当前对象连接的一切槽

public Q_SLOTS:
    virtual void handleDomainUpdated(); // 区域更新响应槽

    QAbstractSeriesPrivate* seriesPrivate() const {return m_series;} // 所属序列私有成员

protected:
    bool m_validData; // 数据是否有效
private:
    QAbstractSeriesPrivate* m_series; // 所属序列私有成员
};

QT_CHARTS_END_NAMESPACE

#endif /* CHARTITEM_H */
