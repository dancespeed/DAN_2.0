#pragma once

#include <stdint.h>

#include "core/message/message.hpp"
#include "core/network/network_transport.hpp"

namespace dan::core
{
    enum class NetworkDispatchResult : uint8_t
    {
        NotRequested,
        Accepted,
        NoTransport,
        NotReady,
        QueueFull,
        Rejected
    };

    struct DispatchResult
    {
        bool localAccepted;
        NetworkDispatchResult networkResult;
    };

    class MessageDispatcher final
    {
    public:
        static bool Initialize(GlobalId localDeviceId);
        static bool SetNetworkTransport(INetworkTransport* transport);
        static bool Start();
        static void Stop();

        static DispatchResult SubmitLocal(const Message& message);
        static bool SubmitNetwork(const Message& message);

        static bool ProcessOne();
        static uint8_t ProcessAll();

        [[nodiscard]]
        static GlobalId GetLocalDeviceId();

    private:
        MessageDispatcher() = delete;
    };
}
