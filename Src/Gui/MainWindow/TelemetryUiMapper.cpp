#include "TelemetryUiMapper.h"
#include "ui_MainWindow.h"

#include <QTableWidgetItem>
#include <QTime>

void TelemetryUiMapper::updateInit(const InitState& init)
{
    m_ui->label_deviceStatusValue->setText(init.deviceStatusText);
    m_ui->label_deviceStatusValue->setStyleSheet(
        "color:" + init.deviceStatusColor.name(QColor::HexRgb));

    m_ui->label_deviceInitValue->setText(init.initStatusText);
    m_ui->label_deviceInitValue->setStyleSheet(
        "color:" + init.initStatusColor.name(QColor::HexRgb));

    m_ui->label_connectedSensorsNumber->setText(init.connectedSensorsText);
    m_ui->label_connectedSensorsNumber->setStyleSheet(
        "color:" + init.connectedSensorsColor.name(QColor::HexRgb));

    m_ui->label_startingPositionValue->setText(init.startingPositionText);
    m_ui->label_startingPositionValue->setStyleSheet(
        "color:" + init.startingPositionColor.name(QColor::HexRgb));

    m_ui->label_finalPositionValue->setText(init.finalPositionText);
    m_ui->label_finalPositionValue->setStyleSheet(
        "color:" + init.finalPositionColor.name(QColor::HexRgb));
}

void TelemetryUiMapper::updateStrokeTest(const Domain::Tests::Stroke::Result& result)
{
    m_ui->lineEdit_strokeTest_forwardTime->setText(
        QTime(0, 0).addMSecs(result.forwardTimeMs).toString("mm:ss.zzz"));

    m_ui->lineEdit_resultsTable_strokeTest_forwardTime->setText(
        QTime(0, 0).addMSecs(result.forwardTimeMs).toString("mm:ss.zzz"));

    m_ui->lineEdit_strokeTest_backwardTime->setText(
        QTime(0, 0).addMSecs(result.backwardTimeMs).toString("mm:ss.zzz"));

    m_ui->lineEdit_resultsTable_strokeTest_backwardTime->setText(
        QTime(0, 0).addMSecs(result.backwardTimeMs).toString("mm:ss.zzz"));

    m_ui->lineEdit_strokeTest_respondingReactionbackwardTime->setText(
        QTime(0, 0).addMSecs(result.backwardSignalDelayMs).toString("mm:ss.zzz"));

    m_ui->lineEdit_strokeTest_respondingReactionforwardTime->setText(
        QTime(0, 0).addMSecs(result.forwardSignalDelayMs).toString("mm:ss.zzz"));
}

void TelemetryUiMapper::updateMainTest(
    const Domain::Tests::Main::Result& mainResult,
    const ValveStrokeRecord& strokeResult)
{
    m_ui->label_pressureDifferenceValue->setText(
        QString("%1").arg(mainResult.pressureDiff, 0, 'f', 3));

    m_ui->label_frictionForceValue->setText(
        QString("%1").arg(mainResult.frictionForce, 0, 'f', 3));

    m_ui->label_frictionPercentValue->setText(
        QString("%1").arg(mainResult.frictionPercent, 0, 'f', 2));

    m_ui->lineEdit_resultsTable_frictionForceValue->setText(
        QString("%1").arg(mainResult.frictionForce, 0, 'f', 3));

    m_ui->lineEdit_resultsTable_frictionPercentValue->setText(
        QString("%1").arg(mainResult.frictionPercent, 0, 'f', 2));

    m_ui->label_dynamicErrorMeanPercent->setText(
        QString("%1 %").arg(mainResult.dynamicErrorMeanPercent, 0, 'f', 2));

    m_ui->label_dynamicErrorMean->setText(
        QString("%1 мА").arg(mainResult.dynamicErrorMean, 0, 'f', 3));

    m_ui->label_dynamicErrorMaxPercent->setText(
        QString("%1 %")
            .arg(mainResult.dynamicErrorMaxPercent, 0, 'f', 2)
        );

    m_ui->label_dynamicErrorMax->setText(
        QString("%1 мА").arg(mainResult.dynamicErrorMax, 0, 'f', 3));

    m_ui->lineEdit_resultsTable_dynamicErrorReal->setText(
        QString("%1").arg(mainResult.dynamicErrorReal, 0, 'f', 2));

    m_ui->lineEdit_resultsTable_strokeReal->setText(
        QString("%1").arg(strokeResult.real, 0, 'f', 2));

    m_ui->label_lowLimitValue->setText(
        QString("%1").arg(mainResult.lowLimitPressure));

    m_ui->label_highLimitValue->setText(
        QString("%1").arg(mainResult.highLimitPressure));

    m_ui->lineEdit_resultsTable_rangePressure->setText(
        QString("%1–%2")
            .arg(mainResult.lowLimitPressure, 0, 'f', 2)
            .arg(mainResult.highLimitPressure, 0, 'f', 2)
        );

    m_ui->lineEdit_resultsTable_driveRangeReal->setText(
        QString("%1–%2")
            .arg(mainResult.springLow, 0, 'f', 2)
            .arg(mainResult.springHigh, 0, 'f', 2)
        );

    m_ui->label_valveStroke_range->setText(strokeResult.range);
}

void TelemetryUiMapper::updateCrossingValues(
    const Domain::Tests::Main::Result& mainResult,
    const ValveStrokeRecord& strokeResult)
{
    m_ui->lineEdit_crossingLimits_dynamicError_value->setText(
        QString::number(mainResult.dynamicErrorReal, 'f', 2));

    m_ui->lineEdit_crossingLimits_linearCharacteristic_value->setText(
        QString::number(mainResult.linearityError, 'f', 2));

    m_ui->lineEdit_crossingLimits_range_value->setText(
        QString::number(strokeResult.real, 'f', 2));

    m_ui->lineEdit_crossingLimits_spring_value->setText(
        QString("%1–%2")
            .arg(mainResult.springLow, 0, 'f', 2)
            .arg(mainResult.springHigh, 0, 'f', 2));

    m_ui->lineEdit_crossingLimits_coefficientFriction_value->setText(
        QString::number(mainResult.frictionPercent, 'f', 2));
}

void TelemetryUiMapper::updateStepTest(const Domain::Tests::Option::Step::Result& result)
{
    const auto& steps = result.steps;

    m_ui->tableWidget_stepResults->clearContents();

    m_ui->tableWidget_stepResults->setColumnCount(2);
    m_ui->tableWidget_stepResults->setHorizontalHeaderLabels({
        QObject::tr("T%1").arg(result.testValue),
        QObject::tr("Перерегулирование")
    });

    m_ui->tableWidget_stepResults->setRowCount(steps.size());

    QStringList rowNames;
    rowNames.reserve(steps.size());

    for (int i = 0; i < steps.size(); ++i)
    {
        const auto& step = steps.at(i);

        const QString time = step.T_value == 0
                                 ? QObject::tr("Ошибка")
                                 : QTime(0, 0)
                                       .addMSecs(step.T_value)
                                       .toString("mm:ss.zzz");

        const QString overshoot =
            QString("%1%").arg(step.overshoot, 4, 'f', 2);

        const QString rowName =
            QString("%1-%2").arg(step.from).arg(step.to);

        m_ui->tableWidget_stepResults->setItem(
            i, 0, new QTableWidgetItem(time));

        m_ui->tableWidget_stepResults->setItem(
            i, 1, new QTableWidgetItem(overshoot));

        rowNames << rowName;
    }

    m_ui->tableWidget_stepResults->setVerticalHeaderLabels(rowNames);
    m_ui->tableWidget_stepResults->resizeColumnsToContents();
}

void TelemetryUiMapper::setupOptionCell(QTableWidget* table,
                                        int row,
                                        int col,
                                        const QString& text)
{
    auto* item = new QTableWidgetItem(text);
    item->setTextAlignment(Qt::AlignCenter);

    if (text == "Восходящее движение") {
        item->setBackground(QColor(Qt::yellow));
    } else if (text == "Нисходящее движение" ||
               text == "Ошибка по положению, %") {
        item->setBackground(QColor(QStringLiteral("#fd7d13")));
    } else if (text == "Задание, %" ||
               text == "Факт, %" ||
               text == "Допустимая ошибка, %" ||
               text == "Ошибка на шаг, %") {
        item->setBackground(QColor(Qt::green));
    }

    table->setItem(row, col, item);
}

void TelemetryUiMapper::setupColoredOptionCell(QTableWidget* table,
                                               int row,
                                               int col,
                                               const QString& text,
                                               const QColor& color)
{
    auto* item = new QTableWidgetItem(text);
    item->setTextAlignment(Qt::AlignCenter);
    item->setBackground(color);
    table->setItem(row, col, item);
}

void TelemetryUiMapper::updateResolutionTest(
    const Domain::Tests::Option::Resolution::Result& result)
{
    auto* table = m_ui->tableWidget_resolutionResults;

    const int steps = std::max(result.upward.size(),
                               result.downward.size());

    const int cols = steps + 2;

    const int upTitleRow = 0;
    const int upHeaderRow = 1;
    const int upBaseRow = 2;
    const int upErrorRow = upBaseRow + 2;

    const int downTitleRow = 5;
    const int downHeaderRow = downTitleRow + 1;
    const int downBaseRow = downHeaderRow + 1;
    const int downErrorRow = downBaseRow + 2;

    const int rows = downErrorRow + 1;

    table->clear();
    table->setRowCount(rows);
    table->setColumnCount(cols);
    table->setSelectionMode(QAbstractItemView::NoSelection);

    table->setSpan(upTitleRow, 0, 1, cols);
    setupOptionCell(table, upTitleRow, 0, "Восходящее движение");

    setupOptionCell(table, upHeaderRow, 0, "Положение хода клапана");

    for (int i = 0; i <= steps; ++i)
    {
        const QString header =
            i == 0
                ? "Задание"
                : QString("Ступенчатый шаг %1").arg(i);

        setupOptionCell(table, upHeaderRow, i + 1, header);
    }

    setupOptionCell(table, upBaseRow, 0, "Задание, %");
    setupOptionCell(table, upBaseRow + 1, 0, "Факт, %");
    setupOptionCell(table, upErrorRow, 0, "Ошибка на шаг, %");

    table->setSpan(downTitleRow, 0, 1, cols);
    setupOptionCell(table, downTitleRow, 0, "Нисходящее движение");

    setupOptionCell(table, downHeaderRow, 0, "Положение хода клапана");

    for (int i = 0; i <= steps; ++i)
    {
        const QString header =
            i == 0
                ? "Задание"
                : QString("Ступенчатый шаг %1").arg(i);

        setupOptionCell(table, downHeaderRow, i + 1, header);
    }

    setupOptionCell(table, downBaseRow, 0, "Задание, %");
    setupOptionCell(table, downBaseRow + 1, 0, "Факт, %");
    setupOptionCell(table, downErrorRow, 0, "Ошибка на шаг, %");

    auto fillDirection =
        [&](const auto& data, int baseRow)
    {
        const int errorRow = baseRow + 2;

        if (std::isfinite(result.baseTask)) {
            setupOptionCell(table,
                            baseRow,
                            1,
                            QString::number(result.baseTask, 'f', 2));
        }

        if (std::isfinite(result.baseActual)) {
            setupOptionCell(table,
                            baseRow + 1,
                            1,
                            QString::number(result.baseActual, 'f', 2));
        }

        for (int i = 0; i < data.size(); ++i)
        {
            const auto& step = data.at(i);
            const int col = i + 2;

            setupOptionCell(table, baseRow, col,
                            QString::number(step.selectedPercent, 'f', 2));

            setupOptionCell(table, baseRow + 1, col,
                            QString::number(step.actualPercent, 'f', 2));

            setupOptionCell(table, errorRow, col,
                            QString::number(step.stepErrorPercent, 'f', 2));
        }
    };

    fillDirection(result.upward, upBaseRow);
    fillDirection(result.downward, downBaseRow);

    table->resizeColumnsToContents();
}

void TelemetryUiMapper::updateResponseTest(
    const Domain::Tests::Option::Response::Result& result)
{
    auto* table = m_ui->tableWidget_responseResults;

    const int steps = std::max(result.upward.size(),
                               result.downward.size());

    const int cols = steps + 2;

    const int upTitleRow = 0;
    const int upHeaderRow = 1;
    const int upBaseRow = 2;
    const int upAllowedErrorRow = upBaseRow + 2;
    const int upErrorRow = upBaseRow + 3;

    const int downTitleRow = 6;
    const int downHeaderRow = downTitleRow + 1;
    const int downBaseRow = downHeaderRow + 1;
    const int downAllowedErrorRow = downBaseRow + 2;
    const int downErrorRow = downBaseRow + 3;

    const int gisRow = downErrorRow + 1;
    const int rows = gisRow + 1;

    table->clear();
    table->setRowCount(rows);
    table->setColumnCount(cols);
    table->setSelectionMode(QAbstractItemView::NoSelection);

    auto allowedErrorForStep = [](int stepIndex) -> double
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
    };

    table->setSpan(upTitleRow, 0, 1, cols);
    setupOptionCell(table, upTitleRow, 0, "Восходящее движение");

    setupOptionCell(table, upHeaderRow, 0, "Положение хода клапана");

    for (int i = 0; i <= steps; ++i)
    {
        const QString header =
            i == 0
                ? "Задание"
                : QString("Задание + шаг %1").arg(i);

        setupOptionCell(table, upHeaderRow, i + 1, header);
    }

    setupOptionCell(table, upBaseRow, 0, "Задание, %");
    setupOptionCell(table, upBaseRow + 1, 0, "Факт, %");
    setupOptionCell(table, upAllowedErrorRow, 0, "Допустимая ошибка, %");
    setupOptionCell(table, upErrorRow, 0, "Ошибка на шаг, %");

    table->setSpan(downTitleRow, 0, 1, cols);
    setupOptionCell(table, downTitleRow, 0, "Нисходящее движение");

    setupOptionCell(table, downHeaderRow, 0, "Положение хода клапана");

    for (int i = 0; i <= steps; ++i)
    {
        const QString header =
            i == 0
                ? "Задание"
                : QString("Задание - шаг %1").arg(i);

        setupOptionCell(table, downHeaderRow, i + 1, header);
    }

    setupOptionCell(table, downBaseRow, 0, "Задание, %");
    setupOptionCell(table, downBaseRow + 1, 0, "Факт, %");
    setupOptionCell(table, downAllowedErrorRow, 0, "Допустимая ошибка, %");
    setupOptionCell(table, downErrorRow, 0, "Ошибка на шаг, %");

    setupOptionCell(table, gisRow, 0, "Ошибка по положению, %");

    if (std::isfinite(result.baseActual) &&
        std::isfinite(result.baseActualTwo))
    {
        const double gis =
            qAbs(result.baseActual - result.baseActualTwo);

        setupOptionCell(table,
                        gisRow,
                        1,
                        QString::number(gis, 'f', 2));

        table->setSpan(gisRow, 2, 1, cols - 2);
    }

    auto fillDirection =
        [&](const auto& data,
            int baseRow,
            double baseActualValue)
    {
        const int allowedErrorRow = baseRow + 2;
        const int errorRow = baseRow + 3;

        if (std::isfinite(result.baseTask)) {
            setupOptionCell(table, baseRow, 1,
                            QString::number(result.baseTask, 'f', 2));
        }

        if (std::isfinite(baseActualValue)) {
            setupOptionCell(table, baseRow + 1, 1,
                            QString::number(baseActualValue, 'f', 2));
        }

        for (int i = 0; i < data.size(); ++i)
        {
            const auto& step = data.at(i);
            const int col = i + 2;

            QString selectedText;

            if (std::isfinite(result.baseTask)) {
                const double delta =
                    step.selectedPercent - result.baseTask;

                const QChar sign =
                    delta >= 0.0 ? '+' : '-';

                selectedText = QString("%1 %2 %3")
                                   .arg(result.baseTask, 0, 'f', 2)
                                   .arg(sign)
                                   .arg(std::abs(delta), 0, 'f', 2);
            }
            else {
                selectedText =
                    QString::number(step.selectedPercent, 'f', 2);
            }

            setupOptionCell(table, baseRow, col, selectedText);

            setupOptionCell(table,
                            baseRow + 1,
                            col,
                            QString::number(step.actualPercent, 'f', 2));

            const double allowedError =
                allowedErrorForStep(i);

            setupOptionCell(table, allowedErrorRow, col,
                            std::isfinite(allowedError)
                                ? QString::number(allowedError, 'f', 2)
                                : "");

            const bool isOk =
                std::isfinite(allowedError) &&
                std::isfinite(step.stepErrorPercent) &&
                qAbs(step.stepErrorPercent) <= allowedError;

            setupColoredOptionCell(table, errorRow, col,
                                   std::isfinite(step.stepErrorPercent)
                                       ? QString::number(step.stepErrorPercent, 'f', 2)
                                       : "",
                                   isOk
                                       ? QColor(198, 239, 206)
                                       : QColor(255, 199, 206));
        }
    };

    fillDirection(result.upward,
                  upBaseRow,
                  result.baseActual);

    fillDirection(result.downward,
                  downBaseRow,
                  result.baseActualTwo);

    table->resizeColumnsToContents();
}