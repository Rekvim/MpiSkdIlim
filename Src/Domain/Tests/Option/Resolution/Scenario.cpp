#include "Scenario.h"

#include "Runner.h"
#include "Analyzer.h"

#include <QDebug>

namespace Domain::Tests::Option::Resolution {

Scenario::Scenario(Tests::Context context,
                   const Params& params,
                   QObject* parent)
    : Tests::AbstractScenario(parent)
    , m_context(context)
    , m_params(params)
{
}

Scenario::~Scenario() = default;

void Scenario::beforeStart()
{
    m_analyzer = std::make_unique<Analyzer>();

    Analyzer::Config cfg;
    cfg.normalOpen =
        m_context.config.safePosition == SafePosition::NormallyOpen;

    m_analyzer->setConfig(cfg);
    m_analyzer->start();
}

std::unique_ptr<BaseRunner> Scenario::createRunner()
{
    const bool normalOpen =
        m_context.config.safePosition == SafePosition::NormallyOpen;

    return std::make_unique<Runner>(
        m_context.device,
        normalOpen,
        m_params,
        this
        );
}

void Scenario::afterRunnerCreated(BaseRunner& baseRunner)
{
    auto& runner = static_cast<Runner&>(baseRunner);

    connect(&runner, &Runner::processCompleted,
            this, &Scenario::onProcessCompleted,
            Qt::DirectConnection);
}

void Scenario::onSample(const Measurement::Sample& sample)
{
    if (m_analyzer)
        m_analyzer->onSample(sample);
}

void Scenario::onProcessCompleted()
{
    if (!m_analyzer) {
        qWarning() << "[Resolution::Scenario] Analyzer is null";
        return;
    }

    m_analyzer->finish();

    const Result& result = m_analyzer->result();

    m_context.telemetry.testResolution = result;

    emit resolutionResultUpdated(result);
    emit telemetryUpdated(m_context.telemetry);
}
}