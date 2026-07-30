#pragma once

#include <stddef.h>

#include "core/module/module.hpp"
#include "modules/eeprom/eeprom_module.hpp"

namespace dan::config
{
    inline modules::EepromModule Eeprom;

    inline core::Module* const DeviceModules[]
    {
        &Eeprom
    };

    inline constexpr size_t DeviceModuleCount =
        sizeof(DeviceModules) / sizeof(DeviceModules[0]);
}
