#pragma once

#include <stdint.h>

// -----------------------------------------------------------------------------
// DAN Internal Message Protocol v1
//
// Message size: 64 bits
//
// Header layout:
//
// 31             24 23             16 15          12 11              0
// ┌────────────────┬─────────────────┬──────────────┬─────────────────┐
// │   Sender ID    │   Receiver ID   │  Attribute   │   Message ID    │
// │     8 bits     │      8 bits     │    4 bits    │     12 bits     │
// └────────────────┴─────────────────┴──────────────┴─────────────────┘
//
// Payload layout:
//
// 31                             16 15                              0
// ┌────────────────────────────────┬────────────────────────────────┐
// │           Object ID            │             Value              │
// │            16 bits             │            16 bits             │
// └────────────────────────────────┴────────────────────────────────┘
// -----------------------------------------------------------------------------

using ModuleId = uint8_t;
using MessageAttribute = uint8_t;
using MessageId = uint16_t;
using ObjectId = uint16_t;
using MessageValue = uint16_t;

constexpr ModuleId NoReceiver = 0x00;
constexpr ModuleId Broadcast = 0xFF;

constexpr MessageAttribute NoAttribute = 0x00;

struct Message
{
    uint32_t header;
    ObjectId objectId;
    MessageValue value;
};

namespace MessageHeader
{
    constexpr uint8_t SenderShift = 24;
    constexpr uint8_t ReceiverShift = 16;
    constexpr uint8_t AttributeShift = 12;

    constexpr uint32_t ModuleMask = 0x00FF;
    constexpr uint32_t AttributeMask = 0x000F;
    constexpr uint32_t MessageIdMask = 0x0FFF;

    constexpr uint32_t Create(
        ModuleId sender,
        ModuleId receiver,
        MessageAttribute attribute,
        MessageId messageId)
    {
        return
            (static_cast<uint32_t>(sender) << SenderShift) |
            (static_cast<uint32_t>(receiver) << ReceiverShift) |
            (
                (static_cast<uint32_t>(attribute) & AttributeMask)
                << AttributeShift
            ) |
            (static_cast<uint32_t>(messageId) & MessageIdMask);
    }

    constexpr ModuleId GetSender(
        uint32_t header)
    {
        return static_cast<ModuleId>(
            (header >> SenderShift) & ModuleMask
        );
    }

    constexpr ModuleId GetReceiver(
        uint32_t header)
    {
        return static_cast<ModuleId>(
            (header >> ReceiverShift) & ModuleMask
        );
    }

    constexpr MessageAttribute GetAttribute(
        uint32_t header)
    {
        return static_cast<MessageAttribute>(
            (header >> AttributeShift) & AttributeMask
        );
    }

    constexpr MessageId GetMessageId(
        uint32_t header)
    {
        return static_cast<MessageId>(
            header & MessageIdMask
        );
    }

    constexpr bool IsValidAttribute(
        MessageAttribute attribute)
    {
        return
            (
                static_cast<uint32_t>(attribute) &
                ~AttributeMask
            ) == 0;
    }

    constexpr bool IsValidMessageId(
        MessageId messageId)
    {
        return
            (
                static_cast<uint32_t>(messageId) &
                ~MessageIdMask
            ) == 0;
    }
}

static_assert(
    sizeof(Message) == 8,
    "DAN Message must be exactly 8 bytes"
);