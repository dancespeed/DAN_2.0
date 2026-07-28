#include "test_module.hpp"

#include "core/config/runtime_config.hpp"
#include "core/message/message_dispatcher.hpp"
#include "core/module/module_ids.hpp"

namespace
{
    constexpr dan::core::MessageId TestMessageId = 1;
    constexpr dan::core::ObjectId TestObjectId = 1;
    constexpr dan::core::MessageValue TestValue = 0x1234;
}

namespace dan::modules
{
    core::ModuleId TestModule::GetId() const
    {
        return core::ModuleIds::Test;
    }

    const char* TestModule::GetName() const
    {
        return "TestModule";
    }

    bool TestModule::HasReceivedMessage() const
    {
        return hasReceivedMessage;
    }

    const core::Message& TestModule::GetLastMessage() const
    {
        return lastMessage;
    }

    bool TestModule::OnInitialize()
    {
        hasReceivedMessage = false;
        lastMessage = {};
        return true;
    }

    bool TestModule::OnStart()
    {
        const core::Message message
        {
            core::MessageHeader::Create(
                core::MessageProtocol::InvalidGlobal,
                core::config::LocalDeviceId,
                GetId(),
                GetId(),
                core::MessageType::Event,
                TestMessageId),
            TestObjectId,
            TestValue
        };

        const core::DispatchResult result =
            core::MessageDispatcher::SubmitLocal(message);

        return result.localAccepted;
    }

    void TestModule::OnRun()
    {
    }

    void TestModule::OnStop()
    {
    }

    void TestModule::OnReceive(const core::Message& message)
    {
        lastMessage = message;
        hasReceivedMessage = true;
    }
}
