#pragma once

#include <avr/io.h>
#include <stdint.h>

namespace dan::platform::HeartbeatConfig
{
    /*
     * Arduino Nano / ATmega328P:
     *
     * PC1 соответствует аналоговому выводу A1.
     */
    constexpr uint8_t PortBit = PC1;

    constexpr uint32_t TogglePeriodMs = 500;
}