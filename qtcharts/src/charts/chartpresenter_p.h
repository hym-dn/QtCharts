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

#ifndef CHARTPRESENTER_H
#define CHARTPRESENTER_H

#include <QtCharts/QChartGlobal>
#include <QtCharts/QChart> //because of QChart::ChartThemeId
#include <QtCharts/private/qchartglobal_p.h>
#include <private/glwidget_p.h>
#include <QtCore/QRectF>
#include <QtCore/QMargins>
#include <QtCore/QLocale>
#include <QtCore/QPointer>
#include <QtCore/QEasingCurve>

QT_CHARTS_BEGIN_NAMESPACE

class ChartItem;
class AxisItem;
class QAbstractSeries;
class ChartDataSet;
class AbstractDomain;
class ChartAxisElement;
class ChartAnimator;
class ChartBackground;
class ChartTitle;
class ChartAnimation;
class AbstractChartLayout;

// 图表主持人，负责图表绘制过程中的控制操作
class QT_CHARTS_PRIVATE_EXPORT ChartPresenter: public QObject
{
    Q_OBJECT
public:
    // z轴坐标,利用控制图层
    enum ZValues{
        BackgroundZValue = -1,
        PlotAreaZValue,
        ShadesZValue,
        GridZValue,
        AxisZValue,
        SeriesZValue,
        LineChartZValue = SeriesZValue,
        SplineChartZValue = SeriesZValue,
        BarSeriesZValue = SeriesZValue,
        ScatterSeriesZValue = SeriesZValue,
        PieSeriesZValue = SeriesZValue,
        BoxPlotSeriesZValue = SeriesZValue,
        CandlestickSeriesZValue = SeriesZValue,
        LegendZValue,
        TopMostZValue
    };

    // 状态
    enum State {
        ShowState,
        ScrollUpState,
        ScrollDownState,
        ScrollLeftState,
        ScrollRightState,
        ZoomInState,
        ZoomOutState
    };

    ChartPresenter(QChart *chart, QChart::ChartType type); // 构造
    virtual ~ChartPresenter(); // 析构


    void setGeometry(QRectF rect);
    QRectF geometry() const;

    QGraphicsItem *rootItem(){ return m_chart; }
    ChartBackground *backgroundElement();
    QAbstractGraphicsShapeItem *plotAreaElement();
    ChartTitle *titleElement();
    QList<ChartAxisElement *> axisItems() const;
    QList<ChartItem *> chartItems() const;

    QLegend *legend();

    void setBackgroundBrush(const QBrush &brush);
    QBrush backgroundBrush() const;

    void setBackgroundPen(const QPen &pen);
    QPen backgroundPen() const;

    void setBackgroundRoundness(qreal diameter);
    qreal backgroundRoundness() const;

    void setPlotAreaBackgroundBrush(const QBrush &brush);
    QBrush plotAreaBackgroundBrush() const;

    void setPlotAreaBackgroundPen(const QPen &pen);
    QPen plotAreaBackgroundPen() const;

    void setTitle(const QString &title);
    QString title() const;

    void setTitleFont(const QFont &font);
    QFont titleFont() const;

    void setTitleBrush(const QBrush &brush);
    QBrush titleBrush() const;

    void setBackgroundVisible(bool visible);
    bool isBackgroundVisible() const;

    void setPlotAreaBackgroundVisible(bool visible);
    bool isPlotAreaBackgroundVisible() const;

    void setBackgroundDropShadowEnabled(bool enabled);
    bool isBackgroundDropShadowEnabled() const;

    void setLocalizeNumbers(bool localize);
    inline bool localizeNumbers() const { return m_localizeNumbers; }
    void setLocale(const QLocale &locale);
    inline const QLocale &locale() const { return m_locale; }

    void setVisible(bool visible);

    void setAnimationOptions(QChart::AnimationOptions options);
    QChart::AnimationOptions animationOptions() const;
    void setAnimationDuration(int msecs);
    int animationDuration() const { return m_animationDuration; }
    void setAnimationEasingCurve(const QEasingCurve &curve);
    QEasingCurve animationEasingCurve() const { return m_animationCurve; }

    void startAnimation(ChartAnimation *animation);

    void setState(State state,QPointF point);
    State state() const { return m_state; }
    QPointF statePoint() const { return m_statePoint; }
    AbstractChartLayout *layout();

    QChart::ChartType chartType() const { return m_chart->chartType(); }
    QChart *chart() { return m_chart; }

    static QRectF textBoundingRect(const QFont &font, const QString &text, qreal angle = 0.0);
    static QString truncatedText(const QFont &font, const QString &text, qreal angle,
                                 qreal maxWidth, qreal maxHeight, QRectF &boundingRect); // 如果文本尺寸超过预订范围，则对文本进行剪裁
    inline static qreal textMargin() { return qreal(0.5); } // 返回文本留白

    QString numberToString(double value, char f = 'g', int prec = 6);
    QString numberToString(int value);

    void updateGLWidget();
    void glSetUseWidget(bool enable) { m_glUseWidget = enable; }

private:
    void createBackgroundItem();
    void createPlotAreaBackgroundItem();
    void createTitleItem();

public Q_SLOTS:
    void handleSeriesAdded(QAbstractSeries *series);
    void handleSeriesRemoved(QAbstractSeries *series);
    void handleAxisAdded(QAbstractAxis *axis);
    void handleAxisRemoved(QAbstractAxis *axis);

Q_SIGNALS:
    void plotAreaChanged(const QRectF &plotArea);

private:
    QChart *m_chart; // 所属图表
    QList<ChartItem *> m_chartItems; // 图表项
    QList<ChartAxisElement *> m_axisItems; // 轴元素项
    QList<QAbstractSeries *> m_series; // 序列项
    QList<QAbstractAxis *> m_axes; // 轴项
    QChart::AnimationOptions m_options; // 动画选项
    int m_animationDuration; // ???
    QEasingCurve m_animationCurve; // ???
    State m_state; // 状态
    QPointF m_statePoint; // 状态点
    AbstractChartLayout *m_layout; // 布局
    ChartBackground *m_background; // 背景
    QAbstractGraphicsShapeItem *m_plotAreaBackground; // 绘图区域背景
    ChartTitle *m_title; // 标题
    QRectF m_rect; // 矩形空间
    bool m_localizeNumbers; // ???
    QLocale m_locale; // ???
#ifndef QT_NO_OPENGL
    QPointer<GLWidget> m_glWidget; // OpenGL 部件
#endif
    bool m_glUseWidget; // 是否使用OpengGL部件
};

QT_CHARTS_END_NAMESPACE

#endif /* CHARTPRESENTER_H */
