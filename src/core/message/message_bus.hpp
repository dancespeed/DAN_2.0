#pragma once

#include <stdint.h>

#include "message/message.hpp"

namespace MessageBus
{
    void Init();

    bool Publish(const Message& message);

    bool Receive(Message& message);

    bool HasMessages();

    uint8_t Count();

    void Clear();
}