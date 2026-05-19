#include "Builder.h"

#include "Report/Pages/TechnicalInspection.h"
#include "Report/Pages/StepReaction.h"
#include "Report/Pages/Resolution.h"
#include "Report/Pages/Response.h"

#include "Report/Writer.h"

namespace Report {
void Builder::build(
    ReportData& report,
    const Telemetry& telemetryStore,
    const ObjectInfo& objectInfo,
    const ValveInfo& valveInfo,
    const MaterialsOfComponentParts& materials,
    const OtherParameters& otherParams,
    const ChartImageStorage& chartImages
    )
{
    Writer writer(report);

    Context ctx{
        telemetryStore,
        objectInfo,
        valveInfo,
        materials,
        otherParams,
        chartImages
    };

    {
        Pages::TechnicalInspection::Layout layout;
        layout.sheet = m_sheetTechnicalInspection;

        layout.positionRow = 1;

        layout.objectInfoRow = 5;
        layout.valveSpecRow = 5;
        layout.materialsRow = 11;
        layout.technicalResultsRow = 28;
        layout.strokeTimeRow = 50;

        layout.firstDateRow = 64;
        layout.fioRow = 72;

        layout.taskImageRow = 82;
        layout.pressureImageRow = 110;
        layout.frictionImageRow = 138;

        layout.secondDateRow = 164;

        layout.positionerModel = true;
        layout.includeSolenoid = false;

        Pages::TechnicalInspection(layout).build(writer, ctx);
    }

    {
        Pages::StepReaction::Layout layout;
        layout.sheet = m_sheetStepReactionTest;

        layout.positionRow = 1;

        layout.objectInfoRow = 4;
        layout.valveSpecRow = 4;

        layout.imageRow = 19;
        layout.tableStartRow = 55;

        layout.dateRow = 67;

        layout.positionerModel = true;
        layout.includeSolenoid = false;

        Pages::StepReaction(layout).build(writer, ctx);
    }

    Report::Pages::Response({
                             m_sheetResponse,

                                1, 9,      // position
                                4, 4,      // object info
                                4, 13,     // valve spec
                                69, 12,    // date

                                55,        // upSelectedRow
                                56,        // upActualRow
                                57,        // upAllowedErrorRow
                                58,        // upErrorRow

                                62,        // downSelectedRow
                                63,        // downActualRow
                                64,        // downAllowedErrorRow
                                65,        // downErrorRow

                                66,        // gisRow

                                5,         // firstStepColumn
                                5,         // base + 4 steps

                                true,
                                false
                            }).build(writer, ctx);

    Report::Pages::Resolution({
                               m_sheetResolution,

                                  1, 9,      // position
                                  4, 4,      // object info
                                  4, 13,     // valve spec
                                  42, 12,    // date

                                  55,        // upSelectedRow
                                  56,        // upActualRow
                                  57,        // upErrorRow

                                  61,        // downSelectedRow
                                  62,        // downActualRow
                                  63,        // downErrorRow

                                  5,         // firstStepColumn
                                  5,         // base + 4 steps

                                  true,
                                  false
                              }).build(writer, ctx);

    writer.validation("=ЗИП!$A$1:$A$37", "J52:J61");
    writer.validation("=Заключение!$B$1:$B$4", "E38");
    writer.validation("=Заключение!$C$1:$C$3", "E40");
    writer.validation("=Заключение!$E$1:$E$4", "E42");
    writer.validation("=Заключение!$D$1:$D$5", "E44");
    writer.validation("=Заключение!$F$3", "E46");
}
}