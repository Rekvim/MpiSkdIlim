#pragma once

#include "IReportBlock.h"

class ResponseBlock : public IReportBlock {
public:

    struct Layout {
        QString sheet;

        quint16 upSelectedRow;
        quint16 upActualRow;
        quint16 upAllowedErrorRow;
        quint16 upErrorRow;

        quint16 downSelectedRow;
        quint16 downActualRow;
        quint16 downAllowedErrorRow;
        quint16 downErrorRow;

        QVector<quint16> stepCols;
    };

    explicit ResponseBlock(Layout layout)
        : m_layout(std::move(layout))
    {}

    void build(ReportWriter& writer,
               const ReportContext& ctx) override
    {
        const OptionDirectionResults& results =
            ctx.telemetry.responseResults;

        writeDirection(writer,
                       results,
                       results.upward,
                       m_layout.upSelectedRow,
                       m_layout.upActualRow,
                       m_layout.upAllowedErrorRow,
                       m_layout.upErrorRow,
                       results.baseActual);

        writeDirection(writer,
                       results,
                       results.downward,
                       m_layout.downSelectedRow,
                       m_layout.downActualRow,
                       m_layout.downAllowedErrorRow,
                       m_layout.downErrorRow,
                       results.baseActual_two);

        if (!m_layout.stepCols.isEmpty() &&
            std::isfinite(results.baseActual) &&
            std::isfinite(results.baseActual_two))
        {
            const double gis =
                qAbs(results.baseActual - results.baseActual_two);

            writer.cell(m_layout.sheet,
                        m_layout.downErrorRow + 1,
                        m_layout.stepCols.first(),
                        QString::number(gis, 'f', 2));
        }
    }

private:
    double allowedErrorForStep(int stepIndex) const
    {
        switch (stepIndex) {
        case 0:
        case 1:
            return 15.0;
        case 2:
            return 25.0;
        case 3:
            return 50.0;
        default:
            return std::numeric_limits<double>::quiet_NaN();
        }
    }

    void writeDirection(ReportWriter& writer,
                        const OptionDirectionResults& results,
                        const QVector<OptionStepRecord>& directionData,
                        quint16 selectedRow,
                        quint16 actualRow,
                        quint16 allowedErrorRow,
                        quint16 errorRow,
                        double baseActualValue)
    {
        if (m_layout.stepCols.isEmpty())
            return;

        const quint16 baseCol = m_layout.stepCols.first();

        writer.cell(m_layout.sheet, selectedRow, baseCol,
                    std::isfinite(results.baseTask)
                        ? QString::number(results.baseTask, 'f', 2)
                        : "");

        writer.cell(m_layout.sheet, actualRow, baseCol,
                    std::isfinite(baseActualValue)
                        ? QString::number(baseActualValue, 'f', 2)
                        : "");

        writer.cell(m_layout.sheet, allowedErrorRow, baseCol, "");
        writer.cell(m_layout.sheet, errorRow, baseCol, "");

        const int stepCount =
            std::min(directionData.size(),
                     m_layout.stepCols.size() - 1);

        for (int i = 0; i < stepCount; ++i)
        {
            const auto& step = directionData[i];
            const quint16 col = m_layout.stepCols[i + 1];

            const double allowedError = allowedErrorForStep(i);

            writer.cell(m_layout.sheet,
                        allowedErrorRow,
                        col,
                        std::isfinite(allowedError)
                            ? QString::number(allowedError, 'f', 2)
                            : "");

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

            writer.cell(m_layout.sheet,
                        actualRow,
                        col,
                        QString::number(step.actualPercent, 'f', 2));

            const bool isOk =
                std::isfinite(allowedError) &&
                qAbs(step.stepErrorPercent) <= allowedError;

            const QColor color =
                isOk ? QColor(198, 239, 206)
                     : QColor(255, 199, 206);

            writer.coloredCell(m_layout.sheet,
                               errorRow,
                               col,
                               QString::number(step.stepErrorPercent, 'f', 2),
                               color);
        }
    }

private:
    Layout m_layout;
};