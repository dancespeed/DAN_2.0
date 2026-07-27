#include "message_dispatcher.hpp"

#include "message.hpp"
#include "message_bus.hpp"

#include "core/module/module.hpp"
#include "core/module/module_table.hpp"

namespace dan::core
{
    bool MessageDispatcher::ProcessOne()
    {
        Message message{};

        if (!MessageBus::Receive(message))
        {
            return false;
        }

        const ModuleId receiver = MessageHeader::GetReceiver(message.header);

        if (receiver == MessageProtocol::Broadcast)
        {
            ModuleTable::ReceiveBroadcast(message);
            return true;
        }

        Module* module = ModuleTable::Find(receiver);

        if (module == nullptr)
        {
            return false;
        }

        module->Receive(message);
        return true;
    }

    uint8_t MessageDispatcher::ProcessAll()
    {
        uint8_t processedCount = 0;

        while (MessageBus::HasMessages())
        {
            ProcessOne();
            ++processedCount;
        }

        return processedCount;
    }
}
