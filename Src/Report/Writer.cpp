#include "Writer.h"

namespace Report {

Writer::Writer(ReportData& report)
    : m_report(report)
{}

void Writer::cell(const QString& sheet,
                  quint16 row,
                  quint16 col,
                  const QVariant& value)
{
    m_report.data.push_back({sheet, row, col, value.toString()});
}

void Writer::image(const QString& sheet,
                   quint16 row,
                   quint16 col,
                   const QImage& img)
{
    m_report.images.push_back({sheet, row, col, img});
}

void Writer::validation(const QString& formula,
                        const QString& range)
{
    m_report.validation.push_back({formula, range});
}

void Writer::coloredCell(const QString& sheet,
                         quint16 row,
                         quint16 col,
                         const QVariant& value,
                         const QColor& backgroundColor)
{
    CellData cellData;
    cellData.sheet = sheet;
    cellData.row = row;
    cellData.col = col;
    cellData.value = value.toString();
    cellData.backgroundColor = backgroundColor;
    cellData.hasBackgroundColor = true;

    m_report.data.push_back(cellData);
}

}