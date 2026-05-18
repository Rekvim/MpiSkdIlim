#include "OptionTest.h"
#include <QDebug>
#include <cmath>

OptionTest::OptionTest(QObject *parent, bool endTestAfterProcess)
    : Test(parent)
    , m_endTestAfterProcess(endTestAfterProcess)
{}

void OptionTest::setMode(Mode m)
{
    m_mode = m;
}

namespace {

static qreal averageY(const QVector<QPointF>& line, int from, int to)
{
    if (line.isEmpty() || from > to)
        return 0.0;

    from = qMax(0, from);
    to   = qMin(to, line.size() - 1);

    qreal sum = 0.0;
    int count = 0;

    for (int i = from; i <= to; ++i)
    {
        const qreal y = line.at(i).y();

        if (!std::isfinite(y))
            continue;

        sum += y;
        ++count;
    }

    if (count == 0)
        return 0.0;

    return sum / count;
}
} // namespace


void OptionTest::Process()
{
    emit started();

    if (m_task.value.empty())
    {
        emit EndTest();
        return;
    }

    emit SetStartTime();

    setDacBlocked(m_task.value.first(), 10000, true);

    if (m_terminate)
    {
        emit EndTest();
        return;
    }

    m_graphTimer->start(50);

    for (const auto &value : m_task.value)
    {
        setDacBlocked(value, m_task.delay);

        if (m_terminate)
        {
            emit EndTest();
            return;
        }
    }

    QVector<QVector<QPointF>> points;
    emit GetPoints(points);

    OptionResults res = calculate(points);

    emit Results(res);

    if (m_endTestAfterProcess)
        emit EndTest();
}


void OptionTest::SetTask(Task task)
{
    m_task = task;
}


OptionTest::OptionResults
OptionTest::calculate(const QVector<QVector<QPointF>>& points) const
{
    OptionResults result;

    if (points.size() < 2)
        return result;

    const QVector<QPointF>& actualLine = points[0];
    const QVector<QPointF>& taskLine   = points[1];

    const int N = qMin(actualLine.size(), taskLine.size());

    if (N < 5)
        return result;

    QVector<Segment> segments;

    int start = 0;

    for (int i = 1; i < N; ++i)
    {
        if (taskLine[i].y() != taskLine[i - 1].y())
        {
            Segment s;

            s.from = start;
            s.to   = i - 1;

            s.task = taskLine[start].y();
            s.actual = averageY(actualLine, qMax(start, i - 5), i - 1);

            if (std::isfinite(s.actual))
                segments.push_back(s);

            start = i;
        }
    }

    Segment last;

    last.from = start;
    last.to = N - 1;
    last.task = taskLine[start].y();
    last.actual = averageY(actualLine, qMax(start, N - 5), N - 1);

    if (std::isfinite(last.actual))
        segments.push_back(last);

    if (segments.size() < 3)
        return result;

    if (m_mode == Mode::Resolution)
        return calculateResolution(segments);

    if (m_mode == Mode::Response)
        return calculateResponse(segments);

    return result;
}
OptionTest::OptionResults
OptionTest::calculateResolution(const QVector<Segment>& segments) const
{
    OptionResults result;

    if (segments.size() < 3)
        return result;

    auto r2 = [](double v)
    {
        return std::round(v * 100.0) / 100.0;
    };

    const double eps = 0.02;

    auto sameTask = [&](double a, double b)
    {
        return std::abs(r2(a) - r2(b)) < eps;
    };

    // ----------------------------
    // поиск базового сегмента
    // ----------------------------

    int baseIndex = -1;

    for (int i = 1; i < segments.size() - 1; ++i)
    {
        for (int j = i + 1; j < segments.size(); ++j)
        {
            if (sameTask(segments[i].task, segments[j].task))
            {
                baseIndex = i;
                break;
            }
        }

        if (baseIndex != -1)
            break;
    }

    if (baseIndex == -1)
        return result;

    const double baseTask = r2(segments[baseIndex].task);
    const double baseActual = r2(segments[baseIndex].actual);

    if (!std::isfinite(baseTask) || !std::isfinite(baseActual))
        return result;

    result.baseTask = baseTask;
    result.baseActual = baseActual;

    int upCount = 0;
    int downCount = 0;

    // ----------------------------
    // анализ шагов
    // ----------------------------

    int stepCount = 0;

    for (int i = baseIndex + 1; i < segments.size(); ++i)
    {
        const auto& seg = segments[i];

        if (stepCount >= 8)
            break;

        double stepTask = r2(seg.task);
        double stepActual = r2(seg.actual);

        if (!std::isfinite(stepTask) || !std::isfinite(stepActual))
            continue;

        double stepSize = std::abs(stepTask - baseTask);

        if (stepSize < 0.0001)
            continue;

        double idealStep;

        if (stepTask > baseTask)
            idealStep = baseActual + stepSize;
        else
            idealStep = baseActual - stepSize;

        double diff = std::abs(stepActual - idealStep);
        double error = diff / stepSize * 100.0;

        if (!std::isfinite(error))
            continue;

        OptionResult rec;

        rec.selectedPercent = stepTask;
        rec.actualPercent = stepActual;
        rec.stepErrorPercent = error;

        bool upward = stepTask > baseTask;

        if (upward)
        {
            if (upCount >= 4)
                continue;

            result.upward.push_back(rec);
            ++upCount;
        }
        else
        {
            if (downCount >= 4)
                continue;

            result.downward.push_back(rec);
            ++downCount;
        }
    }

    if (m_task.normalOpen)
        std::swap(result.upward, result.downward);

    return result;
}

OptionTest::OptionResults
OptionTest::calculateResponse(const QVector<Segment>& segments) const
{
    OptionResults result;

    if (segments.size() < 2)
        return result;

    auto r2 = [](double v)
    {
        return std::round(v * 100.0) / 100.0;
    };

    const double eps = 0.02;

    auto sameTask = [&](double a, double b)
    {
        return std::abs(r2(a) - r2(b)) < eps;
    };

    // -------------------------------------------------
    // поиск базового сегмента
    // -------------------------------------------------

    int baseIndex = -1;

    for (int i = 1; i < segments.size() - 1; ++i)
    {
        for (int j = i + 1; j < segments.size(); ++j)
        {
            if (sameTask(segments[i].task, segments[j].task))
            {
                baseIndex = i;
                break;
            }
        }

        if (baseIndex != -1)
            break;
    }

    if (baseIndex == -1)
        return result;

    double baseTask   = r2(segments[baseIndex].task);
    double baseActual = r2(segments[baseIndex].actual);

    result.baseTask   = baseTask;
    result.baseActual = baseActual;

    double prevTask   = baseTask;
    double prevActual = baseActual;

    bool baseReturnCaptured = false;
    int stepCount = 0;

    int direction = 0; // -1 вниз, +1 вверх

    for (int i = baseIndex + 1; i < segments.size(); ++i)
    {
        const auto& seg = segments[i];

        double stepTask   = r2(seg.task);
        double stepActual = r2(seg.actual);

        if (!std::isfinite(stepTask) || !std::isfinite(stepActual))
            continue;

        if (stepCount >= 8)
            break;

        double stepSize = std::abs(stepTask - prevTask);

        if (stepSize < 0.0001)
            continue;

        // -------------------------------------------------
        // определяем направление
        // -------------------------------------------------

        int newDirection = (stepTask > prevTask) ? +1 : -1;

        // -------------------------------------------------
        // фиксация второй базы (возврат в baseTask)
        // -------------------------------------------------

        if (!baseReturnCaptured && sameTask(stepTask, baseTask))
        {
            baseReturnCaptured = true;

            prevTask   = stepTask;
            prevActual = stepActual;
            direction  = newDirection;

            qDebug() << "SECOND BASE CAPTURED:"
                     << stepActual;

            result.baseActual_two = stepActual;


            continue;
        }

        // -------------------------------------------------
        // вычисление ошибки шага
        // -------------------------------------------------

        double actualStep = std::abs(stepActual - prevActual);

        if (actualStep < 0.01)
            continue;

        double ideal =
            (stepTask > prevTask)
                ? (prevActual + stepSize)
                : (prevActual - stepSize);

        double diff = std::abs(stepActual - ideal);
        double error = diff / stepSize * 100.0;

        if (!std::isfinite(error))
            continue;

        OptionResult rec;
        rec.selectedPercent  = stepTask;
        rec.actualPercent    = stepActual;
        rec.stepErrorPercent = error;

        bool upward = stepTask > prevTask;

        if (m_task.normalOpen)
            upward = !upward;

        if (upward)
            result.upward.push_back(rec);
        else
            result.downward.push_back(rec);

        prevTask   = stepTask;
        prevActual = stepActual;
        direction  = newDirection;

        ++stepCount;

        qDebug() << "TASK:" << stepTask
                 << "ACTUAL:" << stepActual
                 << "BASE:" << baseTask;
    }

    return result;
}

