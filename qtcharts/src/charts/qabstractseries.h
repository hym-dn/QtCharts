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

#ifndef QABSTRACTSERIES_H
#define QABSTRACTSERIES_H

#include <QtCharts/QChartGlobal>
#include <QtCharts/QAbstractAxis>
#include <QtCore/QObject>
#include <QtGui/QPen>

QT_CHARTS_BEGIN_NAMESPACE

class QAbstractSeriesPrivate;
class QChart;

// 序列类，负责抽象描述一项序列信息
class QT_CHARTS_EXPORT QAbstractSeries : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(bool visible READ isVisible WRITE setVisible NOTIFY visibleChanged)
    Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity NOTIFY opacityChanged)
    Q_PROPERTY(SeriesType type READ type)
    Q_PROPERTY(bool useOpenGL READ useOpenGL WRITE setUseOpenGL NOTIFY useOpenGLChanged)
    Q_ENUMS(SeriesType)

public:

    // 序列类型
    enum SeriesType {
        SeriesTypeLine,
        SeriesTypeArea,
        SeriesTypeBar,
        SeriesTypeStackedBar,
        SeriesTypePercentBar,
        SeriesTypePie,
        SeriesTypeScatter,
        SeriesTypeSpline,
        SeriesTypeHorizontalBar,
        SeriesTypeHorizontalStackedBar,
        SeriesTypeHorizontalPercentBar,
        SeriesTypeBoxPlot,
        SeriesTypeCandlestick
    };

protected:

    // 构造
    QAbstractSeries(QAbstractSeriesPrivate &d, QObject *parent = nullptr);

public:
    ~QAbstractSeries(); // 析构
    virtual SeriesType type() const = 0; // 序列类型

    void setName(const QString &name); // 设置名字
    QString name() const; // 获取名字
    void setVisible(bool visible = true); // 设置是否可见
    bool isVisible() const; // 获取是否可见
    qreal opacity() const; // 获取不透明度
    void setOpacity(qreal opacity); // 设置透明度
    void setUseOpenGL(bool enable = true); // 设置是否使用OpenGL
    bool useOpenGL() const; // 获取是否使用OpenGl

    QChart *chart() const; // 获取所属图表

    bool attachAxis(QAbstractAxis *axis); // 捆绑坐标轴
    bool detachAxis(QAbstractAxis *axis); // 松绑坐标轴
    QList<QAbstractAxis*> attachedAxes(); // 获取捆绑坐标轴

    void show(); // 显示
    void hide(); // 隐藏

Q_SIGNALS:
    void nameChanged();
    void visibleChanged();
    void opacityChanged();
    void useOpenGLChanged();

protected:
    QScopedPointer<QAbstractSeriesPrivate> d_ptr; // 序列类私有成员
    friend class ChartDataSet;
    friend class ChartPresenter;
    friend class ChartThemeManager;
    friend class QLegendPrivate;
    friend class DeclarativeChart;
    friend class QAreaSeries;
    friend class GLWidget;
};

QT_CHARTS_END_NAMESPACE

#endif // QABSTRACTSERIES_H
