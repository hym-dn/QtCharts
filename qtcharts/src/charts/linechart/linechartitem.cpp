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

#include <private/linechartitem_p.h>
#include <QtCharts/QLineSeries>
#include <private/qlineseries_p.h>
#include <private/chartpresenter_p.h>
#include <private/polardomain_p.h>
#include <private/chartthememanager_p.h>
#include <private/charttheme_p.h>
#include <QtGui/QPainter>
#include <QtWidgets/QGraphicsSceneMouseEvent>

QT_CHARTS_BEGIN_NAMESPACE

// 构造
LineChartItem::LineChartItem(QLineSeries *series, QGraphicsItem *item)
    : XYChart(series,item), // 基类构造函数
      m_series(series), // 所属序列
      m_pointsVisible(false), // 点集是否可见
      m_chartType(QChart::ChartTypeUndefined), // 图表类型
      m_pointLabelsVisible(false), // 点标签是否可见
      m_pointLabelsFormat(series->pointLabelsFormat()), // 点标签格式
      m_pointLabelsFont(series->pointLabelsFont()), // 点标签字体
      m_pointLabelsColor(series->pointLabelsColor()), // 点标签颜色
      m_pointLabelsClipping(true), // 点标签裁剪
      m_mousePressed(false) // 鼠标是否按下
{
    // 接收鼠标在其上事件
    setAcceptHoverEvents(true);
    // 设置标志
    setFlag(QGraphicsItem::ItemIsSelectable);
    // 设置z轴坐标
    setZValue(ChartPresenter::LineChartZValue);
    // 绑定信号、槽
    QObject::connect(series->d_func(), SIGNAL(updated()), this, SLOT(handleUpdated()));
    QObject::connect(series, SIGNAL(visibleChanged()), this, SLOT(handleUpdated()));
    QObject::connect(series, SIGNAL(opacityChanged()), this, SLOT(handleUpdated()));
    QObject::connect(series, SIGNAL(pointLabelsFormatChanged(QString)),
                     this, SLOT(handleUpdated()));
    QObject::connect(series, SIGNAL(pointLabelsVisibilityChanged(bool)),
                     this, SLOT(handleUpdated()));
    QObject::connect(series, SIGNAL(pointLabelsFontChanged(QFont)), this, SLOT(handleUpdated()));
    QObject::connect(series, SIGNAL(pointLabelsColorChanged(QColor)), this, SLOT(handleUpdated()));
    QObject::connect(series, SIGNAL(pointLabelsClippingChanged(bool)), this, SLOT(handleUpdated()));
    // 更新
    handleUpdated();
}

// 外接矩形
QRectF LineChartItem::boundingRect() const
{
    return m_rect;
}

// 形状路径
QPainterPath LineChartItem::shape() const
{
    return m_shapePath;
}

// 更新几何(更新路径)
void LineChartItem::updateGeometry()
{
    // 是否使用 OpenGL
    if (m_series->useOpenGL()) {
        // 矩形为空
        if (!m_rect.isEmpty()) {
            // 准备更新
            prepareGeometryChange();
            // Changed signal seems to trigger even with empty region
            // 记录矩形
            m_rect = QRectF();
        }
        // 更新
        update();
        // 返回
        return;
    }

    // Store the points to a local variable so that the old line gets properly cleared
    // when animation starts.
    // 获取线的几何点集
    m_linePoints = geometryPoints();
    // 存储线的几何点集
    const QVector<QPointF> &points = m_linePoints;

    // 点集为空
    if (points.size() == 0) {
        prepareGeometryChange(); // 准备几何变更
        m_fullPath = QPainterPath(); // 全部路径
        m_linePath = QPainterPath(); // 线路径
        m_rect = QRect(); // 矩形
        return;
    }


    QPainterPath linePath;
    QPainterPath fullPath;
    // Use worst case scenario to determine required margin.
    // 留白
    qreal margin = m_linePen.width() * 1.42;

    // Area series use component line series that aren't necessarily added to the chart themselves,
    // so check if chart type is forced before trying to obtain it from the chart.
    // 图表类型
    QChart::ChartType chartType = m_chartType;
    if (chartType == QChart::ChartTypeUndefined) // 图表类型未定义
        chartType = m_series->chart()->chartType();

    // For polar charts, we need special handling for angular (horizontal)
    // points that are off-grid.
    // 极坐标
    if (chartType == QChart::ChartTypePolar) {
        QPainterPath linePathLeft;
        QPainterPath linePathRight;
        QPainterPath *currentSegmentPath = 0;
        QPainterPath *previousSegmentPath = 0;
        // 最小x、最大y
        qreal minX = domain()->minX();
        qreal maxX = domain()->maxX();
        qreal minY = domain()->minY();
        // 第一个序列点
        QPointF currentSeriesPoint = m_series->at(0);
        // 当前几何点
        QPointF currentGeometryPoint = points.at(0);
        // 前一个几何点
        QPointF previousGeometryPoint = points.at(0);
        // 线宽度
        int size = m_linePen.width();
        // 点超出范围标记
        bool pointOffGrid = false;
        bool previousPointWasOffGrid = (currentSeriesPoint.x() < minX || currentSeriesPoint.x() > maxX);
        // 半径
        qreal domainRadius = domain()->size().height() / 2.0;
        // 中心点
        const QPointF centerPoint(domainRadius, domainRadius);
        // 未超出范围
        if (!previousPointWasOffGrid) {
            fullPath.moveTo(points.at(0)); // 路径移动
            if (m_pointsVisible && currentSeriesPoint.y() >= minY) {
                // Do not draw ellipses for points below minimum Y.
                linePath.addEllipse(points.at(0), size, size); // 线路径
                fullPath.addEllipse(points.at(0), size, size); // 全部路径
                linePath.moveTo(points.at(0)); // 设置中点
                fullPath.moveTo(points.at(0)); // 设置中点
            }
        }

        // 记录左右留白
        qreal leftMarginLine = centerPoint.x() - margin;
        qreal rightMarginLine = centerPoint.x() + margin;
        // 记录水平高度
        qreal horizontal = centerPoint.y();

        // See ScatterChartItem::updateGeometry() for explanation why seriesLastIndex is needed
        // 计算序列中最后一个索引
        const int seriesLastIndex = m_series->count() - 1;

        // 遍历几何点集
        for (int i = 1; i < points.size(); i++) {
            // Interpolating line fragments would be ugly when thick pen is used,
            // so we work around it by utilizing three separate
            // paths for line segments and clip those with custom regions at paint time.
            // "Right" path contains segments that cross the axis line with visible point on the
            // right side of the axis line, as well as segments that have one point within the margin
            // on the right side of the axis line and another point on the right side of the chart.
            // "Left" path contains points with similarly on the left side.
            // "Full" path contains rest of the points.
            // This doesn't yield perfect results always. E.g. when segment covers more than 90
            // degrees and both of the points are within the margin, one in the top half and one in the
            // bottom half of the chart, the bottom one gets clipped incorrectly.
            // However, this should be rare occurrence in any sensible chart.
            // 获取当前序列点
            currentSeriesPoint = m_series->at(qMin(seriesLastIndex, i));
            // 获取当前几何点
            currentGeometryPoint = points.at(i);
            // x是否超出范围
            pointOffGrid = (currentSeriesPoint.x() < minX || currentSeriesPoint.x() > maxX);

            // Draw something unless both off-grid
            // 当前点、前一点没有超出范围
            if (!pointOffGrid || !previousPointWasOffGrid) {
                QPointF intersectionPoint;
                qreal y;
                if (pointOffGrid != previousPointWasOffGrid) {
                    if (currentGeometryPoint.x() == previousGeometryPoint.x()) {
                        y = currentGeometryPoint.y() + (currentGeometryPoint.y() - previousGeometryPoint.y()) / 2.0;
                    } else {
                        qreal ratio = (centerPoint.x() - currentGeometryPoint.x()) / (currentGeometryPoint.x() - previousGeometryPoint.x());
                        y = currentGeometryPoint.y() + (currentGeometryPoint.y() - previousGeometryPoint.y()) * ratio;
                    }
                    intersectionPoint = QPointF(centerPoint.x(), y);
                }

                bool dummyOk; // We know points are ok, but this is needed
                qreal currentAngle = 0;
                qreal previousAngle = 0;
                if (const PolarDomain *pd = qobject_cast<const PolarDomain *>(domain())) {
                    currentAngle = pd->toAngularCoordinate(currentSeriesPoint.x(), dummyOk);
                    previousAngle = pd->toAngularCoordinate(m_series->at(i - 1).x(), dummyOk);
                } else {
                    qWarning() << Q_FUNC_INFO << "Unexpected domain: " << domain();
                }
                if ((qAbs(currentAngle - previousAngle) > 180.0)) {
                    // If the angle between two points is over 180 degrees (half X range),
                    // any direct segment between them becomes meaningless.
                    // In this case two line segments are drawn instead, from previous
                    // point to the center and from center to current point.
                    if ((previousAngle < 0.0 || (previousAngle <= 180.0 && previousGeometryPoint.x() < rightMarginLine))
                        && previousGeometryPoint.y() < horizontal) {
                        currentSegmentPath = &linePathRight;
                    } else if ((previousAngle > 360.0 || (previousAngle > 180.0 && previousGeometryPoint.x() > leftMarginLine))
                                && previousGeometryPoint.y() < horizontal) {
                        currentSegmentPath = &linePathLeft;
                    } else if (previousAngle > 0.0 && previousAngle < 360.0) {
                        currentSegmentPath = &linePath;
                    } else {
                        currentSegmentPath = 0;
                    }

                    if (currentSegmentPath) {
                        if (previousSegmentPath != currentSegmentPath)
                            currentSegmentPath->moveTo(previousGeometryPoint);
                        if (previousPointWasOffGrid)
                            fullPath.moveTo(intersectionPoint);

                        currentSegmentPath->lineTo(centerPoint);
                        fullPath.lineTo(centerPoint);
                    }

                    previousSegmentPath = currentSegmentPath;

                    if ((currentAngle < 0.0 || (currentAngle <= 180.0 && currentGeometryPoint.x() < rightMarginLine))
                        && currentGeometryPoint.y() < horizontal) {
                        currentSegmentPath = &linePathRight;
                    } else if ((currentAngle > 360.0 || (currentAngle > 180.0 &&currentGeometryPoint.x() > leftMarginLine))
                                && currentGeometryPoint.y() < horizontal) {
                        currentSegmentPath = &linePathLeft;
                    } else if (currentAngle > 0.0 && currentAngle < 360.0) {
                        currentSegmentPath = &linePath;
                    } else {
                        currentSegmentPath = 0;
                    }

                    if (currentSegmentPath) {
                        if (previousSegmentPath != currentSegmentPath)
                            currentSegmentPath->moveTo(centerPoint);
                        if (!previousSegmentPath)
                            fullPath.moveTo(centerPoint);

                        currentSegmentPath->lineTo(currentGeometryPoint);
                        if (pointOffGrid)
                            fullPath.lineTo(intersectionPoint);
                        else
                            fullPath.lineTo(currentGeometryPoint);
                    }
                } else {
                    if (previousAngle < 0.0 || currentAngle < 0.0
                        || ((previousAngle <= 180.0 && currentAngle <= 180.0)
                            && ((previousGeometryPoint.x() < rightMarginLine && previousGeometryPoint.y() < horizontal)
                                || (currentGeometryPoint.x() < rightMarginLine && currentGeometryPoint.y() < horizontal)))) {
                        currentSegmentPath = &linePathRight;
                    } else if (previousAngle > 360.0 || currentAngle > 360.0
                               || ((previousAngle > 180.0 && currentAngle > 180.0)
                                   && ((previousGeometryPoint.x() > leftMarginLine && previousGeometryPoint.y() < horizontal)
                                       || (currentGeometryPoint.x() > leftMarginLine && currentGeometryPoint.y() < horizontal)))) {
                        currentSegmentPath = &linePathLeft;
                    } else {
                        currentSegmentPath = &linePath;
                    }

                    if (currentSegmentPath != previousSegmentPath)
                        currentSegmentPath->moveTo(previousGeometryPoint);
                    if (previousPointWasOffGrid)
                        fullPath.moveTo(intersectionPoint);

                    if (pointOffGrid)
                        fullPath.lineTo(intersectionPoint);
                    else
                        fullPath.lineTo(currentGeometryPoint);
                    currentSegmentPath->lineTo(currentGeometryPoint);
                }
            } else {
                currentSegmentPath = 0;
            }

            previousPointWasOffGrid = pointOffGrid;
            if (m_pointsVisible && !pointOffGrid && currentSeriesPoint.y() >= minY) {
                linePath.addEllipse(points.at(i), size, size);
                fullPath.addEllipse(points.at(i), size, size);
                linePath.moveTo(points.at(i));
                fullPath.moveTo(points.at(i));
            }
            previousSegmentPath = currentSegmentPath;
            previousGeometryPoint = currentGeometryPoint;
        }
        m_linePathPolarRight = linePathRight;
        m_linePathPolarLeft = linePathLeft;
        // Note: This construction of m_fullpath is not perfect. The partial segments that are
        // outside left/right clip regions at axis boundary still generate hover/click events,
        // because shape doesn't get clipped. It doesn't seem possible to do sensibly.
    }
    // 非极坐标
    else { // not polar
        // 开启新路径
        linePath.moveTo(points.at(0));
        // 如果点可见
        if (m_pointsVisible) {
            int size = m_linePen.width(); // 记录点的宽度
            linePath.addEllipse(points.at(0), size, size); // 增加一个圆
            linePath.moveTo(points.at(0)); // 增加一个路径
            for (int i = 1; i < points.size(); i++) { // 遍历点，更新路径
                linePath.lineTo(points.at(i));
                linePath.addEllipse(points.at(i), size, size);
                linePath.moveTo(points.at(i));
            }
        }
        // 如果点不可见
        else {
            for (int i = 1; i < points.size(); i++)
                linePath.lineTo(points.at(i)); // 增加路径
        }
        // 存储路径
        fullPath = linePath;
    }

    // 创建可填充轮廓
    QPainterPathStroker stroker;
    // QPainter::drawLine does not respect join styles, for example BevelJoin becomes MiterJoin.
    // This is why we are prepared for the "worst case" scenario, i.e. use always MiterJoin and
    // multiply line width with square root of two when defining shape and bounding rectangle.
    stroker.setWidth(margin);
    stroker.setJoinStyle(Qt::MiterJoin);
    stroker.setCapStyle(Qt::SquareCap);
    stroker.setMiterLimit(m_linePen.miterLimit());
    // 生成新的可填充路径
    QPainterPath checkShapePath = stroker.createStroke(fullPath);

    // Only zoom in if the bounding rects of the paths fit inside int limits. QWidget::update() uses
    // a region that has to be compatible with QRect.
    // 路径无效
    if (checkShapePath.boundingRect().height() <= INT_MAX
            && checkShapePath.boundingRect().width() <= INT_MAX
            && linePath.boundingRect().height() <= INT_MAX
            && linePath.boundingRect().width() <= INT_MAX
            && fullPath.boundingRect().height() <= INT_MAX
            && fullPath.boundingRect().width() <= INT_MAX) {
        // 准备变更
        prepareGeometryChange();

        // 存储路径
        m_linePath = linePath;
        m_fullPath = fullPath;
        m_shapePath = checkShapePath;

        // 存储矩形
        m_rect = m_shapePath.boundingRect();
    }
    // 路径有效
    else {

        // 更新
        update();
    }
}

// 更新信号响应槽
void LineChartItem::handleUpdated()
{
    // If points visibility has changed, a geometry update is needed.
    // Also, if pen changes when points are visible, geometry update is needed.
    // 几何更新
    bool doGeometryUpdate =
        (m_pointsVisible != m_series->pointsVisible()) // 可见与否发生变更
        || (m_series->pointsVisible() && (m_linePen != m_series->pen())); // 可见并且画笔变化
    // 序列可见是否变更
    bool visibleChanged = m_series->isVisible() != isVisible();
    // 设置可见
    setVisible(m_series->isVisible());
    // 设置不透明度
    setOpacity(m_series->opacity());
    // 点集是否可见
    m_pointsVisible = m_series->pointsVisible();
    // 序列画笔
    m_linePen = m_series->pen();
    // 序列标签格式
    m_pointLabelsFormat = m_series->pointLabelsFormat();
    // 序列标签是否可见
    m_pointLabelsVisible = m_series->pointLabelsVisible();
    // 标签字体
    m_pointLabelsFont = m_series->pointLabelsFont();
    // 标签颜色
    m_pointLabelsColor = m_series->pointLabelsColor();
    // 标签是否检测
    m_pointLabelsClipping = m_series->pointLabelsClipping();
    // 是否更新几何
    if (doGeometryUpdate)
        updateGeometry(); // 更新几何
    // 是否使用OpenGL，并且可见
    else if (m_series->useOpenGL() && visibleChanged)
        refreshGlChart(); // 刷新OpenGL表
    // 更新
    update();
}

// 绘制
void LineChartItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget)
    Q_UNUSED(option)

    // 使用OpenGL
    if (m_series->useOpenGL())
        return; // 返回

    // 剪裁矩形
    QRectF clipRect = QRectF(QPointF(0, 0), domain()->size());
    // Adjust clip rect half a pixel in required dimensions to make it include lines along the
    // plot area edges, but never increase clip so much that any portion of the line is draw beyond
    // the plot area.
    // 调整剪裁矩形
    const qreal x1 = pos().x() - int(pos().x());
    const qreal y1 = pos().y() - int(pos().y());
    const qreal x2 = (clipRect.width() + 0.5) - int(clipRect.width() + 0.5);
    const qreal y2 = (clipRect.height() + 0.5) - int(clipRect.height() + 0.5);
    clipRect.adjust(-x1, -y1, qMax(x1, x2), qMax(y1, y2));
    // 保存绘图场景
    painter->save();
    // 设置画笔
    painter->setPen(m_linePen);
    // 总是使用路径标记
    bool alwaysUsePath = false;
    // 极坐标
    if (m_series->chart()->chartType() == QChart::ChartTypePolar) {
        qreal halfWidth = domain()->size().width() / 2.0;
        QRectF clipRectLeft = QRectF(0, 0, halfWidth, domain()->size().height());
        QRectF clipRectRight = QRectF(halfWidth, 0, halfWidth, domain()->size().height());
        QRegion fullPolarClipRegion(clipRect.toRect(), QRegion::Ellipse);
        QRegion clipRegionLeft(fullPolarClipRegion.intersected(clipRectLeft.toRect()));
        QRegion clipRegionRight(fullPolarClipRegion.intersected(clipRectRight.toRect()));
        painter->setClipRegion(clipRegionLeft); // 设置剪切区域
        painter->drawPath(m_linePathPolarLeft); // 绘制区域
        painter->setClipRegion(clipRegionRight); // 设置剪切区域
        painter->drawPath(m_linePathPolarRight); // 绘制区域
        painter->setClipRegion(fullPolarClipRegion); // 设置剪切区域
        alwaysUsePath = true; // required for proper clipping
    }
    // 非极坐标
    else {
        painter->setClipRect(clipRect); // 设置剪切矩形
    }

    // 点集可视
    if (m_pointsVisible) {
        painter->setBrush(m_linePen.color()); // 设置画刷
        painter->drawPath(m_linePath); // 绘制路径
    }
    // 点集不可视
    else {
        // 设置画刷
        painter->setBrush(QBrush(Qt::NoBrush));
        // 画笔风格
        if (m_linePen.style() != Qt::SolidLine || alwaysUsePath) {
            // If pen style is not solid line, always fall back to path painting
            // to ensure proper continuity of the pattern
            painter->drawPath(m_linePath); // 绘制线路径
        } else {
            for (int i(1); i < m_linePoints.size(); i++)
                painter->drawLine(m_linePoints.at(i - 1), m_linePoints.at(i)); // 绘制线
        }
    }

    // 点标签可视
    if (m_pointLabelsVisible) {
        // 标签可剪裁
        if (m_pointLabelsClipping)
            painter->setClipping(true);
        // 标签不可剪裁
        else
            painter->setClipping(false);
        // 绘制序列点标签
        m_series->d_func()->drawSeriesPointLabels(painter, m_linePoints, m_linePen.width() / 2);
    }

    // 恢复绘图场景
    painter->restore();

}

// 鼠标按下事件
void LineChartItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    // 发送信号
    emit XYChart::pressed(domain()->calculateDomainPoint(event->pos()));
    // 鼠标位置
    m_lastMousePos = event->pos();
    // 按下标记
    m_mousePressed = true;
    // 调用基类相应处理函数
    QGraphicsItem::mousePressEvent(event);
}

// 鼠标进入事件
void LineChartItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    // 发送信号
    emit XYChart::hovered(domain()->calculateDomainPoint(event->pos()), true);
//    event->accept();
    // 调用基类相应处理函数
    QGraphicsItem::hoverEnterEvent(event);
}

// 鼠标离开事件
void LineChartItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    // 发送信号
    emit XYChart::hovered(domain()->calculateDomainPoint(event->pos()), false);
//    event->accept();
    // 调用基类相应处理函数
    QGraphicsItem::hoverEnterEvent(event);
}

// 鼠标释放事件
void LineChartItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    // 发送信号
    emit XYChart::released(domain()->calculateDomainPoint(m_lastMousePos));
    // 发送点击信号
    if (m_mousePressed)
        emit XYChart::clicked(domain()->calculateDomainPoint(m_lastMousePos));
    // 清空鼠标按下标记
    m_mousePressed = false;
    // 调用基类相应处理函数
    QGraphicsItem::mouseReleaseEvent(event);
}

// 鼠标双击事件
void LineChartItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    // 发送信号
    emit XYChart::doubleClicked(domain()->calculateDomainPoint(m_lastMousePos));
    // 调用基类相应处理函数
    QGraphicsItem::mouseDoubleClickEvent(event);
}

#include "moc_linechartitem_p.cpp"

QT_CHARTS_END_NAMESPACE
