#include "runtime.hpp"

#include "core/message/message_bus.hpp"
#include "core/message/message_dispatcher.hpp"
#include "core/module/module_table.hpp"

namespace
{
    bool initialized = false;
    bool running = false;
}

namespace dan::core
{
    bool Runtime::Initialize(GlobalId globalId)
    {
        if (initialized)
        {
            return true;
        }

        ModuleTable::Initialize();
        MessageBus::Initialize();

        if (!MessageDispatcher::Initialize(globalId))
        {
            return false;
        }

        ModuleTable::InitializeModules();

        initialized = true;
        return true;
    }

    bool Runtime::Start()
    {
        if (running)
        {
            return true;
        }

        if (!initialized)
        {
            return false;
        }

        if (!MessageDispatcher::Start())
        {
            return false;
        }

        ModuleTable::Start();

        running = true;
        return true;
    }

    void Runtime::Run()
    {
        if (!running)
        {
            return;
        }

        MessageDispatcher::ProcessAll();
        ModuleTable::Run();
    }

    void Runtime::Stop()
    {
        if (!running)
        {
            return;
        }

        ModuleTable::Stop();
        MessageDispatcher::Stop();

        running = false;
    }
}
