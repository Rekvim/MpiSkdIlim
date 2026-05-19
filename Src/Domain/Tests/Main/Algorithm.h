#pragma once

#include "Domain/Tests/AbstractTestAlgorithm.h"
#include "Params.h"

namespace Domain::Tests::Main {

class Algorithm : public AbstractTestAlgorithm
{
    Q_OBJECT

public:
    explicit Algorithm(QObject* parent = nullptr)
        : AbstractTestAlgorithm(parent)
    {
    }

    void run() override;
    void setParameters(Params& parameters);

private:
    Params m_params;

signals:
    void requestSensorRawValue(quint16& value);

    void dublSeries(); // Для графиков
    void backwardStrokeStarted();// Для Analyzer

    void processCompleted();
};

}