#include "message_bus.hpp"

#include "message_bus_config.hpp"
#include "message_queue.hpp"

#include "core/diagnostics/diagnostics.hpp"

namespace
{
    dan::core::MessageQueue<dan::core::MessageBusConfig::QueueSize> queue;
}

namespace dan::core
{
    void MessageBus::Initialize()
    {
        queue.Clear();
        Diagnostics::PrintLine("[BUS] Initialized");
    }

    bool MessageBus::Publish(const Message& message)
    {
        const bool published = queue.Push(message);

        Diagnostics::PrintLine(
            published
                ? "[BUS] Publish OK"
                : "[BUS] Publish FAILED");

        return published;
    }

    bool MessageBus::Receive(Message& message)
    {
        const bool received = queue.Pop(message);

        if (received)
        {
            Diagnostics::PrintLine("[BUS] Receive OK");
        }

        return received;
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
        Diagnostics::PrintLine("[BUS] Cleared");
    }
}
