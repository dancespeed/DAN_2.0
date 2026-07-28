#pragma once

#include <stddef.h>

#include "core/message/message.hpp"

namespace dan::core::config
{
    inline constexpr size_t MaxModules = 32;
    inline constexpr GlobalId LocalDeviceId = 1;

    static_assert(
        MessageProtocol::IsDeviceId(LocalDeviceId),
        "LocalDeviceId must be in the range 1..30"
    );
}
