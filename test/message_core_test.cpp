#include <assert.h>
#include <stdint.h>

#include "../src/core/message/message.hpp"
#include "../src/core/message/message_bus.hpp"
#include "../src/core/message/message_dispatcher.hpp"
#include "../src/core/message/message_queue.hpp"
#include "../src/core/module/module.hpp"
#include "../src/core/module/module_table.hpp"
#include "../src/core/network/network_transport.hpp"
#include "../src/core/network/network_transport_config.hpp"

using namespace dan::core;

namespace
{
    class TestTransport final : public INetworkTransport
    {
    public:
        void Start()
        {
            ready_ = true;
        }

        void Stop()
        {
            ready_ = false;
        }

        TransportSendResult Send(const Message& message) override
        {
            if (!MessageProtocol::IsNetworkVisible(
                    MessageHeader::GetType(message.header)))
            {
                return TransportSendResult::Rejected;
            }

            if (!ready_)
            {
                return TransportSendResult::NotReady;
            }

            if (!queue_.Push(message))
            {
                return TransportSendResult::QueueFull;
            }

            return TransportSendResult::Accepted;
        }

        bool Pop(Message& message)
        {
            return queue_.Pop(message);
        }

        uint8_t Count() const
        {
            return queue_.Count();
        }

    private:
        bool ready_ = false;
        MessageQueue<NetworkTransportConfig::TxQueueSize> queue_;
    };

    class TestModule final : public Module
    {
    public:
        explicit TestModule(ModuleId id)
            : id_(id)
        {
        }

        ModuleId GetId() const override
        {
            return id_;
        }

        const char* GetName() const override
        {
            return "RoutingTestModule";
        }

        bool HasMessage() const
        {
            return hasMessage_;
        }

        const Message& GetLastMessage() const
        {
            return lastMessage_;
        }

        void ClearMessage()
        {
            hasMessage_ = false;
            lastMessage_ = {};
        }

    protected:
        bool OnInitialize() override
        {
            ClearMessage();
            return true;
        }

        bool OnStart() override
        {
            return true;
        }

        void OnRun() override
        {
        }

        void OnStop() override
        {
        }

        void OnReceive(const Message& message) override
        {
            lastMessage_ = message;
            hasMessage_ = true;
        }

    private:
        ModuleId id_;
        bool hasMessage_ = false;
        Message lastMessage_{};
    };

    Message CreateMessage(
        GlobalId globalSender,
        GlobalId globalReceiver,
        ModuleId sender,
        ModuleId receiver,
        MessageType type,
        MessageId messageId)
    {
        return Message
        {
            MessageHeader::Create(
                globalSender,
                globalReceiver,
                sender,
                receiver,
                type,
                messageId),
            0x1234,
            0x5678
        };
    }
}

int main()
{
    constexpr GlobalId globalSender = 0x12;
    constexpr GlobalId globalReceiver = 0x0D;
    constexpr ModuleId sender = 0x15;
    constexpr ModuleId receiver = 0x2A;
    constexpr MessageType type = MessageType::System;
    constexpr MessageId messageId = 0xBC;

    constexpr uint32_t header = MessageHeader::Create(
        globalSender,
        globalReceiver,
        sender,
        receiver,
        type,
        messageId
    );

    const Message packedMessage
    {
        header,
        0x1234,
        0x5678
    };

    static_assert(sizeof(Message) == 8, "Message size changed");

    static_assert(
        MessageHeader::Create(
            0x1F,
            0x1F,
            0x3F,
            0x3F,
            MessageType::Reserved,
            0xFF)
            == UINT32_MAX,
        "Header must use all 32 bits"
    );

    static_assert(MessageProtocol::InvalidGlobal == 0x00);
    static_assert(MessageProtocol::FirstDevice == 0x01);
    static_assert(MessageProtocol::LastDevice == 0x1E);
    static_assert(MessageProtocol::GlobalBroadcast == 0x1F);

    static_assert(MessageProtocol::InvalidModule == 0x00);
    static_assert(MessageProtocol::FirstModule == 0x01);
    static_assert(MessageProtocol::LastModule == 0x3E);
    static_assert(MessageProtocol::Broadcast == 0x3F);

    static_assert(MessageProtocol::IsDeviceId(1));
    static_assert(MessageProtocol::IsDeviceId(30));
    static_assert(!MessageProtocol::IsDeviceId(0));
    static_assert(!MessageProtocol::IsDeviceId(31));
    static_assert(MessageProtocol::IsGlobalReceiver(31));

    static_assert(static_cast<uint8_t>(MessageType::Command) == 0x00);
    static_assert(static_cast<uint8_t>(MessageType::Event) == 0x01);
    static_assert(static_cast<uint8_t>(MessageType::System) == 0x02);
    static_assert(static_cast<uint8_t>(MessageType::Reserved) == 0x03);

    static_assert(MessageProtocol::IsDefinedType(MessageType::Command));
    static_assert(MessageProtocol::IsDefinedType(MessageType::Event));
    static_assert(MessageProtocol::IsDefinedType(MessageType::System));
    static_assert(!MessageProtocol::IsDefinedType(MessageType::Reserved));

    static_assert(MessageProtocol::IsNetworkVisible(MessageType::Command));
    static_assert(MessageProtocol::IsNetworkVisible(MessageType::Event));
    static_assert(!MessageProtocol::IsNetworkVisible(MessageType::System));
    static_assert(!MessageProtocol::IsNetworkVisible(MessageType::Reserved));

    static_assert(NetworkTransportConfig::TxQueueSize == 8);

    assert(MessageHeader::GetGlobalSender(packedMessage.header) == globalSender);
    assert(MessageHeader::GetGlobalReceiver(packedMessage.header) == globalReceiver);
    assert(MessageHeader::GetSender(packedMessage.header) == sender);
    assert(MessageHeader::GetReceiver(packedMessage.header) == receiver);
    assert(MessageHeader::GetType(packedMessage.header) == type);
    assert(MessageHeader::GetMessageId(packedMessage.header) == messageId);

    constexpr GlobalId localDeviceId = 1;
    constexpr GlobalId remoteDeviceId = 2;
    constexpr ModuleId sourceModuleId = 1;
    constexpr ModuleId targetModuleId = 2;

    TestModule sourceModule(sourceModuleId);
    TestModule targetModule(targetModuleId);
    TestTransport transport;

    ModuleTable::Initialize();
    MessageBus::Initialize();

    assert(ModuleTable::Register(sourceModule));
    assert(ModuleTable::Register(targetModule));
    ModuleTable::InitializeModules();

    assert(MessageDispatcher::Initialize(localDeviceId));
    assert(MessageDispatcher::SetNetworkTransport(&transport));
    transport.Start();
    assert(MessageDispatcher::Start());
    ModuleTable::Start();

    const Message localMessage = CreateMessage(
        remoteDeviceId,
        localDeviceId,
        sourceModuleId,
        targetModuleId,
        MessageType::Command,
        1
    );

    const DispatchResult localResult =
        MessageDispatcher::SubmitLocal(localMessage);

    assert(localResult.localAccepted);
    assert(localResult.networkResult == NetworkDispatchResult::NotRequested);
    assert(MessageDispatcher::ProcessOne());
    assert(targetModule.HasMessage());
    assert(
        MessageHeader::GetGlobalSender(
            targetModule.GetLastMessage().header)
        == localDeviceId
    );

    targetModule.ClearMessage();

    const Message remoteMessage = CreateMessage(
        MessageProtocol::InvalidGlobal,
        remoteDeviceId,
        sourceModuleId,
        targetModuleId,
        MessageType::Command,
        2
    );

    const DispatchResult remoteResult =
        MessageDispatcher::SubmitLocal(remoteMessage);

    assert(!remoteResult.localAccepted);
    assert(remoteResult.networkResult == NetworkDispatchResult::Accepted);
    assert(transport.Count() == 1);

    Message transmitted{};
    assert(transport.Pop(transmitted));
    assert(MessageHeader::GetGlobalSender(transmitted.header) == localDeviceId);
    assert(MessageHeader::GetGlobalReceiver(transmitted.header) == remoteDeviceId);

    const Message broadcastMessage = CreateMessage(
        MessageProtocol::InvalidGlobal,
        MessageProtocol::GlobalBroadcast,
        sourceModuleId,
        targetModuleId,
        MessageType::Event,
        3
    );

    const DispatchResult broadcastResult =
        MessageDispatcher::SubmitLocal(broadcastMessage);

    assert(broadcastResult.localAccepted);
    assert(broadcastResult.networkResult == NetworkDispatchResult::Accepted);
    assert(transport.Count() == 1);
    assert(MessageDispatcher::ProcessOne());
    assert(targetModule.HasMessage());

    targetModule.ClearMessage();

    const Message localSystemMessage = CreateMessage(
        MessageProtocol::InvalidGlobal,
        localDeviceId,
        sourceModuleId,
        targetModuleId,
        MessageType::System,
        4
    );

    const DispatchResult localSystemResult =
        MessageDispatcher::SubmitLocal(localSystemMessage);

    assert(localSystemResult.localAccepted);
    assert(
        localSystemResult.networkResult ==
        NetworkDispatchResult::NotRequested
    );
    assert(MessageDispatcher::ProcessOne());
    assert(targetModule.HasMessage());

    const Message invalidSystemMessage = CreateMessage(
        MessageProtocol::InvalidGlobal,
        remoteDeviceId,
        sourceModuleId,
        targetModuleId,
        MessageType::System,
        5
    );

    const DispatchResult invalidSystemResult =
        MessageDispatcher::SubmitLocal(invalidSystemMessage);

    assert(!invalidSystemResult.localAccepted);
    assert(
        invalidSystemResult.networkResult ==
        NetworkDispatchResult::Rejected
    );

    targetModule.ClearMessage();

    const Message incomingMessage = CreateMessage(
        remoteDeviceId,
        localDeviceId,
        sourceModuleId,
        targetModuleId,
        MessageType::Event,
        6
    );

    assert(MessageDispatcher::SubmitNetwork(incomingMessage));
    assert(MessageDispatcher::ProcessOne());
    assert(targetModule.HasMessage());

    targetModule.ClearMessage();
    const uint8_t queuedBeforeIncomingBroadcast = transport.Count();

    const Message incomingBroadcast = CreateMessage(
        remoteDeviceId,
        MessageProtocol::GlobalBroadcast,
        sourceModuleId,
        targetModuleId,
        MessageType::Event,
        7
    );

    assert(MessageDispatcher::SubmitNetwork(incomingBroadcast));
    assert(MessageDispatcher::ProcessOne());
    assert(targetModule.HasMessage());
    assert(transport.Count() == queuedBeforeIncomingBroadcast);

    const Message incomingSystem = CreateMessage(
        remoteDeviceId,
        localDeviceId,
        sourceModuleId,
        targetModuleId,
        MessageType::System,
        8
    );

    assert(!MessageDispatcher::SubmitNetwork(incomingSystem));

    while (transport.Pop(transmitted))
    {
    }

    for (uint8_t index = 0;
         index < NetworkTransportConfig::TxQueueSize;
         ++index)
    {
        const Message queuedMessage = CreateMessage(
            MessageProtocol::InvalidGlobal,
            remoteDeviceId,
            sourceModuleId,
            targetModuleId,
            MessageType::Command,
            index
        );

        const DispatchResult queuedResult =
            MessageDispatcher::SubmitLocal(queuedMessage);

        assert(
            queuedResult.networkResult ==
            NetworkDispatchResult::Accepted
        );
    }

    const DispatchResult fullResult =
        MessageDispatcher::SubmitLocal(remoteMessage);

    assert(fullResult.networkResult == NetworkDispatchResult::QueueFull);

    MessageDispatcher::Stop();
    ModuleTable::Stop();
    transport.Stop();

    assert(MessageDispatcher::SetNetworkTransport(nullptr));
    assert(MessageDispatcher::Start());

    const DispatchResult noTransportResult =
        MessageDispatcher::SubmitLocal(remoteMessage);

    assert(
        noTransportResult.networkResult ==
        NetworkDispatchResult::NoTransport
    );

    MessageDispatcher::Stop();
    return 0;
}
