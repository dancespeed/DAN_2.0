#include <Arduino.h>

#include "core/diagnostics/diagnostics.hpp"
#include "core/message/message.hpp"
#include "core/message/message_bus.hpp"
#include "core/module/module_ids.hpp"
#include "core/system/system.hpp"

namespace
{
constexpr unsigned long SerialBaudRate = 9600UL;

constexpr dan::core::GlobalId Stage4GlobalId = 0;
constexpr dan::core::MessageId Stage4MessageId = 3;
constexpr dan::core::ObjectId Stage4ObjectId = 3;
constexpr dan::core::MessageValue Stage4Value = 0x9ABC;
}

void setup()
{
    Serial.begin(SerialBaudRate);
    delay(500);

    dan::core::Diagnostics::Initialize(Serial);
    dan::core::Diagnostics::Enable();
    dan::core::Diagnostics::PrintLine("[DIAG] Stage 4 start");

    dan::core::System::Initialize();
    dan::core::System::Start();

    dan::core::Diagnostics::PrintLine("[BOOT] System started");

    const dan::core::Message message
    {
        dan::core::MessageHeader::Create(
            Stage4GlobalId,
            Stage4GlobalId,
            dan::core::ModuleIds::Test,
            dan::core::ModuleIds::Test,
            dan::core::MessageType::Command,
            Stage4MessageId),
        Stage4ObjectId,
        Stage4Value
    };

    dan::core::MessageBus::Publish(message);
}

void loop()
{
    dan::core::System::Run();
}
