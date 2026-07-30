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
    bool Runtime::Initialize(
        GlobalId globalId,
        Module* const* modules,
        size_t moduleCount)
    {
        if (initialized)
        {
            return true;
        }

        if (modules == nullptr && moduleCount != 0)
        {
            return false;
        }

        ModuleTable::Initialize();

        for (size_t index = 0; index < moduleCount; ++index)
        {
            Module* const module = modules[index];

            if (module != nullptr && !ModuleTable::Register(*module))
            {
                return false;
            }
        }

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
