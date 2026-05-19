#pragma once

#include "Storage/Telemetry.h"
#include <QTableWidget>

namespace Ui {
class MainWindow;
}

class TelemetryUiMapper
{
public:
    TelemetryUiMapper(Ui::MainWindow* ui) : m_ui(ui) { }

    void updateInit(const InitState& init);

    void updateMainTest(const Domain::Tests::Main::Result& mainResult,
                        const ValveStrokeRecord& strokeResult);

    void updateStrokeTest(const Domain::Tests::Stroke::Result& result);

    void updateStepTest(const Domain::Tests::Option::Step::Result& result);

    void updateCrossingValues(const Domain::Tests::Main::Result& mainResult,
                              const ValveStrokeRecord& strokeResult);

    void updateResolutionTest(
        const Domain::Tests::Option::Resolution::Result& result);

    void updateResponseTest(
        const Domain::Tests::Option::Response::Result& result);

private:
    void setupOptionCell(QTableWidget* table, int row, int col, const QString& text);

    void setupColoredOptionCell(QTableWidget* table, int row, int col, const QString& text, const QColor& color);

    Ui::MainWindow* m_ui;
};