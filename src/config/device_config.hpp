#pragma once

#include <stdint.h>

#include "core/message/message.hpp"

namespace dan::config
{
    inline constexpr uint8_t Version = 1;
    inline constexpr core::GlobalId GlobalId =
        core::MessageProtocol::FirstDevice;

    static_assert(
        core::MessageProtocol::IsDeviceId(GlobalId),
        "GlobalId must be in the range 1..30"
    );
}
