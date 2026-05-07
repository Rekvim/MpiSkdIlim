#pragma once

#include "IReportBlock.h"

struct ResponseResolutionLayout {
    QString sheet;

    quint16 upSelectedRow;
    quint16 upActualRow;
    quint16 upErrorRow;

    quint16 downSelectedRow;
    quint16 downActualRow;
    quint16 downErrorRow;

    // колонки шагов: Шаг1..Шаг4
    QVector<quint16> stepCols;
};

class ResponseResolutionBlock : public IReportBlock {
public:
    enum class Kind {
        Response,
        Resolution
    };

    ResponseResolutionBlock(ResponseResolutionLayout layout, Kind kind)
        : m_layout(std::move(layout))
        , m_kind(kind)
    {}

    void build(ReportWriter& writer,
               const ReportContext& ctx) override
    {
        const OptionDirectionResults& results =
            (m_kind == Kind::Response)
                ? ctx.telemetry.responseResults
                : ctx.telemetry.resolutionResults;

        auto toText = [](double v) -> QString
        {
            if (!std::isfinite(v))
                return "";
            return QString::number(v, 'f', 2);
        };

        auto writeDirection =
            [&](const QVector<OptionStepRecord>& directionData,
                quint16 selectedRow,
                quint16 actualRow,
                quint16 errorRow)
        {
            if (m_layout.stepCols.isEmpty())
                return;

            const quint16 baseCol = m_layout.stepCols.first();

            const bool hasBaseTask = std::isfinite(results.baseTask);
            double baseActualValue = results.baseActual;

            if (m_kind == Kind::Response &&
                actualRow == m_layout.downActualRow)
            {
                baseActualValue = results.baseActual_two;
            }

            const bool hasBaseActual = std::isfinite(baseActualValue);

            writer.cell(m_layout.sheet,
                        actualRow,
                        baseCol,
                        hasBaseActual ? QString::number(baseActualValue, 'f', 2) : "");

            writer.cell(m_layout.sheet,
                        selectedRow,
                        baseCol,
                        hasBaseTask ? QString::number(results.baseTask, 'f', 2) : "");

            writer.cell(m_layout.sheet,
                        errorRow,
                        baseCol,
                        "");

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

                QString selectedText;

                if (hasBaseTask)
                {
                    const double base = results.baseTask;
                    const double delta = step.selectedPercent - base;
                    const QChar sign = (delta >= 0.0) ? '+' : '-';

                    selectedText = QString("%1 %2 %3")
                                       .arg(base, 0, 'f', 2)
                                       .arg(sign)
                                       .arg(std::abs(delta), 0, 'f', 2);
                }
                else
                {
                    selectedText = QString::number(step.selectedPercent, 'f', 2);
                }

                writer.cell(m_layout.sheet,
                            selectedRow,
                            col,
                            selectedText);

                writer.cell(m_layout.sheet,
                            actualRow,
                            col,
                            QString::number(step.actualPercent, 'f', 2));

                writer.cell(m_layout.sheet,
                            errorRow,
                            col,
                            QString::number(step.stepErrorPercent, 'f', 2));
            }

            if (m_kind == Kind::Response &&
                actualRow == m_layout.downActualRow)
            {
                double gis = qAbs(results.baseActual - results.baseActual_two);

                writer.cell(m_layout.sheet,
                            errorRow + 1,
                            baseCol,
                            QString::number(gis, 'f', 2));
            }
        };
        writeDirection(results.upward,
                       m_layout.upSelectedRow,
                       m_layout.upActualRow,
                       m_layout.upErrorRow);

        writeDirection(results.downward,
                       m_layout.downSelectedRow,
                       m_layout.downActualRow,
                       m_layout.downErrorRow);
    }

private:
    ResponseResolutionLayout m_layout;
    Kind m_kind;
};
