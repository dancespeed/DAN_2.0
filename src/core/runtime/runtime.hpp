#pragma once

#include <stddef.h>

#include "core/message/message.hpp"

namespace dan::core
{
    class Module;

    class Runtime final
    {
    public:
        static bool Initialize(
            GlobalId globalId,
            Module* const* modules = nullptr,
            size_t moduleCount = 0
        );
        static bool Start();
        static void Run();
        static void Stop();

        Runtime() = delete;
    };
}
