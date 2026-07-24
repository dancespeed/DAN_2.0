#pragma once

#include <avr/io.h>

namespace SystemConfig
{
    constexpr uint32_t CpuFrequencyHz = 16000000UL;
    constexpr uint16_t Timer2Prescaler = 64;
    constexpr uint16_t Timer2CompareValue = (CpuFrequencyHz / Timer2Prescaler / 1000) - 1;

    //PC1 = heartbeat output
    constexpr uint8_t HeartbeatBit = PC1;

    constexpr uint16_t HeartbeatToggleMs = 500;
}