#pragma once

#include <stdint.h>

using ModuleId = uint8_t;

namespace Module
{
    constexpr ModuleId None       = 0x00;

    constexpr ModuleId System     = 0x01;
    constexpr ModuleId Button     = 0x02;
    constexpr ModuleId PWM        = 0x03;
    constexpr ModuleId EEPROM     = 0x04;
    constexpr ModuleId CAN        = 0x05;
    constexpr ModuleId Logic      = 0x06;
    constexpr ModuleId Sequencer  = 0x07;
    constexpr ModuleId Diagnostic = 0x08;

    constexpr ModuleId Broadcast  = 0xFF;
}