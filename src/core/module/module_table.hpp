#pragma once

#include "core/message/message.hpp"
#include "core/module/module.hpp"

namespace dan::core
{
    class ModuleTable final
    {
    public:
        static void Initialize();

        static bool Register(Module& module);

        [[nodiscard]]
        static Module* Find(ModuleId id);

        static void ReceiveBroadcast(const Message& message);

        static void InitializeModules();
        static void Start();
        static void Run();
        static void Stop();

    private:
        ModuleTable() = delete;
    };
}
