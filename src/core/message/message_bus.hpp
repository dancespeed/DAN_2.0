#pragma once

#include <stdint.h>

#include "message.hpp"

namespace dan::core
{
    class MessageBus
    {
    public:
        static void Initialize();
        static bool Publish(const Message& message);
        static bool Receive(Message& message);
        static bool HasMessages();
        static uint8_t Count();
        static void Clear();
    };
}
