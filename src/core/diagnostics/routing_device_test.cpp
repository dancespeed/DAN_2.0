#include "routing_device_test.hpp"

#include "core/config/runtime_config.hpp"
#include "core/diagnostics/diagnostics.hpp"
#include "core/message/message.hpp"
#include "core/message/message_dispatcher.hpp"
#include "core/system/system.hpp"
#include "modules/test/test_module.hpp"

namespace
{
using namespace dan::core;

constexpr GlobalId RemoteDeviceId =
    config::LocalDeviceId == MessageProtocol::FirstDevice
        ? static_cast<GlobalId>(MessageProtocol::FirstDevice + 1U)
        : MessageProtocol::FirstDevice;

constexpr MessageId RemoteMessageId = 10;
constexpr MessageId BroadcastMessageId = 11;
constexpr MessageId InvalidSystemMessageId = 12;
constexpr MessageId IncomingBroadcastMessageId = 13;

constexpr ObjectId TestObjectId = 0x0100;
constexpr MessageValue TestValue = 0xA55A;

static_assert(
    MessageProtocol::IsDeviceId(RemoteDeviceId),
    "Routing test remote device ID must be valid"
);

static_assert(
    RemoteDeviceId != config::LocalDeviceId,
    "Routing test remote device ID must differ from LocalDeviceId"
);

Message CreateMessage(
    GlobalId globalSender,
    GlobalId globalReceiver,
    MessageType type,
    MessageId messageId)
{
    return Message
    {
        MessageHeader::Create(
            globalSender,
            globalReceiver,
            dan::modules::TestModule::Id,
            dan::modules::TestModule::Id,
            type,
            messageId),
        TestObjectId,
        TestValue
    };
}

void PrintScenarioResult(uint8_t scenario, bool passed)
{
    switch (scenario)
    {
        case 1:
            Diagnostics::PrintLine(
                passed
                    ? F("[TEST] Scenario 1: PASS")
                    : F("[TEST] Scenario 1: FAIL"));
            break;

        case 2:
            Diagnostics::PrintLine(
                passed
                    ? F("[TEST] Scenario 2: PASS")
                    : F("[TEST] Scenario 2: FAIL"));
            break;

        case 3:
            Diagnostics::PrintLine(
                passed
                    ? F("[TEST] Scenario 3: PASS")
                    : F("[TEST] Scenario 3: FAIL"));
            break;

        case 4:
            Diagnostics::PrintLine(
                passed
                    ? F("[TEST] Scenario 4: PASS")
                    : F("[TEST] Scenario 4: FAIL"));
            break;

        default:
            break;
    }
}

bool RunRemoteWithoutTransportScenario()
{
    Diagnostics::PrintLine(
        F("[TEST] Scenario 1: Remote -> NoTransport"));

    const Message message = CreateMessage(
        MessageProtocol::InvalidGlobal,
        RemoteDeviceId,
        MessageType::Command,
        RemoteMessageId
    );

    const DispatchResult result =
        MessageDispatcher::SubmitLocal(message);

    return
        !result.localAccepted &&
        result.networkResult == NetworkDispatchResult::NoTransport;
}

bool RunLocalBroadcastScenario()
{
    Diagnostics::PrintLine(
        F("[TEST] Scenario 2: GlobalBroadcast -> Local + NoTransport"));

    const Message message = CreateMessage(
        MessageProtocol::InvalidGlobal,
        MessageProtocol::GlobalBroadcast,
        MessageType::Event,
        BroadcastMessageId
    );

    const DispatchResult result =
        MessageDispatcher::SubmitLocal(message);

    const bool passed =
        result.localAccepted &&
        result.networkResult == NetworkDispatchResult::NoTransport;

    System::Run();
    return passed;
}

bool RunInvalidRemoteSystemScenario()
{
    Diagnostics::PrintLine(
        F("[TEST] Scenario 3: Remote System -> Rejected"));

    const Message message = CreateMessage(
        MessageProtocol::InvalidGlobal,
        RemoteDeviceId,
        MessageType::System,
        InvalidSystemMessageId
    );

    const DispatchResult result =
        MessageDispatcher::SubmitLocal(message);

    return
        !result.localAccepted &&
        result.networkResult == NetworkDispatchResult::Rejected;
}

bool RunIncomingBroadcastScenario()
{
    Diagnostics::PrintLine(
        F("[TEST] Scenario 4: SubmitNetwork Broadcast -> Local, no echo"));

    const Message message = CreateMessage(
        RemoteDeviceId,
        MessageProtocol::GlobalBroadcast,
        MessageType::Event,
        IncomingBroadcastMessageId
    );

    const bool accepted =
        MessageDispatcher::SubmitNetwork(message);

    System::Run();
    return accepted;
}
}

namespace dan::diagnostics
{

void RoutingDeviceTest::Run()
{
    core::Diagnostics::PrintLine(
        F("[TEST] Routing device test: start"));

    core::Diagnostics::PrintLine(
        F("[TEST] Drain startup message"));
    core::System::Run();

    const bool scenario1 = RunRemoteWithoutTransportScenario();
    PrintScenarioResult(1, scenario1);

    const bool scenario2 = RunLocalBroadcastScenario();
    PrintScenarioResult(2, scenario2);

    const bool scenario3 = RunInvalidRemoteSystemScenario();
    PrintScenarioResult(3, scenario3);

    const bool scenario4 = RunIncomingBroadcastScenario();
    PrintScenarioResult(4, scenario4);

    const bool allPassed =
        scenario1 && scenario2 && scenario3 && scenario4;

    core::Diagnostics::PrintLine(
        allPassed
            ? F("[TEST] Routing device test: 4/4 PASS")
            : F("[TEST] Routing device test: FAILED"));
}

} // namespace dan::diagnostics
