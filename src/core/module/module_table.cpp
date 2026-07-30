#include "module_table.hpp"

#include "core/config/runtime_config.hpp"
#include "core/diagnostics/diagnostics.hpp"

#include <stddef.h>

namespace
{
    constexpr char HexDigits[] = "0123456789ABCDEF";

    struct ModuleEntry
    {
        dan::core::Module* module = nullptr;
        bool runReported = false;
    };

    static_assert(
        dan::core::config::MaxModules <=
            static_cast<size_t>(dan::core::MessageProtocol::Broadcast - 1),
        "MaxModules exceeds the usable six-bit ModuleId range"
    );

    ModuleEntry modules[dan::core::config::MaxModules];

    size_t moduleCount = 0;

    void PrintModuleReady(const dan::core::Module& module)
    {
        const dan::core::ModuleId id = module.GetId();
        const char idText[]
        {
            HexDigits[(id >> 4U) & 0x0FU],
            HexDigits[id & 0x0FU],
            '\0'
        };

        dan::core::Diagnostics::Print(F("Module 0x"));
        dan::core::Diagnostics::Print(idText);

        const char* const name = module.GetName();

        if (name != nullptr && name[0] != '\0')
        {
            dan::core::Diagnostics::Print(F(" ("));
            dan::core::Diagnostics::Print(name);
            dan::core::Diagnostics::Print(F(")"));
        }

        dan::core::Diagnostics::PrintLine(F(" ready"));
    }
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

        if (!MessageHeader::IsValidModuleId(moduleId) ||
            moduleId == MessageProtocol::InvalidModule ||
            moduleId == MessageProtocol::Broadcast)
        {
            return false;
        }

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
                if (module->Start())
                {
                    PrintModuleReady(*module);
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
                    Diagnostics::Print(F("[MODULE] Run: "));
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
                    Diagnostics::Print(F("[MODULE] Stop OK: "));
                    Diagnostics::PrintLine(module->GetName());
                }
                else
                {
                    Diagnostics::Print(F("[MODULE] Stop FAILED: "));
                    Diagnostics::PrintLine(module->GetName());
                }
            }
        }
    }
}
