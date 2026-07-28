#pragma once

#include <stdint.h>

namespace dan::core
{
    using GlobalId = uint8_t;
    using ModuleId = uint8_t;
    using MessageId = uint8_t;
    using ObjectId = uint16_t;
    using MessageValue = uint16_t;

    enum class MessageType : uint8_t
    {
        Command = 0x00,
        Event = 0x01,
        System = 0x02,
        Reserved = 0x03
    };

    namespace MessageProtocol
    {
        inline constexpr ModuleId InvalidModule = 0x00;
        inline constexpr ModuleId Broadcast = 0x3F;

        constexpr bool IsDefinedType(MessageType type)
        {
            return type == MessageType::Command ||
                   type == MessageType::Event ||
                   type == MessageType::System;
        }

        constexpr bool IsNetworkVisible(MessageType type)
        {
            return type == MessageType::Command ||
                   type == MessageType::Event;
        }
    }

    struct Message
    {
        uint32_t header;
        ObjectId objectId;
        MessageValue value;
    };

    namespace MessageHeader
    {
        inline constexpr uint8_t MessageIdShift = 0;
        inline constexpr uint8_t TypeShift = 8;
        inline constexpr uint8_t SenderShift = 10;
        inline constexpr uint8_t ReceiverShift = 16;
        inline constexpr uint8_t GlobalSenderShift = 22;
        inline constexpr uint8_t GlobalReceiverShift = 27;

        inline constexpr uint32_t GlobalMask = 0x001F;
        inline constexpr uint32_t ModuleMask = 0x003F;
        inline constexpr uint32_t TypeMask = 0x0003;
        inline constexpr uint32_t MessageIdMask = 0x00FF;

        constexpr uint32_t Create(
            GlobalId globalSender,
            GlobalId globalReceiver,
            ModuleId sender,
            ModuleId receiver,
            MessageType type,
            MessageId messageId)
        {
            return
                ((static_cast<uint32_t>(globalReceiver) & GlobalMask)
                    << GlobalReceiverShift) |
                ((static_cast<uint32_t>(globalSender) & GlobalMask)
                    << GlobalSenderShift) |
                ((static_cast<uint32_t>(receiver) & ModuleMask)
                    << ReceiverShift) |
                ((static_cast<uint32_t>(sender) & ModuleMask)
                    << SenderShift) |
                ((static_cast<uint32_t>(type) & TypeMask)
                    << TypeShift) |
                ((static_cast<uint32_t>(messageId) & MessageIdMask)
                    << MessageIdShift);
        }

        constexpr GlobalId GetGlobalSender(uint32_t header)
        {
            return static_cast<GlobalId>(
                (header >> GlobalSenderShift) & GlobalMask
            );
        }

        constexpr GlobalId GetGlobalReceiver(uint32_t header)
        {
            return static_cast<GlobalId>(
                (header >> GlobalReceiverShift) & GlobalMask
            );
        }

        constexpr ModuleId GetSender(uint32_t header)
        {
            return static_cast<ModuleId>(
                (header >> SenderShift) & ModuleMask
            );
        }

        constexpr ModuleId GetReceiver(uint32_t header)
        {
            return static_cast<ModuleId>(
                (header >> ReceiverShift) & ModuleMask
            );
        }

        constexpr MessageType GetType(uint32_t header)
        {
            return static_cast<MessageType>(
                (header >> TypeShift) & TypeMask
            );
        }

        constexpr MessageId GetMessageId(uint32_t header)
        {
            return static_cast<MessageId>(
                (header >> MessageIdShift) & MessageIdMask
            );
        }

        constexpr bool IsValidGlobalId(GlobalId id)
        {
            return (static_cast<uint32_t>(id) & ~GlobalMask) == 0;
        }

        constexpr bool IsValidModuleId(ModuleId id)
        {
            return (static_cast<uint32_t>(id) & ~ModuleMask) == 0;
        }

        constexpr bool IsValidType(MessageType type)
        {
            return (static_cast<uint32_t>(type) & ~TypeMask) == 0;
        }
    }

    static_assert(sizeof(Message) == 8, "DAN Message must be exactly 8 bytes");
}
