#include <assert.h>
#include <stddef.h>

#include <EEPROM.h>

#include <string>

#include "../src/config/device_config.hpp"
#include "../src/config/device_modules.hpp"
#include "../src/core/diagnostics/diagnostics.hpp"
#include "../src/core/module/module_ids.hpp"
#include "../src/core/module/module_state.hpp"
#include "../src/core/module/module_table.hpp"
#include "../src/core/system/system.hpp"
#include "../src/platform/platform.hpp"

EEPROMClass EEPROM;

namespace
{
    class CaptureStream final : public Stream
    {
    public:
        size_t print(const char* text) override
        {
            if (text == nullptr)
            {
                return 0;
            }

            output_ += text;
            return std::char_traits<char>::length(text);
        }

        size_t println(const char* text) override
        {
            const size_t length = print(text);
            output_ += '\n';
            return length + 1;
        }

        const std::string& GetOutput() const
        {
            return output_;
        }

    private:
        std::string output_;
    };
}

namespace dan::platform
{
    void Platform::Initialize()
    {
    }

    void Platform::Start()
    {
    }

    void Platform::Run()
    {
    }

    void Platform::Stop()
    {
    }
}

int main()
{
    using namespace dan;

    modules::EepromModule unavailableEeprom;
    CaptureStream diagnostics;

    core::Diagnostics::Initialize(diagnostics);
    core::Diagnostics::Enable();

    EEPROM.SetLength(0);

    core::ModuleTable::Initialize();
    assert(core::ModuleTable::Register(unavailableEeprom));
    core::ModuleTable::InitializeModules();
    core::ModuleTable::Start();

    assert(
        unavailableEeprom.GetState() ==
        core::ModuleState::Stopped
    );
    assert(
        diagnostics.GetOutput().find(
            "Module 0x03 (EEPROM) ready\n"
        ) == std::string::npos
    );

    EEPROM.SetLength(1024);

    assert(config::DeviceModuleCount == 1);
    assert(config::DeviceModules[0] == &config::Eeprom);
    assert(config::Eeprom.GetId() == core::ModuleIds::Eeprom);
    assert(std::string(config::Eeprom.GetName()) == "EEPROM");

    core::Module* const modulesWithMissingEntry[]
    {
        nullptr,
        config::DeviceModules[0]
    };

    assert(core::System::Initialize(
        config::GlobalId,
        modulesWithMissingEntry,
        sizeof(modulesWithMissingEntry) /
            sizeof(modulesWithMissingEntry[0])
    ));
    assert(core::System::Start());

    assert(
        core::ModuleTable::Find(core::ModuleIds::Eeprom) ==
        &config::Eeprom
    );
    assert(
        config::Eeprom.GetState() ==
        core::ModuleState::Running
    );
    assert(
        diagnostics.GetOutput().find(
            "Module 0x03 (EEPROM) ready\n"
        ) != std::string::npos
    );

    core::System::Run();
    core::System::Stop();
    return 0;
}
