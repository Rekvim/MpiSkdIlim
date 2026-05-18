#pragma once

#include "ReportSaver.h"

class ReportWriter {
public:
    explicit ReportWriter(ReportSaver::Report& report)
        : m_report(report) {}

    void cell(const QString& sheet,
              quint16 row,
              quint16 col,
              const QVariant& value)
    {
        m_report.data.push_back({sheet, row, col, value.toString()});
    }

    void image(const QString& sheet,
               quint16 row,
               quint16 col,
               const QImage& img)
    {
        m_report.images.push_back({sheet, row, col, img});
    }

    void validation(const QString& sheet,
                    const QString& formula,
                    const QString& range)
    {
        m_report.validation.push_back({sheet, formula, range});
    }

    void coloredCell(const QString& sheet,
                     quint16 row,
                     quint16 col,
                     const QVariant& value,
                     const QColor& backgroundColor)
    {
        ReportSaver::ReportData data;
        data.sheet = sheet;
        data.row = row;
        data.col = col;
        data.value = value.toString();
        data.backgroundColor = backgroundColor;
        data.hasBackgroundColor = true;

        m_report.data.push_back(data);
    }

private:
    ReportSaver::Report& m_report;
};
