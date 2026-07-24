#pragma once

#include <stdint.h>

namespace dan::platform
{
    class Interrupt final
    {
    public:
        using State = uint8_t;

        static void Enable();
        static void Disable();

        static State SaveAndDisable();
        static void Restore(State state);

        Interrupt() = delete;
    };
}