#pragma once

#include <QHash>
#include <QVector>
#include <QPointF>

#include "ChartView.h"
#include "ChartType.h"
#include "Point.h"

struct SensorColors;

namespace Widgets::Chart {
class Manager : public QObject
{
    Q_OBJECT

public:
    explicit Manager(QObject* parent = nullptr) : QObject(parent) {}

    struct SeriesVisibilityBackup
    {
        QVector<bool> visible;
    };

    void duplicateMainChartsSeries();

    QPair<QList<QPointF>, QList<QPointF>> getPoints(Widgets::Chart::ChartType chart, int series);

    QPixmap grabChart(Widgets::Chart::ChartType chart);

    SeriesVisibilityBackup hidePressureAuxSeries();
    SeriesVisibilityBackup hideTaskAuxSeries();

    void restoreSeries(Widgets::Chart::ChartType chart, const SeriesVisibilityBackup& backup);

    ChartView* chart(Widgets::Chart::ChartType type);
    ChartView* createTrendChart(ChartView* chart, const SensorColors& colors);
    ChartView* createStrokeChart(ChartView* chart, const SensorColors& colors);
    ChartView* createTaskChart(
        ChartView* chart,
        const QString& strokeAxisFormat,
        const SensorColors& colors
    );
    ChartView* createFrictionChart(
        ChartView* chart,
        const QString& strokeAxisFormat,
        const SensorColors& colors
    );
    ChartView* createPressureChart(
        ChartView* chart,
        const QString& strokeAxisFormat,
        const SensorColors& colors
    );
    ChartView* createResponseChart(ChartView* chart, const SensorColors& colors);
    ChartView* createResolutionChart(ChartView* chart, const SensorColors& colors);
    ChartView* createStepChart(ChartView* chart, const SensorColors& colors);
    ChartView* createCyclicChart(ChartView* chart, const SensorColors& colors);
public slots:
    void addPoints(Widgets::Chart::ChartType chart, const QVector<Widgets::Chart::Point> &points);
    void clearPoints(Widgets::Chart::ChartType chart);
    void setVisible(Widgets::Chart::ChartType chart, quint16 series, bool visible);
    void showDots(bool visible);

private:
    QHash<Widgets::Chart::ChartType, ChartView*> m_charts;
};
}