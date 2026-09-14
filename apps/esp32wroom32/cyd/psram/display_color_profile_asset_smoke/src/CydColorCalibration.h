#pragma once

#include "brick/core/display/Rgb565ColorProfile.h"

namespace cyd_color_profile
{

inline brick::core::display::Rgb565ColorProfile profile()
{
    brick::core::display::Rgb565ColorProfile value{};
    value.red = { 0U, 0U, 0U, 0U, 0U, 1U, 1U, 1U, 2U, 2U, 3U, 4U, 4U, 5U, 6U, 7U, 7U, 8U, 9U, 11U, 12U, 13U, 14U, 15U, 17U, 18U, 20U, 21U, 23U, 24U, 26U, 28U };
    value.green = { 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 1U, 1U, 1U, 1U, 1U, 2U, 2U, 2U, 3U, 3U, 4U, 4U, 4U, 5U, 6U, 6U, 7U, 7U, 8U, 9U, 10U, 10U, 11U, 12U, 13U, 14U, 15U, 16U, 17U, 18U, 19U, 20U, 22U, 23U, 24U, 26U, 27U, 28U, 30U, 31U, 33U, 34U, 36U, 38U, 39U, 41U, 43U, 45U, 47U, 49U, 51U, 53U, 55U, 57U, 59U, 61U };
    value.blue = { 0U, 0U, 0U, 0U, 0U, 0U, 0U, 1U, 1U, 1U, 1U, 2U, 2U, 2U, 3U, 3U, 4U, 5U, 5U, 6U, 7U, 8U, 9U, 9U, 10U, 12U, 13U, 14U, 15U, 16U, 18U, 19U };
    return value;
}

}  // namespace cyd_color_profile
