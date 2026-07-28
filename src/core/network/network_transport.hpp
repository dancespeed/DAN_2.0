#pragma once

#include <stdint.h>

#include "core/message/message.hpp"

namespace dan::core
{
    enum class TransportSendResult : uint8_t
    {
        Accepted,
        NotReady,
        QueueFull,
        Rejected
    };

    class INetworkTransport
    {
    public:
        virtual ~INetworkTransport() = default;

        virtual TransportSendResult Send(const Message& message) = 0;
    };
}
