#pragma once

#include "IReportBlock.h"

class ResolutionBlock : public IReportBlock {
public:
    struct Layout {
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

    explicit ResolutionBlock(Layout layout);

    void build(ReportWriter& writer, const ReportContext& ctx) override;

private:
    void writeDirection(ReportWriter& writer,
                        const OptionDirectionResults& results,
                        const QVector<OptionStepRecord>& directionData,
                        quint16 selectedRow,
                        quint16 actualRow,
                        quint16 errorRow);

private:
    Layout m_layout;
};