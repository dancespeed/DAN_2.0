#pragma once

#include <stdint.h>

namespace dan::core
{
    enum class ModuleType : uint8_t
    {
        System = 0,
        Driver,
        Service,
        Application
    };
}