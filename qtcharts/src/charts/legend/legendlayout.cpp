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

#include <private/legendlayout_p.h>
#include <private/chartpresenter_p.h>
#include <private/qlegend_p.h>
#include <private/abstractchartlayout_p.h>

#include <private/qlegendmarker_p.h>
#include <private/legendmarkeritem_p.h>
#include <QtCharts/QLegendMarker>

QT_CHARTS_BEGIN_NAMESPACE

// 构造
LegendLayout::LegendLayout(QLegend *legend)
    : m_legend(legend), // 源图例
      m_offsetX(0), // x偏移
      m_offsetY(0) // y偏移
{

}

// 析构
LegendLayout::~LegendLayout()
{

}

// 设置偏移量
void LegendLayout::setOffset(qreal x, qreal y)
{
    // 滚动标记
    bool scrollHorizontal = true;
    switch (m_legend->alignment()) {
    case Qt::AlignTop:
    case Qt::AlignBottom:
        scrollHorizontal = true;
        break;
    case Qt::AlignLeft:
    case Qt::AlignRight:
        scrollHorizontal = false;
        break;
    }

    // If detached, the scrolling direction is vertical instead of horizontal and vice versa.
    // 如果没有捆绑到图表
    if (!m_legend->isAttachedToChart())
        scrollHorizontal = !scrollHorizontal;

    // 外接矩形
    QRectF boundingRect = geometry();
    // 获取内容留白
    qreal left, top, right, bottom;
    getContentsMargins(&left, &top, &right, &bottom);
    // 调整外接矩形
    boundingRect.adjust(left, top, -right, -bottom);

    // Limit offset between m_minOffset and m_maxOffset
    // 水平滚动
    if (scrollHorizontal) {
        // 宽度小于外接宽度
        if (m_width <= boundingRect.width())
            return;

        // 偏移量改变
        if (x != m_offsetX) {
            // 重置偏移量x
            m_offsetX = qBound(m_minOffsetX, x, m_maxOffsetX);
            // 设置位置
            m_legend->d_ptr->items()->setPos(-m_offsetX, boundingRect.top());
        }
    }
    // 无滚动
    else {
        // 高度小于外接高度
        if (m_height <= boundingRect.height())
            return;

        // 偏移量改变
        if (y != m_offsetY) {
            // 重置偏移量y
            m_offsetY = qBound(m_minOffsetY, y, m_maxOffsetY);
            // 设置位置
            m_legend->d_ptr->items()->setPos(boundingRect.left(), -m_offsetY);
        }
    }
}

// 返回偏移量
QPointF LegendLayout::offset() const
{
    return QPointF(m_offsetX, m_offsetY);
}

// 更新
void LegendLayout::invalidate()
{
    QGraphicsLayout::invalidate();
    if (m_legend->isAttachedToChart())
        m_legend->d_ptr->m_presenter->layout()->invalidate();
}

// 设置几何尺寸
void LegendLayout::setGeometry(const QRectF &rect)
{
    // 设置图例是否可见
    m_legend->d_ptr->items()->setVisible(m_legend->isVisible());

    // 设置几何尺寸
    QGraphicsLayout::setGeometry(rect);

    // 如果图例捆绑了图表
    if (m_legend->isAttachedToChart())
        setAttachedGeometry(rect); // 设置绑定几何尺寸
    // 如果图例尚未捆绑图表
    else
        setDettachedGeometry(rect); // 设置未绑定几何尺寸
}

// 设置绑定几何尺寸
void LegendLayout::setAttachedGeometry(const QRectF &rect)
{
    // 尺寸无效
    if (!rect.isValid())
        return;

    // 记录x、y偏移
    qreal oldOffsetX = m_offsetX;
    qreal oldOffsetY = m_offsetY;

    // 清空x、y偏移
    m_offsetX = 0;
    m_offsetY = 0;

    QSizeF size(0, 0);

    // 图例中标记为空
    if (m_legend->d_ptr->markers().isEmpty()) {
        return; // 直接返回
    }

    // 清空宽、高
    m_width = 0;
    m_height = 0;

    // 获取留白尺寸
    qreal left, top, right, bottom;
    getContentsMargins(&left, &top, &right, &bottom);

    // 调整几何尺寸
    QRectF geometry = rect.adjusted(left, top, -right, -bottom);

    // 根据图例停靠方式，来调整布局尺寸
    switch(m_legend->alignment()) {
    // 顶部、底部停靠
    case Qt::AlignTop:
    case Qt::AlignBottom: {
            // Calculate the space required for items and add them to a sorted list.
            qreal markerItemsWidth = 0; // 标记宽度
            qreal itemMargins = 0; // 项留白
            QList<LegendWidthStruct *> legendWidthList; // 图例标记、宽度存储链表
            foreach (QLegendMarker *marker, m_legend->d_ptr->markers()) { // 遍历标记
                LegendMarkerItem *item = marker->d_ptr->item(); // 获取一项标记
                if (item->isVisible()) { // 可见
                    QSizeF dummySize;
                    qreal itemWidth = item->sizeHint(Qt::PreferredSize, dummySize).width(); // 默认宽度
                    LegendWidthStruct *structItem = new LegendWidthStruct; // 创建标记、宽度结构体
                    structItem->item = item; // 存储标记
                    structItem->width = itemWidth; // 存储宽度
                    legendWidthList.append(structItem); // 最小到链表中
                    markerItemsWidth += itemWidth; // 累计宽度
                    itemMargins += marker->d_ptr->item()->m_margin; // 累计留白尺寸
                }
            }
            // 排序
            std::sort(legendWidthList.begin(), legendWidthList.end(), widthLongerThan);

            // If the items would occupy more space than is available, start truncating them
            // from the longest one.
            // 计算有效区域
            qreal availableGeometry = geometry.width() - right - left * 2 - itemMargins;
            // 实际占用宽度大于有效宽度，并且图例标记存在
            if (markerItemsWidth >= availableGeometry && legendWidthList.count() > 0) { // 进行截断操作
                // 截断标记
                bool truncated(false);
                // 标记计数
                int count = legendWidthList.count();
                // 遍历标记
                for (int i = 1; i < count; i++) {
                    // 换算索引
                    int truncateIndex = i - 1;
                    // 前一个标记宽度>=后一个标记宽度，并且尚未截断
                    while (legendWidthList.at(truncateIndex)->width >= legendWidthList.at(i)->width
                           && !truncated) {
                        // 前一个标记宽度递减
                        legendWidthList.at(truncateIndex)->width--;
                        // 累计标记宽度递减
                        markerItemsWidth--;
                        // 并非第一次迭代
                        if (i > 1) {
                            // Truncate the items that are before the truncated one in the list.
                            // 查看小于truncateIndex的全部标记
                            for (int j = truncateIndex - 1; j >= 0; j--) {
                                if (legendWidthList.at(truncateIndex)->width
                                        < legendWidthList.at(j)->width) { // 大于truncateIndex的标记
                                    legendWidthList.at(j)->width--; // 宽度递减
                                    markerItemsWidth--; // 累计宽度递减
                                }
                            }
                        }
                        // 累计宽度小于有效宽度
                        if (markerItemsWidth < availableGeometry)
                            truncated = true; // 设置标记
                    }
                    // Truncate the last item if needed.
                    // 截断最后一项，如果必要的话
                    if (i == count - 1) {
                        if (legendWidthList.at(count - 1)->width
                                > legendWidthList.at(truncateIndex)->width) {
                            legendWidthList.at(count - 1)->width--;
                            markerItemsWidth--;
                        }
                    }
                    // 截断成功，跳出循环
                    if (truncated)
                        break;
                }
                // Items are of same width and all of them need to be truncated
                // or there is just one item that is truncated.
                // 如果宽度依然大于有效区域，则统一强制截断
                while (markerItemsWidth >= availableGeometry) {
                    for (int i = 0; i < count; i++) {
                        legendWidthList.at(i)->width--;
                        markerItemsWidth--;
                    }
                }
            }

            QPointF point(0,0);
            // 标记计数
            int markerCount = m_legend->d_ptr->markers().count();
            // 遍历标记
            for (int i = 0; i < markerCount; i++) {
                // 记录标记
                QLegendMarker *marker;
                // 获取标记
                if (m_legend->d_ptr->m_reverseMarkers) // 逆向标记
                    marker = m_legend->d_ptr->markers().at(markerCount - 1 - i);
                else // 正向标记
                    marker = m_legend->d_ptr->markers().at(i);
                // 获取标记项
                LegendMarkerItem *item = marker->d_ptr->item();
                if (item->isVisible()) { // 可见
                    // 记录项目最终尺寸
                    QRectF itemRect = geometry;
                    // 获取有效宽度
                    qreal availableWidth = 0;
                    for (int i = 0; i < legendWidthList.size(); ++i) {
                        if (legendWidthList.at(i)->item == item) {
                            availableWidth = legendWidthList.at(i)->width;
                            break;
                        }
                    }
                    // 更新项目宽度
                    itemRect.setWidth(availableWidth);
                    // 设置项目尺寸
                    item->setGeometry(itemRect);
                    // 设置项目位置
                    item->setPos(point.x(),geometry.height()/2 - item->boundingRect().height()/2);
                    // 项目外接矩形
                    const QRectF &rect = item->boundingRect();
                    // 拓展尺寸
                    size = size.expandedTo(rect.size());
                    // 存储宽度
                    qreal w = rect.width();
                    // 更新宽度
                    m_width = m_width + w - item->m_margin;
                    // 设置起始x
                    point.setX(point.x() + w);
                }
            }
            // Delete structs from the container
            // 清空链表
            qDeleteAll(legendWidthList);

            // Round to full pixel via QPoint to avoid one pixel clipping on the edge in some cases
            // 设置项目组位置
            if (m_width < geometry.width()) {
                m_legend->d_ptr->items()->setPos(QPoint(geometry.width() / 2 - m_width / 2,
                                                        geometry.top()));
            } else {
                m_legend->d_ptr->items()->setPos(geometry.topLeft().toPoint());
            }
            // 更新高度
            m_height = size.height();
        }
        break;
    // 左、右停靠
    case Qt::AlignLeft:
    case Qt::AlignRight: {
            // 点
            QPointF point(0,0);
            // 标记计数
            int markerCount = m_legend->d_ptr->markers().count();
            // 遍历标记
            for (int i = 0; i < markerCount; i++) {
                // 获取当前标记
                QLegendMarker *marker;
                if (m_legend->d_ptr->m_reverseMarkers) // 逆向标记
                    marker = m_legend->d_ptr->markers().at(markerCount - 1 - i);
                else // 正向标记
                    marker = m_legend->d_ptr->markers().at(i);
                // 获取当前标记项
                LegendMarkerItem *item = marker->d_ptr->item();
                // 可见
                if (item->isVisible()) {
                    item->setGeometry(geometry); // 设置尺寸
                    item->setPos(point); // 设置标记位置
                    const QRectF &rect = item->boundingRect(); // 标记外接矩形
                    qreal h = rect.height(); // 记录高度
                    size = size.expandedTo(rect.size()); // 计算尺寸
                    m_height+=h; // 更新高度
                    point.setY(point.y() + h); // 更新起始Y
                }
            }

            // Round to full pixel via QPoint to avoid one pixel clipping on the edge in some cases
            // 更新项目组位置
            if (m_height < geometry.height()) {
                m_legend->d_ptr->items()->setPos(QPoint(geometry.left(),
                                                        geometry.height() / 2 - m_height / 2));
            } else {
                m_legend->d_ptr->items()->setPos(geometry.topLeft().toPoint());
            }
            //  更新宽度
            m_width = size.width();
            break;
            }
        }
    // 其他方式
    m_minOffsetX = -left;
    m_minOffsetY = - top;
    m_maxOffsetX = m_width - geometry.width() - right;
    m_maxOffsetY = m_height - geometry.height() - bottom;
    setOffset(oldOffsetX, oldOffsetY);
}

// 设置松绑尺寸
void LegendLayout::setDettachedGeometry(const QRectF &rect)
{
    // 尺寸无效
    if (!rect.isValid())
        return;

    // Detached layout is different.
    // In detached mode legend may have multiple rows and columns, so layout calculations
    // differ a log from attached mode.
    // Also the scrolling logic is bit different.

    // 记录当前偏移x、y坐标
    qreal oldOffsetX = m_offsetX;
    qreal oldOffsetY = m_offsetY;

    // 清空当前偏移坐标
    m_offsetX = 0;
    m_offsetY = 0;

    // 获取内容留白
    qreal left, top, right, bottom;
    getContentsMargins(&left, &top, &right, &bottom);

    // 记录去留白后尺寸
    QRectF geometry = rect.adjusted(left, top, -right, -bottom);

    // 记录尺寸
    QSizeF size(0, 0);

    // 标记链表
    QList<QLegendMarker *> markers = m_legend->d_ptr->markers();

    // 标记为空
    if (markers.isEmpty())
        return;

    // 图例停靠方式
    switch (m_legend->alignment()) {
    // 顶部停靠
    case Qt::AlignTop: {
        // 起始点
        QPointF point(0, 0);
        // 清除宽、高
        m_width = 0;
        m_height = 0;
        // 遍历标记
        for (int i = 0; i < markers.count(); i++) {
            // 获取标记项
            LegendMarkerItem *item = markers.at(i)->d_ptr->item();
            // 标记项可见
            if (item->isVisible()) {
                // 设置尺寸
                item->setGeometry(geometry);
                // 设置位置
                item->setPos(point.x(),point.y());
                // 项目外接矩形
                const QRectF &boundingRect = item->boundingRect();
                // 记录宽、高
                qreal w = boundingRect.width();
                qreal h = boundingRect.height();
                // 更新宽、高
                m_width = qMax(m_width,w);
                m_height = qMax(m_height,h);
                // 起始点更新
                point.setX(point.x() + w);
                // 出界
                if (point.x() + w > geometry.left() + geometry.width() - right) {
                    // Next item would go off rect.
                    point.setX(0); // 设置起始点x
                    point.setY(point.y() + h); // 设置起始点y （换行）
                    if (i+1 < markers.count()) {
                        m_height += h; // 高度累计
                    }
                }
            }
        }
        // 项目位置
        m_legend->d_ptr->items()->setPos(geometry.topLeft());
        // 更新偏移量
        m_minOffsetX = -left;
        m_minOffsetY = -top;
        m_maxOffsetX = m_width - geometry.width() - right;
        m_maxOffsetY = m_height - geometry.height() - bottom;
    }
    break;
    // 底部停靠
    case Qt::AlignBottom: {
        // 起始点
        QPointF point(0,geometry.height());
        // 宽、高清零
        m_width = 0;
        m_height = 0;
        // 标记计数
        for (int i = 0; i < markers.count(); i++) {
            // 获取标记项
            LegendMarkerItem *item = markers.at(i)->d_ptr->item();
            // 项可见
            if (item->isVisible()) {
                // 设置几何尺寸
                item->setGeometry(geometry);
                // 项外接矩形
                const QRectF &boundingRect = item->boundingRect();
                // 记录宽、高
                qreal w = boundingRect.width();
                qreal h = boundingRect.height();
                // 更新宽、高
                m_width = qMax(m_width,w);
                m_height = qMax(m_height,h);
                // 记录位置
                item->setPos(point.x(),point.y() - h);
                // 更新起始点
                point.setX(point.x() + w);
                // 出界
                if (point.x() + w > geometry.left() + geometry.width() - right) {
                    // Next item would go off rect.
                    point.setX(0);
                    point.setY(point.y() - h);
                    if (i+1 < markers.count()) {
                        m_height += h;
                    }
                }
            }
        }
        // 设置项目组位置
        m_legend->d_ptr->items()->setPos(geometry.topLeft());
        // 重新更新偏移
        m_minOffsetX = -left;
        m_minOffsetY = -m_height + geometry.height() - top;
        m_maxOffsetX = m_width - geometry.width() - right;
        m_maxOffsetY = -bottom;
    }
    break;
    // 左侧停靠
    case Qt::AlignLeft: {
        // 起始点
        QPointF point(0, 0);
        // 清空宽高
        m_width = 0;
        m_height = 0;
        // 标记计数
        qreal maxWidth = 0;
        for (int i = 0; i < markers.count(); i++) {
            // 获取标记项
            LegendMarkerItem *item = markers.at(i)->d_ptr->item();
            // 可见
            if (item->isVisible()) {
                // 设置尺寸
                item->setGeometry(geometry);
                // 边界矩形
                const QRectF &boundingRect = item->boundingRect();
                // 编辑宽、高
                qreal w = boundingRect.width();
                qreal h = boundingRect.height();
                // 存储高度、宽度
                m_height = qMax(m_height,h);
                maxWidth = qMax(maxWidth,w);
                // 设置位置
                item->setPos(point.x(),point.y());
                // 更新起始点
                point.setY(point.y() + h);
                // 出界
                if (point.y() + h > geometry.bottom() - bottom) {
                    // Next item would go off rect.
                    point.setX(point.x() + maxWidth);
                    point.setY(0);
                    if (i+1 < markers.count()) {
                        m_width += maxWidth;
                        maxWidth = 0;
                    }
                }
            }
        }
        // 累计宽度
        m_width += maxWidth;
        // 更新组位置
        m_legend->d_ptr->items()->setPos(geometry.topLeft());
        // 更新偏移
        m_minOffsetX = -left;
        m_minOffsetY = -top;
        m_maxOffsetX = m_width - geometry.width() - right;
        m_maxOffsetY = m_height - geometry.height() - bottom;
    }
    break;
    // 右侧停靠
    case Qt::AlignRight: {
        // 起始点
        QPointF point(geometry.width(), 0);
        // 清空宽、高
        m_width = 0;
        m_height = 0;
        qreal maxWidth = 0;
        // 遍历标记
        for (int i = 0; i < markers.count(); i++) {
            // 获取标记
            LegendMarkerItem *item = markers.at(i)->d_ptr->item();
            // 可见
            if (item->isVisible()) {
                // 设置尺寸
                item->setGeometry(geometry);
                // 外接矩形
                const QRectF &boundingRect = item->boundingRect();
                // 存储宽、高
                qreal w = boundingRect.width();
                qreal h = boundingRect.height();
                m_height = qMax(m_height,h);
                maxWidth = qMax(maxWidth,w);
                // 更新位置
                item->setPos(point.x() - w,point.y());
                // 更新起始点
                point.setY(point.y() + h);
                // 出界
                if (point.y() + h > geometry.bottom()-bottom) {
                    // Next item would go off rect.
                    point.setX(point.x() - maxWidth);
                    point.setY(0);
                    if (i+1 < markers.count()) {
                        m_width += maxWidth;
                        maxWidth = 0;
                    }
                }
            }
        }
        // 更新宽度
        m_width += maxWidth;
        // 设置位置
        m_legend->d_ptr->items()->setPos(geometry.topLeft());
        // 更新偏移量
        m_minOffsetX = - m_width + geometry.width() - left;
        m_minOffsetY = -top;
        m_maxOffsetX = - right;
        m_maxOffsetY = m_height - geometry.height() - bottom;
    }
    break;
    default:
        break;
    }
    // 恢复偏移
    setOffset(oldOffsetX, oldOffsetY);
}

// 默认尺寸
QSizeF LegendLayout::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    // 记录尺寸
    QSizeF size(0, 0);

    // 内容留白
    qreal left, top, right, bottom;
    getContentsMargins(&left, &top, &right, &bottom);

    // 约束尺寸有效
    if(constraint.isValid()) {
        // 获取最大尺寸
        foreach(QLegendMarker *marker, m_legend->d_ptr->markers()) {
            // 获取标记项
            LegendMarkerItem *item = marker->d_ptr->item();
            // 拓展尺寸
            size = size.expandedTo(item->effectiveSizeHint(which));
        }
        // 限制尺寸
        size = size.boundedTo(constraint);
    }
    // 约束宽度有效
    else if (constraint.width() >= 0) {
        // 遍历标记，累计宽度
        qreal width = 0;
        qreal height = 0;
        foreach(QLegendMarker *marker, m_legend->d_ptr->markers()) {
            LegendMarkerItem *item = marker->d_ptr->item();
            width+=item->effectiveSizeHint(which).width();
            height=qMax(height,item->effectiveSizeHint(which).height());
        }
        // 取最小宽度
        size = QSizeF(qMin(constraint.width(),width), height);
    }
    // 约束高度有效
    else if (constraint.height() >= 0) {
        // 遍历标记，累计高度
        qreal width = 0;
        qreal height = 0;
        foreach(QLegendMarker *marker, m_legend->d_ptr->markers()) {
            LegendMarkerItem *item = marker->d_ptr->item();
            width=qMax(width,item->effectiveSizeHint(which).width());
            height+=height,item->effectiveSizeHint(which).height();
        }
        // 取最小高度
        size = QSizeF(width,qMin(constraint.height(),height));
    }
    // 约束无效
    else {
        // 取最大尺寸
        foreach(QLegendMarker *marker, m_legend->d_ptr->markers()) {
            LegendMarkerItem *item = marker->d_ptr->item();
            size = size.expandedTo(item->effectiveSizeHint(which));
        }
    }
    // 增加留白尺寸
    size += QSize(left + right, top + bottom);
    // 返回尺寸
    return size;
}

// 图例标记、宽度结构体大小比较函数
bool LegendLayout::widthLongerThan(const LegendWidthStruct *item1,
                                   const LegendWidthStruct *item2)
{
    return item1->width > item2->width;
}

QT_CHARTS_END_NAMESPACE
