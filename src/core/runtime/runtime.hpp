#pragma once

#include "core/message/message.hpp"

namespace dan::core
{
    class Runtime final
    {
    public:
        static bool Initialize(GlobalId globalId);
        static bool Start();
        static void Run();
        static void Stop();

        Runtime() = delete;
    };
}
