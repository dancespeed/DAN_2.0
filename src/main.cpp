#include <Arduino.h>

#include "config/device_config.hpp"
#include "config/device_modules.hpp"
#include "core/diagnostics/diagnostics.hpp"
#include "core/system/system.hpp"

namespace
{
constexpr unsigned long SerialBaudRate = 9600UL;
constexpr uint16_t EepromTestAddress = 1023;

bool RunEepromHardwareTest()
{
    uint8_t original = 0;
    if (!dan::config::Eeprom.Read(
            EepromTestAddress,
            &original,
            sizeof(original)))
    {
        return false;
    }

    const uint8_t testValue =
        static_cast<uint8_t>(original ^ 0xFFU);
    uint8_t readValue = 0;

    const bool writeSucceeded =
        dan::config::Eeprom.Write(
            EepromTestAddress,
            &testValue,
            sizeof(testValue));
    const bool testReadSucceeded =
        dan::config::Eeprom.Read(
            EepromTestAddress,
            &readValue,
            sizeof(readValue));
    const bool testMatched =
        testReadSucceeded && readValue == testValue;

    const bool restoreWriteSucceeded =
        dan::config::Eeprom.Write(
            EepromTestAddress,
            &original,
            sizeof(original));

    readValue = 0;
    const bool restoreReadSucceeded =
        dan::config::Eeprom.Read(
            EepromTestAddress,
            &readValue,
            sizeof(readValue));
    const bool restoreMatched =
        restoreReadSucceeded && readValue == original;

    return writeSucceeded &&
        testMatched &&
        restoreWriteSucceeded &&
        restoreMatched;
}
}

void setup()
{
    Serial.begin(SerialBaudRate);
    delay(500);

    dan::core::Diagnostics::Initialize(Serial);
    dan::core::Diagnostics::Enable();
    dan::core::Diagnostics::PrintLine(F("[DIAG] Routing stage start"));

    if (dan::core::System::Initialize(
            dan::config::GlobalId,
            dan::config::DeviceModules,
            dan::config::DeviceModuleCount) &&
        dan::core::System::Start())
    {
        dan::core::Diagnostics::PrintLine(F("[BOOT] System started"));

        if (RunEepromHardwareTest())
        {
            dan::core::Diagnostics::PrintLine(
                F("[EEPROM TEST] Passed"));
        }
        else
        {
            dan::core::Diagnostics::PrintLine(
                F("[EEPROM TEST] Failed"));
        }
    }
    else
    {
        dan::core::Diagnostics::PrintLine(F("[BOOT] System failed"));
    }
}

void loop()
{
    dan::core::System::Run();
}
