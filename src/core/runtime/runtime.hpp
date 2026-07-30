#pragma once

#include "core/message/message.hpp"

namespace dan::core
{
    class Runtime final
    {
    public:
        static void Initialize(GlobalId globalId);
        static void Start();
        static void Run();
        static void Stop();

        Runtime() = delete;
    };
}
