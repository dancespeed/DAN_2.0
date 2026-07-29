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

<<<<<<< HEAD
    dan::core::Diagnostics::PrintLine(F("[BOOT] System started"));

    dan::diagnostics::RoutingDeviceTest::Run();
=======
    dan::core::Diagnostics::PrintLine("[BOOT] System started");
>>>>>>> a1b7758 (Run core without application modules)
}

void loop()
{
    dan::core::System::Run();
}
