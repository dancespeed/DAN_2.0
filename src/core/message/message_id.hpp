#pragma once

#include <stdint.h>

#include "event/event.hpp"

using MessageId = uint16_t;

namespace MessageIds
{
    constexpr MessageId None =
        static_cast<MessageId>(EventType::None);

    constexpr MessageId SystemHeartbeat =
        static_cast<MessageId>(EventType::SystemHeartbeat);

    constexpr MessageId IgnitionOn =
        static_cast<MessageId>(EventType::IgnitionOn);

    constexpr MessageId IgnitionOff =
        static_cast<MessageId>(EventType::IgnitionOff);

    constexpr MessageId UserCommand =
        static_cast<MessageId>(EventType::UserCommand);

    constexpr MessageId SetChannelState =
        static_cast<MessageId>(EventType::SetChannelState);

    constexpr MessageId ChannelStateChanged =
        static_cast<MessageId>(EventType::ChannelStateChanged);

    constexpr MessageId StartSequence =
        static_cast<MessageId>(EventType::StartSequence);

    constexpr MessageId SequenceFinished =
        static_cast<MessageId>(EventType::SequenceFinished);

    constexpr MessageId RequestEepromFlush =
        static_cast<MessageId>(EventType::RequestEepromFlush);

    constexpr MessageId RequestSleep =
        static_cast<MessageId>(EventType::RequestSleep);
}