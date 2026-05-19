#pragma once

#include <QString>

namespace Report {

class Writer;
struct Context;

namespace Pages {

class Resolution final {
public:
    struct Layout {
        QString sheet;

        int positionRow = 0;
        int positionColumn = 9;

        int objectInfoRow = 0;
        int objectInfoColumn = 4;

        int valveSpecRow = 0;
        int valveSpecColumn = 13;

        int dateRow = 0;
        int dateColumn = 12;

        int upSelectedRow = 0;
        int upActualRow = 0;
        int upErrorRow = 0;

        int downSelectedRow = 0;
        int downActualRow = 0;
        int downErrorRow = 0;

        int firstStepColumn = 0;
        int stepCount = 5;

        bool positionerModel = true;
        bool includeSolenoid = false;
    };

    explicit Resolution(Layout layout);

    void build(Writer& writer, const Context& ctx);

private:
    Layout m_layout;
};

}
}