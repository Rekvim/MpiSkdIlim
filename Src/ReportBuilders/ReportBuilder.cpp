#include "Src/ReportBuilders/ReportBuilder.h"
#include "Src/ReportBuilders/ReportWriter.h"

#include "Src/ReportBuilders/ReportBlocks/ObjectInfoBlock.h"
#include "Src/ReportBuilders/ReportBlocks/ValveSpecBlock.h"
#include "Src/ReportBuilders/ReportBlocks/MaterialsBlock.h"
#include "Src/ReportBuilders/ReportBlocks/StepReactionBlock.h"
#include "Src/ReportBuilders/ReportBlocks/TechnicalResultsBlock.h"
#include "Src/ReportBuilders/ReportBlocks/ResponseBlock.h"
#include "Src/ReportBuilders/ReportBlocks/ResolutionBlock.h"

void ReportBuilder::buildReport(
    ReportSaver::Report &report,
    const TelemetryStore &telemetryStore,
    const ObjectInfo &objectInfo,
    const ValveInfo &valveInfo,
    const OtherParameters &otherParams,
    const MaterialsOfComponentParts &materialsOfComponentParts,
    const QImage &imageChartTask,
    const QImage &imageChartPressure,
    const QImage &imageChartFriction,
    const QImage &imageChartResponse,
    const QImage &imageChartResolution,
    const QImage &imageChartStep
    )
{ // 168
    ReportWriter writer(report);

    ReportContext ctx{
        telemetryStore,
        objectInfo,
        valveInfo,
        otherParams,
        materialsOfComponentParts,
        imageChartTask,
        imageChartPressure,
        imageChartFriction,
        imageChartResponse,
        imageChartResolution,
        imageChartStep
    };

    writer.cell(m_sheetStepReactionTest, 1, 9, ctx.valve.positionNumber);

    ObjectInfoBlock({m_sheetStepReactionTest, 4, 4}).build(writer, ctx);
    ValveSpecBlock({m_sheetStepReactionTest, 4, 13}).build(writer, ctx);
    StepReactionBlock({m_sheetStepReactionTest,
                          18,  // imageRow
                          2,   // imageCol
                          55,  // startRow
                          3,   // firstBaseCol
                          10   // secondBaseCol
                      }).build(writer, ctx);

    writer.cell(m_sheetStepReactionTest, 67, 12, ctx.params.date);

    ObjectInfoBlock({m_sheetTechnicalInspection, 5, 4}).build(writer, ctx);
    ValveSpecBlock({m_sheetTechnicalInspection, 5, 13}).build(writer, ctx);
    MaterialsBlock({m_sheetTechnicalInspection, 11, 4}).build(writer, ctx);

    TechnicalResultsBlock({m_sheetTechnicalInspection,
                              28, // rowStart
                              5, // colFact
                              8, // colNorm
                              11, // colResult
                              50 // rowStrokeTime
    }).build(writer, ctx);

    writer.validation(m_sheetTechnicalInspection, "=Заключение!$B$1:$B$4", "E40");
    writer.validation(m_sheetTechnicalInspection, "=Заключение!$C$1:$C$3", "E42");
    writer.validation(m_sheetTechnicalInspection, "=Заключение!$E$1:$E$4", "E44");
    writer.validation(m_sheetTechnicalInspection, "=Заключение!$D$1:$D$5", "E46");
    writer.validation(m_sheetTechnicalInspection, "=Заключение!$F$3", "E48");

    writer.validation(m_sheetTechnicalInspection, "=ЗИП!$A$1:$A$37", "J54:J63");

    // Дата и Исполнитель
    writer.cell(m_sheetTechnicalInspection, 64, 12, ctx.params.date);
    writer.cell(m_sheetTechnicalInspection, 73, 4, ctx.object.FIO);

    // Страница: Отчет; Блок: Диагностические графики
    writer.image(m_sheetTechnicalInspection, 82, 1, imageChartTask);
    writer.image(m_sheetTechnicalInspection, 110, 1, imageChartPressure);
    writer.image(m_sheetTechnicalInspection, 138, 1, imageChartFriction);

    // Страница: Отчет; Блок: Дата
    writer.cell(m_sheetTechnicalInspection, 164, 12, ctx.params.date);

    writer.cell(m_sheetResponse, 1, 9, ctx.valve.positionNumber);
    ObjectInfoBlock({m_sheetResponse, 4, 4}).build(writer, ctx);
    ValveSpecBlock({m_sheetResponse, 4, 13}).build(writer, ctx);
    writer.image(m_sheetResponse, 18, 2, ctx.chartResponse);
    ResponseBlock::Layout responseLayout {
        .sheet = m_sheetResponse,
        .upSelectedRow = 55,
        .upActualRow =56,
        .upAllowedErrorRow = 57,
        .upErrorRow =58,

        .downSelectedRow = 62,
        .downActualRow = 63,
        .downAllowedErrorRow = 64,
        .downErrorRow = 65,
        .stepCols = {5, 6, 10, 12, 14}
    };
    ResponseBlock(responseLayout).build(writer, ctx);
    writer.cell(m_sheetResponse, 69, 12, ctx.params.date);

    writer.cell(m_sheetResolution, 1, 9, ctx.valve.positionNumber);
    ObjectInfoBlock({m_sheetResolution, 4, 4}).build(writer, ctx);
    ValveSpecBlock({m_sheetResolution, 4, 13}).build(writer, ctx);
    writer.image(m_sheetResolution, 18, 2, ctx.chartResolution);
    ResolutionBlock::Layout resolutionLayout {
        .sheet = m_sheetResolution,
        .upSelectedRow = 55,
        .upActualRow =56,
        .upErrorRow =57,

        .downSelectedRow = 61,
        .downActualRow = 62,
        .downErrorRow = 63,
        .stepCols = {5, 6, 10, 12, 14}
    };
    ResolutionBlock(resolutionLayout).build(writer, ctx);

    writer.cell(m_sheetResolution, 67, 12, ctx.params.date);

    // // Страница: Отчет; Блок: Диагностические графики
    // writer.cell(m_sheetGraphsOptionalTests, 1, 13, ctx.valve.positionNumber);

    // writer.image(m_sheetGraphsOptionalTests, 5, 1, imageChartResponse);
    // writer.image(m_sheetGraphsOptionalTests, 30, 1, imageChartResolution);
    // writer.image(m_sheetGraphsOptionalTests, 55, 1, imageChartStep);

    // // Страница: Отчет; Блок: Дата
    // writer.cell(m_sheetGraphsOptionalTests, 80, 12, ctx.params.date);
}
