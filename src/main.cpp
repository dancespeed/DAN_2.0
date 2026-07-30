#include <Arduino.h>

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

    dan::core::System::Initialize();
    dan::core::System::Start();

    dan::core::Diagnostics::PrintLine(F("[BOOT] System started"));
}

void loop()
{
    dan::core::System::Run();
}