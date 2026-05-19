#pragma once

#include "Data.h"
#include "Storage/Telemetry.h"
#include "Storage/Registry.h"
#include "Storage/ChartImageStorage.h"

namespace Report {
    struct Context {
        const Telemetry& telemetry;
        const ObjectInfo& object;
        const ValveInfo& valve;
        const MaterialsOfComponentParts& materials;
        const OtherParameters& params;
        const ChartImageStorage& chartImages;
    };

    class Builder {
    public:
        ~Builder() = default;
        void build(
            ReportData& report,
            const Telemetry& telemetryStore,
            const ObjectInfo& objectInfo,
            const ValveInfo& valveInfo,
            const MaterialsOfComponentParts& materials,
            const OtherParameters& otherParams,
            const ChartImageStorage& chartImages
        );

        QString templatePath() const { return ":/excel/Reports/Report.xlsx"; }

    private:
        QString m_sheetStepReactionTest = "ТШР";
        QString m_sheetTechnicalInspection = "ТО";
        QString m_sheetResponse = "ТЧР";
        QString m_sheetResolution = "ТРС";
    };
}