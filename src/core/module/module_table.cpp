#include "module_table.hpp"

#include "core/config/runtime_config.hpp"
#include "core/diagnostics/diagnostics.hpp"

#include <stddef.h>

namespace
{
    struct ModuleEntry
    {
        dan::core::Module* module = nullptr;
        bool runReported = false;
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
            entry.runReported = false;
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
        modules[moduleCount].runReported = false;
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

                if (module->GetState() == ModuleState::Initialized)
                {
                    Diagnostics::Print("[MODULE] Initialize OK: ");
                    Diagnostics::PrintLine(module->GetName());
                }
                else
                {
                    Diagnostics::Print("[MODULE] Initialize FAILED: ");
                    Diagnostics::PrintLine(module->GetName());
                }
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

                if (module->GetState() == ModuleState::Running)
                {
                    Diagnostics::Print("[MODULE] Start OK: ");
                    Diagnostics::PrintLine(module->GetName());
                }
                else
                {
                    Diagnostics::Print("[MODULE] Start FAILED: ");
                    Diagnostics::PrintLine(module->GetName());
                }
            }
        }
    }

    void ModuleTable::Run()
    {
        for (size_t index = 0; index < moduleCount; ++index)
        {
            ModuleEntry& entry = modules[index];
            Module* module = entry.module;

            if (module != nullptr)
            {
                module->Run();

                if (!entry.runReported &&
                    module->GetState() == ModuleState::Running)
                {
                    Diagnostics::Print("[MODULE] Run: ");
                    Diagnostics::PrintLine(module->GetName());
                    entry.runReported = true;
                }
            }
        }
    }

    void ModuleTable::Stop()
    {
        for (size_t index = moduleCount; index > 0; --index)
        {
            ModuleEntry& entry = modules[index - 1];
            Module* module = entry.module;

            if (module != nullptr)
            {
                module->Stop();
                entry.runReported = false;

                if (module->GetState() == ModuleState::Stopped)
                {
                    Diagnostics::Print("[MODULE] Stop OK: ");
                    Diagnostics::PrintLine(module->GetName());
                }
                else
                {
                    Diagnostics::Print("[MODULE] Stop FAILED: ");
                    Diagnostics::PrintLine(module->GetName());
                }
            }
        }
    }
}
