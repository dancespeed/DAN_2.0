#include "module_table.hpp"

#include "core/config/runtime_config.hpp"

#include <stddef.h>

namespace
{
    struct ModuleEntry
    {
        dan::core::Module* module = nullptr;
    };

    ModuleEntry modules[dan::core::config::MaxModules];

    size_t moduleCount = 0;
}

namespace dan::core
{
    void ModuleTable::Initialize()
    {
        moduleCount = 0;

        for (ModuleEntry& entry : modules)
        {
            entry.module = nullptr;
        }
    }

    bool ModuleTable::Register(Module& module)
    {
        if (moduleCount >= config::MaxModules)
        {
            return false;
        }

        const ModuleId moduleId = module.GetId();

        if (Find(moduleId) != nullptr)
        {
            return false;
        }

        modules[moduleCount].module = &module;
        ++moduleCount;

        return true;
    }

    Module* ModuleTable::Find(ModuleId id)
    {
        for (size_t index = 0; index < moduleCount; ++index)
        {
            Module* module = modules[index].module;

            if (module != nullptr && module->GetId() == id)
            {
                return module;
            }
        }

        return nullptr;
    }

    void ModuleTable::ReceiveBroadcast(const Message& message)
    {
        for (size_t index = 0; index < moduleCount; ++index)
        {
            Module* module = modules[index].module;

            if (module != nullptr)
            {
                module->Receive(message);
            }
        }
    }

    void ModuleTable::InitializeModules()
    {
        for (size_t index = 0; index < moduleCount; ++index)
        {
            Module* module = modules[index].module;

            if (module != nullptr)
            {
                module->Initialize();
            }
        }
    }

    void ModuleTable::Start()
    {
        for (size_t index = 0; index < moduleCount; ++index)
        {
            Module* module = modules[index].module;

            if (module != nullptr)
            {
                module->Start();
            }
        }
    }

    void ModuleTable::Run()
    {
        for (size_t index = 0; index < moduleCount; ++index)
        {
            Module* module = modules[index].module;

            if (module != nullptr)
            {
                module->Run();
            }
        }
    }

    void ModuleTable::Stop()
    {
        for (size_t index = moduleCount; index > 0; --index)
        {
            Module* module = modules[index - 1].module;

            if (module != nullptr)
            {
                module->Stop();
            }
        }
    }
}
