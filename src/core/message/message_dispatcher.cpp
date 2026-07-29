#include "message_dispatcher.hpp"

#include "message_bus.hpp"

#include "core/diagnostics/diagnostics.hpp"
#include "core/module/module.hpp"
#include "core/module/module_table.hpp"

namespace
{
    bool initialized = false;
    bool running = false;
    dan::core::GlobalId localDeviceId =
        dan::core::MessageProtocol::InvalidGlobal;
    dan::core::INetworkTransport* networkTransport = nullptr;

    bool IsValidLocalMessage(const dan::core::Message& message)
    {
        using namespace dan::core;

        const GlobalId globalReceiver =
            MessageHeader::GetGlobalReceiver(message.header);
        const ModuleId sender = MessageHeader::GetSender(message.header);
        const ModuleId receiver = MessageHeader::GetReceiver(message.header);
        const MessageType type = MessageHeader::GetType(message.header);

        if (!MessageProtocol::IsGlobalReceiver(globalReceiver) ||
            !MessageProtocol::IsModuleSender(sender) ||
            !MessageProtocol::IsModuleReceiver(receiver) ||
            !MessageProtocol::IsDefinedType(type))
        {
            return false;
        }

        if (type == MessageType::System &&
            globalReceiver != localDeviceId)
        {
            return false;
        }

        return true;
    }

    bool IsValidNetworkMessage(const dan::core::Message& message)
    {
        using namespace dan::core;

        const GlobalId globalSender =
            MessageHeader::GetGlobalSender(message.header);
        const GlobalId globalReceiver =
            MessageHeader::GetGlobalReceiver(message.header);
        const ModuleId sender = MessageHeader::GetSender(message.header);
        const ModuleId receiver = MessageHeader::GetReceiver(message.header);
        const MessageType type = MessageHeader::GetType(message.header);

        if (!MessageProtocol::IsDeviceId(globalSender) ||
            !MessageProtocol::IsModuleSender(sender) ||
            !MessageProtocol::IsModuleReceiver(receiver) ||
            !MessageProtocol::IsNetworkVisible(type))
        {
            return false;
        }

        return globalReceiver == localDeviceId ||
               globalReceiver == MessageProtocol::GlobalBroadcast;
    }

    dan::core::NetworkDispatchResult ConvertTransportResult(
        dan::core::TransportSendResult result)
    {
        using namespace dan::core;

        switch (result)
        {
            case TransportSendResult::Accepted:
                return NetworkDispatchResult::Accepted;

            case TransportSendResult::NotReady:
                return NetworkDispatchResult::NotReady;

            case TransportSendResult::QueueFull:
                return NetworkDispatchResult::QueueFull;

            case TransportSendResult::Rejected:
            default:
                return NetworkDispatchResult::Rejected;
        }
    }

    void PrintNetworkDispatchResult(
    dan::core::NetworkDispatchResult result)
    {
        using namespace dan::core;

        switch (result)
        {
            case NetworkDispatchResult::NotRequested:
                Diagnostics::PrintLine(
                    F("[DISPATCH] Network: not requested"));
                break;

            case NetworkDispatchResult::Accepted:
                Diagnostics::PrintLine(
                    F("[DISPATCH] Network: accepted"));
                break;

            case NetworkDispatchResult::NoTransport:
                Diagnostics::PrintLine(
                    F("[DISPATCH] Network: no transport"));
                break;

            case NetworkDispatchResult::NotReady:
                Diagnostics::PrintLine(
                    F("[DISPATCH] Network: transport not ready"));
                break;

            case NetworkDispatchResult::QueueFull:
                Diagnostics::PrintLine(
                    F("[DISPATCH] Network: queue full"));
                break;

            case NetworkDispatchResult::Rejected:
            default:
                Diagnostics::PrintLine(
                    F("[DISPATCH] Network: rejected"));
                break;
        }
    }
}

namespace dan::core
{
    bool MessageDispatcher::Initialize(GlobalId deviceId)
    {
        if (running)
        {
            return false;
        }

        if (!MessageProtocol::IsDeviceId(deviceId))
        {
            initialized = false;
            running = false;
            localDeviceId = MessageProtocol::InvalidGlobal;
            networkTransport = nullptr;
            return false;
        }

        localDeviceId = deviceId;
        networkTransport = nullptr;
        running = false;
        initialized = true;

        Diagnostics::PrintLine(F("[DISPATCH] Initialized"));
        return true;
    }

    bool MessageDispatcher::SetNetworkTransport(
        INetworkTransport* transport)
    {
        if (!initialized || running)
        {
            return false;
        }

        networkTransport = transport;
        return true;
    }

    bool MessageDispatcher::Start()
    {
        if (!initialized || running)
        {
            return false;
        }

        running = true;
        Diagnostics::PrintLine(F("[DISPATCH] Started"));
        return true;
    }

    void MessageDispatcher::Stop()
    {
        if (!running)
        {
            return;
        }

        running = false;
        Diagnostics::PrintLine(F("[DISPATCH] Stopped"));
    }

    DispatchResult MessageDispatcher::SubmitLocal(
        const Message& message)
    {
        Diagnostics::PrintLine(F("[DISPATCH] SubmitLocal: entered"));

        DispatchResult result
        {
            false,
            NetworkDispatchResult::NotRequested
        };

        if (!running)
        {
            Diagnostics::PrintLine(
                F("[DISPATCH] SubmitLocal: rejected, dispatcher not running"));

            result.networkResult = NetworkDispatchResult::Rejected;
            return result;
        }

        if (!IsValidLocalMessage(message))
        {
            Diagnostics::PrintLine(
                F("[DISPATCH] SubmitLocal: rejected, invalid message"));

            result.networkResult = NetworkDispatchResult::Rejected;
            return result;
        }

        Message routedMessage = message;

        routedMessage.header = MessageHeader::SetGlobalSender(
            routedMessage.header,
            localDeviceId
        );

        Diagnostics::PrintLine(
            F("[DISPATCH] SubmitLocal: GlobalSender <- LocalDeviceId"));

        const GlobalId globalReceiver =
            MessageHeader::GetGlobalReceiver(routedMessage.header);

        const MessageType type =
            MessageHeader::GetType(routedMessage.header);

        const bool requiresLocalDelivery =
            globalReceiver == localDeviceId ||
            globalReceiver == MessageProtocol::GlobalBroadcast;

        const bool requiresNetworkDelivery =
            MessageProtocol::IsNetworkVisible(type) &&
            globalReceiver != localDeviceId;

        if (requiresLocalDelivery)
        {
            Diagnostics::PrintLine(
                F("[DISPATCH] SubmitLocal: route -> local"));

            result.localAccepted = MessageBus::Publish(routedMessage);

            Diagnostics::PrintLine(
                result.localAccepted
                    ? F("[DISPATCH] SubmitLocal: local accepted")
                    : F("[DISPATCH] SubmitLocal: local rejected"));
        }
        else
        {
            Diagnostics::PrintLine(
                F("[DISPATCH] SubmitLocal: local not requested"));
        }

        if (!requiresNetworkDelivery)
        {
            Diagnostics::PrintLine(
                F("[DISPATCH] SubmitLocal: network not requested"));

            return result;
        }

        Diagnostics::PrintLine(
            F("[DISPATCH] SubmitLocal: route -> network"));

        if (networkTransport == nullptr)
        {
            result.networkResult = NetworkDispatchResult::NoTransport;
            PrintNetworkDispatchResult(result.networkResult);
            return result;
        }

        result.networkResult = ConvertTransportResult(
            networkTransport->Send(routedMessage)
        );

        PrintNetworkDispatchResult(result.networkResult);

        return result;
    }

    bool MessageDispatcher::SubmitNetwork(const Message& message)
    {
        Diagnostics::PrintLine(F("[DISPATCH] SubmitNetwork: entered"));

        if (!running)
        {
            Diagnostics::PrintLine(
                F("[DISPATCH] SubmitNetwork: rejected, dispatcher not running"));

            return false;
        }

        if (!IsValidNetworkMessage(message))
        {
            Diagnostics::PrintLine(
                F("[DISPATCH] SubmitNetwork: rejected, invalid message"));

            return false;
        }

        Diagnostics::PrintLine(
            F("[DISPATCH] SubmitNetwork: accepted from network"));

        Diagnostics::PrintLine(
            F("[DISPATCH] SubmitNetwork: network echo blocked"));

        Diagnostics::PrintLine(
            F("[DISPATCH] SubmitNetwork: route -> local"));

        const bool accepted = MessageBus::Publish(message);

        Diagnostics::PrintLine(
            accepted
                ? F("[DISPATCH] SubmitNetwork: local accepted")
                : F("[DISPATCH] SubmitNetwork: local rejected"));

        return accepted;
    }

    bool MessageDispatcher::ProcessOne()
    {
        Message message{};

        if (!MessageBus::Receive(message))
        {
            return false;
        }

        Diagnostics::PrintLine(F("[DISPATCH] Dequeue"));

        const ModuleId receiver = MessageHeader::GetReceiver(message.header);

        if (receiver == MessageProtocol::Broadcast)
        {
            Diagnostics::PrintLine(F("[DISPATCH] Route -> Broadcast"));
            ModuleTable::ReceiveBroadcast(message);
            Diagnostics::PrintLine(F("[DISPATCH] Delivered"));
            return true;
        }

        Module* module = ModuleTable::Find(receiver);

        if (module == nullptr)
        {
            Diagnostics::PrintLine(F("[DISPATCH] Receiver not found"));
            return false;
        }

        Diagnostics::Print(F("[DISPATCH] Route -> "));
        Diagnostics::PrintLine(module->GetName());

        module->Receive(message);

        Diagnostics::PrintLine(F("[DISPATCH] Delivered"));
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

    GlobalId MessageDispatcher::GetLocalDeviceId()
    {
        return localDeviceId;
    }
}
