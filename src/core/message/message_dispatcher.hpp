#pragma once

#include <stdint.h>

namespace dan::core
{
    class MessageDispatcher final
    {
    public:
        static bool ProcessOne();
        static uint8_t ProcessAll();

    private:
        MessageDispatcher() = delete;
    };
}
