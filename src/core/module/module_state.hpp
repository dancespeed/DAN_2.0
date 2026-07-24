#pragma once

#include <stdint.h>

namespace dan::core
{
    enum class ModuleState : uint8_t
    {
        Stopped = 0,
        Initialized,
        Running
    };
}