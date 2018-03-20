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

#include <private/abstractchartlayout_p.h>
#include <private/chartpresenter_p.h>
#include <private/qlegend_p.h>
#include <private/chartaxiselement_p.h>
#include <private/charttitle_p.h>
#include <private/chartbackground_p.h>
#include <QtCore/QDebug>

QT_CHARTS_BEGIN_NAMESPACE

static const qreal golden_ratio = 0.4;

// 构造
AbstractChartLayout::AbstractChartLayout(ChartPresenter *presenter)
    : m_presenter(presenter), // 图表主持
      m_margins(20, 20, 20, 20) // 间隔
{
}

// 析构
AbstractChartLayout::~AbstractChartLayout()
{
}

void AbstractChartLayout::setGeometry(const QRectF &rect)
{
    if (!rect.isValid())
        return;

    if (m_presenter->chart()->isVisible()) {
        QList<ChartAxisElement *> axes = m_presenter->axisItems();
        ChartTitle *title = m_presenter->titleElement();
        QLegend *legend = m_presenter->legend();
        ChartBackground *background = m_presenter->backgroundElement();

        // 计算背景尺寸
        QRectF contentGeometry = calculateBackgroundGeometry(rect, background);

        // 计算内容尺寸
        contentGeometry = calculateContentGeometry(contentGeometry);

        // 计算标题尺寸
        if (title && title->isVisible())
            contentGeometry = calculateTitleGeometry(contentGeometry, title);

        // 计算图例尺寸
        if (legend->isAttachedToChart() && legend->isVisible())
            contentGeometry = calculateLegendGeometry(contentGeometry, legend);

        // 计算坐标轴尺寸
        contentGeometry = calculateAxisGeometry(contentGeometry, axes);

        // 存储几何尺寸
        m_presenter->setGeometry(contentGeometry);
        if (m_presenter->chart()->chartType() == QChart::ChartTypeCartesian) // 笛卡尔坐标
            static_cast<QGraphicsRectItem *>(m_presenter->plotAreaElement())->setRect(contentGeometry);
        else // 极坐标
            static_cast<QGraphicsEllipseItem *>(m_presenter->plotAreaElement())->setRect(contentGeometry);
    }

    // 设置布局尺寸
    QGraphicsLayout::setGeometry(rect);
}

// 计算内容几何尺寸
QRectF AbstractChartLayout::calculateContentGeometry(const QRectF &geometry) const
{
    return geometry.adjusted(m_margins.left(), m_margins.top(), -m_margins.right(), -m_margins.bottom());
}

// 计算内容最小尺寸
QRectF AbstractChartLayout::calculateContentMinimum(const QRectF &minimum) const
{
    return  minimum.adjusted(0, 0, m_margins.left() + m_margins.right(), m_margins.top() + m_margins.bottom());
}

// 计算背景几何尺寸
QRectF AbstractChartLayout::calculateBackgroundGeometry(const QRectF &geometry, ChartBackground *background) const
{
    qreal left;
    qreal top;
    qreal right;
    qreal bottom;
    getContentsMargins(&left, &top, &right, &bottom); // 基类函数，获取上、下、左、右间隔
    QRectF backgroundGeometry = geometry.adjusted(left, top, -right, -bottom); // 调整尺寸
    if (background) // 设置背景尺寸
        background->setRect(backgroundGeometry); // 更新背景尺寸
    return backgroundGeometry; // 返回背景几何尺寸
}

// 计算背景最小尺寸
QRectF AbstractChartLayout::calculateBackgroundMinimum(const QRectF &minimum) const
{
    qreal left;
    qreal top;
    qreal right;
    qreal bottom;
    getContentsMargins(&left, &top, &right, &bottom);
    return minimum.adjusted(0, 0, left + right, top + bottom);
}

// 计算图例几何尺寸
QRectF AbstractChartLayout::calculateLegendGeometry(const QRectF &geometry, QLegend *legend) const
{
    QSizeF size = legend->effectiveSizeHint(Qt::PreferredSize, QSizeF(-1, -1));
    QRectF legendRect;
    QRectF result;

    switch (legend->alignment()) {
    case Qt::AlignTop: {
        legendRect = QRectF(geometry.topLeft(), QSizeF(geometry.width(), size.height()));
        result = geometry.adjusted(0, legendRect.height(), 0, 0);
        break;
    }
    case Qt::AlignBottom: {
        legendRect = QRectF(QPointF(geometry.left(), geometry.bottom() - size.height()), QSizeF(geometry.width(), size.height()));
        result = geometry.adjusted(0, 0, 0, -legendRect.height());
        break;
    }
    case Qt::AlignLeft: {
        qreal width = qMin(size.width(), geometry.width() * golden_ratio);
        legendRect = QRectF(geometry.topLeft(), QSizeF(width, geometry.height()));
        result = geometry.adjusted(width, 0, 0, 0);
        break;
    }
    case Qt::AlignRight: {
        qreal width = qMin(size.width(), geometry.width() * golden_ratio);
        legendRect = QRectF(QPointF(geometry.right() - width, geometry.top()), QSizeF(width, geometry.height()));
        result = geometry.adjusted(0, 0, -width, 0);
        break;
    }
    default: {
        legendRect = QRectF(0, 0, 0, 0);
        result = geometry;
        break;
    }
    }

    legend->setGeometry(legendRect);

    return result;
}

// 计算图例最小尺寸
QRectF AbstractChartLayout::calculateLegendMinimum(const QRectF &geometry, QLegend *legend) const
{
    if (!legend->isAttachedToChart() || !legend->isVisible()) {
        return geometry;
    } else {
        QSizeF minSize = legend->effectiveSizeHint(Qt::MinimumSize, QSizeF(-1, -1));
        return geometry.adjusted(0, 0, minSize.width(), minSize.height());
    }
}

// 计算标题几何尺寸
QRectF AbstractChartLayout::calculateTitleGeometry(const QRectF &geometry, ChartTitle *title) const
{
    title->setGeometry(geometry); // 更新标题尺寸
    if (title->text().isEmpty()) { // 标题为空
        return geometry;
    } else { // 标题非空
        // Round to full pixel via QPoint to avoid one pixel clipping on the edge in some cases
        QPointF center((geometry.center() - title->boundingRect().center()).toPoint());
        title->setPos(center.x(), title->pos().y()); // 设置标题位置
        return geometry.adjusted(0, title->boundingRect().height() + 1, 0, 0); // 返回几何尺寸
    }
}

// 计算标题最小尺寸
QRectF AbstractChartLayout::calculateTitleMinimum(const QRectF &minimum, ChartTitle *title) const
{
    if (!title->isVisible() || title->text().isEmpty()) {
        return minimum;
    } else {
        QSizeF min = title->sizeHint(Qt::MinimumSize);
        return  minimum.adjusted(0, 0, min.width(), min.height());
    }
}

QSizeF AbstractChartLayout::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    Q_UNUSED(constraint);
    if (which == Qt::MinimumSize) {
        QList<ChartAxisElement *> axes = m_presenter->axisItems();
        ChartTitle *title = m_presenter->titleElement();
        QLegend *legend = m_presenter->legend();
        QRectF minimumRect(0, 0, 0, 0);
        minimumRect = calculateBackgroundMinimum(minimumRect);
        minimumRect = calculateContentMinimum(minimumRect);
        minimumRect = calculateTitleMinimum(minimumRect, title);
        minimumRect = calculateLegendMinimum(minimumRect, legend);
        minimumRect = calculateAxisMinimum(minimumRect, axes);
        return  minimumRect.size().toSize();
    }
    return QSize(-1, -1);
}

void AbstractChartLayout::setMargins(const QMargins &margins)
{
    if (m_margins != margins) {
        m_margins = margins;
        updateGeometry();
    }
}

QMargins AbstractChartLayout::margins() const
{
    return m_margins;
}

QT_CHARTS_END_NAMESPACE
