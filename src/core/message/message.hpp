#pragma once

#include <stdint.h>

namespace dan::core
{
    using ModuleId = uint8_t;
    using MessageType = uint8_t;
    using MessageAttribute = uint8_t;
    using MessageId = uint16_t;
    using ObjectId = uint16_t;
    using MessageValue = uint16_t;

    namespace MessageProtocol
    {
        inline constexpr ModuleId InvalidModule = 0x00;
        inline constexpr ModuleId Broadcast = 0xFF;

        inline constexpr MessageType NoType = 0x00;
        inline constexpr MessageAttribute NoAttribute = 0x00;
    }

    struct Message
    {
        uint32_t header;
        ObjectId objectId;
        MessageValue value;
    };

    namespace MessageHeader
    {
        inline constexpr uint8_t SenderShift = 24;
        inline constexpr uint8_t ReceiverShift = 16;
        inline constexpr uint8_t TypeShift = 14;
        inline constexpr uint8_t AttributeShift = 12;

        inline constexpr uint32_t ModuleMask = 0x00FF;
        inline constexpr uint32_t TypeMask = 0x0003;
        inline constexpr uint32_t AttributeMask = 0x0003;
        inline constexpr uint32_t MessageIdMask = 0x0FFF;

        constexpr uint32_t Create(
            ModuleId sender,
            ModuleId receiver,
            MessageType type,
            MessageAttribute attributes,
            MessageId messageId)
        {
            return
                (static_cast<uint32_t>(sender) << SenderShift) |
                (static_cast<uint32_t>(receiver) << ReceiverShift) |
                ((static_cast<uint32_t>(type) & TypeMask) << TypeShift) |
                ((static_cast<uint32_t>(attributes) & AttributeMask) << AttributeShift) |
                (static_cast<uint32_t>(messageId) & MessageIdMask);
        }

        constexpr ModuleId GetSender(uint32_t header)
        {
            return static_cast<ModuleId>((header >> SenderShift) & ModuleMask);
        }

        constexpr ModuleId GetReceiver(uint32_t header)
        {
            return static_cast<ModuleId>((header >> ReceiverShift) & ModuleMask);
        }

        constexpr MessageType GetType(uint32_t header)
        {
            return static_cast<MessageType>((header >> TypeShift) & TypeMask);
        }

        constexpr MessageAttribute GetAttributes(uint32_t header)
        {
            return static_cast<MessageAttribute>(
                (header >> AttributeShift) & AttributeMask
            );
        }

        constexpr MessageId GetMessageId(uint32_t header)
        {
            return static_cast<MessageId>(header & MessageIdMask);
        }

        constexpr bool HasAttribute(
            uint32_t header,
            MessageAttribute attribute)
        {
            return (GetAttributes(header) & attribute) == attribute;
        }

        constexpr bool IsValidType(MessageType type)
        {
            return (static_cast<uint32_t>(type) & ~TypeMask) == 0;
        }

        constexpr bool IsValidAttributes(MessageAttribute attributes)
        {
            return (static_cast<uint32_t>(attributes) & ~AttributeMask) == 0;
        }

        constexpr bool IsValidMessageId(MessageId messageId)
        {
            return (static_cast<uint32_t>(messageId) & ~MessageIdMask) == 0;
        }
    }

    static_assert(sizeof(Message) == 8, "DAN Message must be exactly 8 bytes");
}
