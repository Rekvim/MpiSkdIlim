#pragma once

#include <QString>

namespace Report {

class Writer;
struct Context;

namespace Blocks {

class Materials final {
public:
    struct Layout {
        QString sheet;
        int rowStart;
        int column;
    };

    explicit Materials(Layout layout);

    void build(Writer& writer, const Context& ctx);

private:
    Layout m_layout;
};

}

}