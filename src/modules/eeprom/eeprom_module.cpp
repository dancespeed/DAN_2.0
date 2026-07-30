#include "eeprom_module.hpp"

#include "platform/eeprom/eeprom.hpp"

namespace dan::modules
{
    core::ModuleId EepromModule::GetId() const
    {
        return Id;
    }

    const char* EepromModule::GetName() const
    {
        return "EEPROM";
    }

    bool EepromModule::Read(
        uint16_t address,
        uint8_t* buffer,
        uint16_t size)
    {
        return platform::Eeprom::Read(address, buffer, size);
    }

    bool EepromModule::Write(
        uint16_t address,
        const uint8_t* data,
        uint16_t size)
    {
        return platform::Eeprom::Write(address, data, size);
    }

    bool EepromModule::OnInitialize()
    {
        return platform::Eeprom::Initialize();
    }

    bool EepromModule::OnStart()
    {
        return true;
    }

    void EepromModule::OnRun()
    {
    }

    void EepromModule::OnStop()
    {
    }

    void EepromModule::OnReceive(const core::Message&)
    {
    }
}
