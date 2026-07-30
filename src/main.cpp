#include <Arduino.h>

#include "config/device_config.hpp"
#include "config/device_modules.hpp"
#include "core/diagnostics/diagnostics.hpp"
#include "core/system/system.hpp"

namespace
{
constexpr unsigned long SerialBaudRate = 9600UL;
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
