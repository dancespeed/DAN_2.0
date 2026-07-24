#pragma once

#include <stdint.h>

namespace dan::platform::SystemClockConfig
{
    constexpr uint32_t CpuFrequencyHz = F_CPU;

    constexpr uint16_t TimerPrescaler = 64;
    constexpr uint32_t TickFrequencyHz = 1000;

    constexpr uint32_t TimerCompareValue =
        (CpuFrequencyHz / TimerPrescaler / TickFrequencyHz) - 1;

    static_assert(
        TimerCompareValue <= 255,
        "Timer2 compare value does not fit into 8 bits");
}