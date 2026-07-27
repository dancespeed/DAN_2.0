#include "message_dispatcher.hpp"

#include "message.hpp"
#include "message_bus.hpp"

#include "core/diagnostics/diagnostics.hpp"
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

        Diagnostics::PrintLine("[DISPATCH] Dequeue");

        const ModuleId receiver = MessageHeader::GetReceiver(message.header);

        if (receiver == MessageProtocol::Broadcast)
        {
            Diagnostics::PrintLine("[DISPATCH] Route -> Broadcast");
            ModuleTable::ReceiveBroadcast(message);
            Diagnostics::PrintLine("[DISPATCH] Delivered");
            return true;
        }

        Module* module = ModuleTable::Find(receiver);

        if (module == nullptr)
        {
            Diagnostics::PrintLine("[DISPATCH] Receiver not found");
            return false;
        }

        Diagnostics::Print("[DISPATCH] Route -> ");
        Diagnostics::PrintLine(module->GetName());

        module->Receive(message);

        Diagnostics::PrintLine("[DISPATCH] Delivered");
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
