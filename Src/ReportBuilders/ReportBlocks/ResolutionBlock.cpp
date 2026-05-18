#include "ResolutionBlock.h"

ResolutionBlock::ResolutionBlock(Layout layout)
    : m_layout(std::move(layout))
{}

void ResolutionBlock::build(ReportWriter& writer, const ReportContext& ctx)
{
    const OptionDirectionResults& results =
        ctx.telemetry.resolutionResults;

    writeDirection(writer,
                   results,
                   results.upward,
                   m_layout.upSelectedRow,
                   m_layout.upActualRow,
                   m_layout.upErrorRow);

    writeDirection(writer,
                   results,
                   results.downward,
                   m_layout.downSelectedRow,
                   m_layout.downActualRow,
                   m_layout.downErrorRow);
}

void ResolutionBlock::writeDirection(ReportWriter& writer,
                    const OptionDirectionResults& results,
                    const QVector<OptionStepRecord>& directionData,
                    quint16 selectedRow,
                    quint16 actualRow,
                    quint16 errorRow)
{
    if (m_layout.stepCols.isEmpty())
        return;

    const quint16 baseCol = m_layout.stepCols.first();

    writer.cell(m_layout.sheet,
                actualRow,
                baseCol,
                std::isfinite(results.baseActual)
                    ? QString::number(results.baseActual, 'f', 2)
                    : "");

    writer.cell(m_layout.sheet,
                selectedRow,
                baseCol,
                std::isfinite(results.baseTask)
                    ? QString::number(results.baseTask, 'f', 2)
                    : "");

    writer.cell(m_layout.sheet, errorRow, baseCol, "");

    const int stepCount =
        std::min(directionData.size(),
                 m_layout.stepCols.size() - 1);

    for (int i = 0; i < stepCount; ++i)
    {
        const auto& step = directionData[i];
        const quint16 col = m_layout.stepCols[i + 1];

        if (!std::isfinite(step.selectedPercent) ||
            !std::isfinite(step.actualPercent) ||
            !std::isfinite(step.stepErrorPercent))
        {
            writer.cell(m_layout.sheet, selectedRow, col, "");
            writer.cell(m_layout.sheet, actualRow, col, "");
            writer.cell(m_layout.sheet, errorRow, col, "");
            continue;
        }

        const double delta =
            step.selectedPercent - results.baseTask;

        const QChar sign =
            delta >= 0.0 ? '+' : '-';

        const QString selectedText =
            std::isfinite(results.baseTask)
                ? QString("%1 %2 %3")
                      .arg(results.baseTask, 0, 'f', 2)
                      .arg(sign)
                      .arg(std::abs(delta), 0, 'f', 2)
                : QString::number(step.selectedPercent, 'f', 2);

        writer.cell(m_layout.sheet, selectedRow, col, selectedText);
        writer.cell(m_layout.sheet, actualRow, col,
                    QString::number(step.actualPercent, 'f', 2));
        writer.cell(m_layout.sheet, errorRow, col,
                    QString::number(step.stepErrorPercent, 'f', 2));
    }
}