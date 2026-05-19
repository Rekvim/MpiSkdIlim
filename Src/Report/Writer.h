#pragma once

#include "Data.h"

#include <QVariant>
#include <QColor>

namespace Report {

class Writer {
public:
    explicit Writer(ReportData& report);

    void cell(const QString& sheet,
              quint16 row,
              quint16 col,
              const QVariant& value);

    void image(const QString& sheet,
               quint16 row,
               quint16 col,
               const QImage& img);

    void validation(const QString& formula,
                    const QString& range);

    void coloredCell(const QString& sheet,
                     quint16 row,
                     quint16 col,
                     const QVariant& value,
                     const QColor& backgroundColor);

private:
    ReportData& m_report;
};

}