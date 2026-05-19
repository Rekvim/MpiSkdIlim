#include "Response.h"

#include "Report/Writer.h"
#include "Report/Builder.h"

#include "Report/Blocks/ObjectInfo.h"
#include "Report/Blocks/ValveSpec.h"
#include "Report/Blocks/ResponseTable.h"

#include <utility>

namespace Report::Pages {

Response::Response(Layout layout)
    : m_layout(std::move(layout))
{}

void Response::build(Writer& writer, const Context& ctx)
{
    if (m_layout.positionRow > 0) {
        writer.cell(m_layout.sheet,
                    m_layout.positionRow,
                    m_layout.positionColumn,
                    ctx.valve.positionNumber);
    }

    Blocks::ObjectInfo({
        m_layout.sheet,
        m_layout.objectInfoRow,
        m_layout.objectInfoColumn
    }).build(writer, ctx);

    Blocks::ValveSpec({
        m_layout.sheet,
        m_layout.valveSpecRow,
        m_layout.valveSpecColumn,
        m_layout.positionerModel,
        m_layout.includeSolenoid
    }).build(writer, ctx);

    QVector<int> stepCols;
    stepCols.reserve(m_layout.stepCount);

    for (int i = 0; i < m_layout.stepCount; ++i)
        stepCols.push_back(m_layout.firstStepColumn + i);

    Blocks::ResponseTable({
        m_layout.sheet,

        m_layout.upSelectedRow,
        m_layout.upActualRow,
        m_layout.upAllowedErrorRow,
        m_layout.upErrorRow,

        m_layout.downSelectedRow,
        m_layout.downActualRow,
        m_layout.downAllowedErrorRow,
        m_layout.downErrorRow,

        m_layout.gisRow,

        stepCols
    }).build(writer, ctx);

    if (m_layout.dateRow > 0) {
        writer.cell(m_layout.sheet,
                    m_layout.dateRow,
                    m_layout.dateColumn,
                    ctx.params.date);
    }
}

}