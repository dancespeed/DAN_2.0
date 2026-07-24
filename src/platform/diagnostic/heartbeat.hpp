#pragma once

#include <stdint.h>

namespace dan::platform
{
    class Heartbeat final
    {
    public:
        static void Initialize();
        static void Update(uint32_t currentTickMs);
        static void Stop();

        Heartbeat() = delete;
    };
}