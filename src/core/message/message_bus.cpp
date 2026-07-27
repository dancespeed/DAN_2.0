#include "message_bus.hpp"

#include "message_bus_config.hpp"
#include "message_queue.hpp"

namespace
{
    dan::core::MessageQueue<dan::core::MessageBusConfig::QueueSize> queue;
}

namespace dan::core
{
    void MessageBus::Initialize()
    {
        queue.Clear();
    }

    bool MessageBus::Publish(const Message& message)
    {
        return queue.Push(message);
    }

    bool MessageBus::Receive(Message& message)
    {
        return queue.Pop(message);
    }

    bool MessageBus::HasMessages()
    {
        return !queue.IsEmpty();
    }

    uint8_t MessageBus::Count()
    {
        return queue.Count();
    }

    void MessageBus::Clear()
    {
        queue.Clear();
    }
}
