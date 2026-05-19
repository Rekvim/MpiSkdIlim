#include "Manager.h"
#include "Storage/Registry.h"

namespace Widgets::Chart {
ChartView* Manager::chart(ChartType type)
{
    return m_charts.value(type, nullptr);
}

void Manager::addPoints(Widgets::Chart::ChartType chart, const QVector<Widgets::Chart::Point> &points)
{
    if (!m_charts.contains(chart))
        return;

    for (const auto& p : points) {
        m_charts[chart]->addPoint(p.seriesNum, p.X, p.Y);
    }
}

void Manager::clearPoints(Widgets::Chart::ChartType chart)
{
    if (!m_charts.contains(chart))
        return;

    m_charts[chart]->clear();
}

void Manager::setVisible(ChartType chart, quint16 series, bool visible)
{
    if (!m_charts.contains(chart))
        return;

    m_charts[chart]->visible(series, visible);
}

void Manager::showDots(bool visible)
{
    m_charts[ChartType::Task]->showDots(visible);

    if (m_charts.contains(ChartType::Pressure))
        m_charts[ChartType::Pressure]->showDots(visible);
}

void Manager::duplicateMainChartsSeries()
{
    if (!m_charts.contains(ChartType::Task))
        return;

    auto* task = m_charts[ChartType::Task];

    task->duplicateChartSeries(1);
    task->duplicateChartSeries(2);
    task->duplicateChartSeries(3);
    task->duplicateChartSeries(4);

    if (m_charts.contains(ChartType::Pressure))
        m_charts[ChartType::Pressure]->duplicateChartSeries(0);
}

QPair<QList<QPointF>, QList<QPointF>>
Manager::getPoints(ChartType chart, int series)
{
    if (!m_charts.contains(chart))
        return {};

    return m_charts[chart]->getPoints(series);
}

QPixmap Manager::grabChart(ChartType chart)
{
    if (!m_charts.contains(chart))
        return {};

    return m_charts[chart]->grab();
}

ChartView*
Manager::createTrendChart(ChartView* chart, const SensorColors& colors)
{
    m_charts[ChartType::Trend] = chart;

    chart->useTimeaxis(true);
    chart->addAxis(QStringLiteral("%.2f%%"));
    chart->addSeries(0, QObject::tr("Задание"), colors.task);
    chart->addSeries(0, QObject::tr("Датчик линейных перемещений"), colors.linear);
    chart->setMaxRange(60000);

    return chart;
}

ChartView*
Manager::createStrokeChart(ChartView* chart, const SensorColors& colors)
{
    m_charts[ChartType::Stroke] = chart;

    chart->setName(QStringLiteral("Stroke"));
    chart->useTimeaxis(true);
    chart->addAxis(QStringLiteral("%.2f%%"));
    chart->addSeries(0, QObject::tr("Задание"),colors.task);
    chart->addSeries(0, QObject::tr("Датчик линейных перемещений"), colors.linear);

    return chart;
}

ChartView*
Manager::createTaskChart(ChartView* chart, const QString& strokeAxisFormat, const SensorColors& colors) {
    m_charts[ChartType::Task] = chart;

    chart->setName(QStringLiteral("Task"));
    chart->useTimeaxis(false);
    chart->addAxis(QStringLiteral("%.2f bar"));
    chart->addAxis(strokeAxisFormat);
    chart->addSeries(1, QObject::tr("Задание"), colors.task);
    chart->addSeries(1, QObject::tr("Датчик линейных перемещений"), colors.linear);
    chart->addSeries(0, QObject::tr("Датчик давления 1"), colors.pressure1);
    chart->addSeries(0, QObject::tr("Датчик давления 2"), colors.pressure2);
    chart->addSeries(0, QObject::tr("Датчик давления 3"), colors.pressure3);

    return chart;
}

ChartView*
Manager::createFrictionChart(ChartView* chart, const QString& strokeAxisFormat,const SensorColors& colors) {
    m_charts[ChartType::Friction] = chart;

    chart->setName(QStringLiteral("Friction"));
    chart->addAxis(QStringLiteral("%.2f N"));
    chart->addSeries(0, QObject::tr("Трение от перемещения"), colors.linear);
    chart->setLabelXformat(strokeAxisFormat);

    return chart;
}

ChartView*
Manager::createPressureChart(ChartView* chart, const QString& strokeAxisFormat, const SensorColors& colors) {
    m_charts[ChartType::Pressure] = chart;

    chart->setName(QStringLiteral("Pressure"));
    chart->useTimeaxis(false);
    chart->setLabelXformat(QStringLiteral("%.2f bar"));
    chart->addAxis(strokeAxisFormat);
    chart->addSeries(0, QObject::tr("Перемещение от давления"), colors.linear);
    chart->addSeries(0, QObject::tr("Линейная регрессия"), colors.task);
    chart->visible(1, false);

    return chart;
}

ChartView*
Manager::createResponseChart(ChartView* chart, const SensorColors& colors) {
    m_charts[ChartType::Response] = chart;

    chart->setName(QStringLiteral("Response"));
    chart->useTimeaxis(true);
    chart->addAxis(QStringLiteral("%.2f%%"));
    chart->addSeries(0, QObject::tr("Задание"), colors.task);
    chart->addSeries(0, QObject::tr("Датчик линейных перемещений"), colors.linear);

    return chart;
}

ChartView*
Manager::createResolutionChart(ChartView* chart, const SensorColors& colors) {
    m_charts[ChartType::Resolution] = chart;

    chart->setName(QStringLiteral("Resolution"));
    chart->useTimeaxis(true);
    chart->addAxis(QStringLiteral("%.2f%%"));
    chart->addSeries(0, QObject::tr("Задание"), colors.task);
    chart->addSeries(0, QObject::tr("Датчик линейных перемещений"), colors.linear);

    return chart;
}

ChartView*
Manager::createStepChart(ChartView* chart, const SensorColors& colors)
{
    m_charts[ChartType::Step] = chart;

    chart->setName(QStringLiteral("Step"));
    chart->useTimeaxis(true);
    chart->addAxis(QStringLiteral("%.2f%%"));
    chart->addSeries(0, QObject::tr("Задание"), colors.task);
    chart->addSeries(0, QObject::tr("Датчик линейных перемещений"), colors.linear);

    return chart;
}
}