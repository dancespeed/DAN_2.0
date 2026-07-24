#include <Arduino.h>

#include "core/system/system.hpp"

void setup()
{
    dan::core::System::Initialize();
    dan::core::System::Start();
}

void loop()
{
    dan::core::System::Run();
}