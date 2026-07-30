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

    EEPROM.Reset();

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

    const uint8_t written[] {0x10, 0x20, 0x30, 0x40};
    uint8_t read[sizeof(written)] {};

    assert(config::Eeprom.Write(10, written, sizeof(written)));
    assert(EEPROM.GetUpdateCount() == sizeof(written));
    assert(config::Eeprom.Read(10, read, sizeof(read)));

    for (size_t index = 0; index < sizeof(written); ++index)
    {
        assert(read[index] == written[index]);
    }

    assert(config::Eeprom.Write(10, written, sizeof(written)));
    assert(EEPROM.GetUpdateCount() == sizeof(written));

    assert(!config::Eeprom.Read(0, nullptr, 1));
    assert(!config::Eeprom.Write(0, nullptr, 1));
    assert(!config::Eeprom.Read(0, read, 0));
    assert(!config::Eeprom.Write(0, written, 0));
    assert(!config::Eeprom.Read(1023, read, 2));
    assert(!config::Eeprom.Write(1023, written, 2));
    assert(!config::Eeprom.Read(65535, read, 2));
    assert(!config::Eeprom.Write(65535, written, 2));

    uint8_t lastByte = 0x5A;
    assert(config::Eeprom.Write(1023, &lastByte, 1));
    lastByte = 0;
    assert(config::Eeprom.Read(1023, &lastByte, 1));
    assert(lastByte == 0x5A);

    core::System::Run();
    core::System::Stop();
    return 0;
}
