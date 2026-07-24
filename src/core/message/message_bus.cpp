#include "message_bus.hpp"

#include "message_bus_config.hpp"
#include "message_queue.hpp"

namespace
{
    MessageQueue<MessageBusConfig::QueueSize> queue;
}

namespace MessageBus
{
    void Init()
    {
        queue.Clear();
    }

    bool Publish(const Message& message)
    {
        return queue.Push(message);
    }

    bool Receive(Message& message)
    {
        return queue.Pop(message);
    }

    bool HasMessages()
    {
        return !queue.IsEmpty();
    }

    uint8_t Count()
    {
        return queue.Count();
    }

    void Clear()
    {
        queue.Clear();
    }
}