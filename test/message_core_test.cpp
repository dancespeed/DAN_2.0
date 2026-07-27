#include <assert.h>

#include "../src/core/message/message.hpp"
#include "../src/core/message/message_bus.hpp"
#include "../src/core/message/message_bus_config.hpp"

using namespace dan::core;

int main()
{
    constexpr ModuleId sender = 0x11;
    constexpr ModuleId receiver = 0x22;
    constexpr MessageType type = 0x02;
    constexpr MessageAttribute attributes = 0x01;
    constexpr MessageId messageId = 0x0ABC;

    const Message message
    {
        MessageHeader::Create(sender, receiver, type, attributes, messageId),
        0x1234,
        0x5678
    };

    static_assert(sizeof(Message) == 8, "Message size changed");

    assert(MessageHeader::GetSender(message.header) == sender);
    assert(MessageHeader::GetReceiver(message.header) == receiver);
    assert(MessageHeader::GetType(message.header) == type);
    assert(MessageHeader::GetAttributes(message.header) == attributes);
    assert(MessageHeader::GetMessageId(message.header) == messageId);

    MessageBus::Initialize();
    assert(!MessageBus::HasMessages());

    for (uint8_t index = 0; index < MessageBusConfig::QueueSize; ++index)
    {
        assert(MessageBus::Publish(message));
    }

    assert(!MessageBus::Publish(message));
    assert(MessageBus::Count() == MessageBusConfig::QueueSize);

    Message received{};
    for (uint8_t index = 0; index < MessageBusConfig::QueueSize; ++index)
    {
        assert(MessageBus::Receive(received));
        assert(received.header == message.header);
        assert(received.objectId == message.objectId);
        assert(received.value == message.value);
    }

    assert(!MessageBus::Receive(received));
    assert(!MessageBus::HasMessages());
    return 0;
}
