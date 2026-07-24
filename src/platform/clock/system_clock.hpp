#pragma once

#include <stdint.h>

namespace dan::platform
{
    class SystemClock final
    {
    public:
        static void Initialize();
        static void Start();
        static void Stop();

        static uint32_t GetTickMs();

        SystemClock() = delete;
    };
}