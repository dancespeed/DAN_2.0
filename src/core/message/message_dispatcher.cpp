#include "message_dispatcher.hpp"

#include "message_bus.hpp"
#include "message.hpp"

namespace MessageDispatcher
{
    void Process()
    {
        Message message;

        if (!MessageBus::Receive(message))
        {
            return;
        }

        //
        // Здесь постепенно появится новая цепочка:
        //
        // MessageValidator
        // ObjectRegistry
        // ObjectValidator
        // Logic
        // Services
        //
    }
}