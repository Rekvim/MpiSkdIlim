#pragma once

#include <QString>
#include <QVector>
#include "Domain/Tests/Option/Resolution/Result.h"

namespace Report {

class Writer;
struct Context;

namespace Blocks {

class ResolutionTable final {
public:
    struct Layout {
        QString sheet;

        int upSelectedRow;
        int upActualRow;
        int upErrorRow;

        int downSelectedRow;
        int downActualRow;
        int downErrorRow;

        QVector<int> stepCols;
    };

    explicit ResolutionTable(Layout layout);

    void build(Writer& writer, const Context& ctx);

private:
    void writeDirection(
        Writer& writer,
        const Domain::Tests::Option::Resolution::Result& result,
        const QVector<Domain::Tests::Option::Resolution::ResultItem>& directionData,
        int selectedRow,
        int actualRow,
        int errorRow);

private:
    Layout m_layout;
};

}
}