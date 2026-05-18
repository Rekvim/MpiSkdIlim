#ifndef OPTIONTEST_H
#define OPTIONTEST_H

#pragma once

#include <QObject>
#include <QPointF>
#include <QTimer>
#include <QDateTime>

#include "Test.h"


class OptionTest : public Test
{
    Q_OBJECT

public:
    explicit OptionTest(QObject *parent = nullptr, bool endTestAfterProcess = true);
    virtual void Process() override;

    enum class Mode
    {
        Response,
        Resolution
    };

    void setMode(Mode m);

    struct Task
    {
        QVector<quint16> value;
        quint32 delay;
        bool normalOpen = false;
    };

    struct Segment
    {
        int from;
        int to;
        double task;
        double actual;
    };

    struct OptionResult
    {
        double selectedPercent  = 0.0;
        double actualPercent    = 0.0;
        double stepErrorPercent = 0.0;
    };

    struct OptionResults
    {
        double baseTask = 0.0;

        double baseActual = 0.0;
        double baseActual_two = 0.0;

        QVector<OptionResult> upward;
        QVector<OptionResult> downward;
    };
    void SetTask(Task task);

private:
    OptionResults calculate(const QVector<QVector<QPointF>>& points) const;
    OptionResults calculateResponse(const QVector<Segment>& segments) const;
    OptionResults calculateResolution(const QVector<Segment>& segments) const;
    Mode m_mode;
    Task m_task;
    const bool m_endTestAfterProcess;
signals:
    void SetStartTime();
    void GetPoints(QVector<QVector<QPointF>>& points);
    void Results(const OptionResults& results);
};

#endif // OPTIONTEST_H
