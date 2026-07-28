#include "runtime.hpp"

#include "core/config/runtime_config.hpp"
#include "core/message/message_bus.hpp"
#include "core/message/message_dispatcher.hpp"
#include "core/module/module_table.hpp"
#include "modules/test/test_module.hpp"

namespace
{
    bool initialized = false;
    bool running = false;

    dan::modules::TestModule testModule;

    void RegisterModules()
    {
        dan::core::ModuleTable::Register(testModule);
    }
}

namespace dan::core
{
    void Runtime::Initialize()
    {
        if (initialized)
        {
            return;
        }

        ModuleTable::Initialize();
        MessageBus::Initialize();

        if (!MessageDispatcher::Initialize(config::LocalDeviceId))
        {
            return;
        }

        RegisterModules();
        ModuleTable::InitializeModules();

        initialized = true;
    }

    void Runtime::Start()
    {
        if (!initialized || running)
        {
            return;
        }

        if (!MessageDispatcher::Start())
        {
            return;
        }

        ModuleTable::Start();

        running = true;
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
