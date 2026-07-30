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
