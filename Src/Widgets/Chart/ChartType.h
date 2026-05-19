#pragma once

#include <QMetaType>

namespace Widgets::Chart {
    enum class ChartType {
        None,
        Task,
        Pressure,
        Friction,
        Response,
        Resolution,
        Stroke,
        Step,
        Trend
    };
}
Q_DECLARE_METATYPE(Widgets::Chart::ChartType)
