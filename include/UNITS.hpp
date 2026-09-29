//
// Created on 9/29/2026.
//

#pragma once

/**
 * @details
 * deci    d	10^−1
 * centi   c	10^−2
 * milli   m	10^−3
 * micro   u	10^−6
 * nano    n	10^−9
 * pico    p	10^−12
 * femto   f	10^−15
 */
namespace UNITS {
    constexpr uint_fast32_t MICRO_PER_MILLI = 1000;
    constexpr uint_fast32_t MILLI_PER_NANO = 1000;
    constexpr uint_fast32_t NANO_PER_PICO = 1000;
}
