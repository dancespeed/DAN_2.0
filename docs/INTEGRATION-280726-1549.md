# DAN Message Integration

The current project contains the approved fixed-size DAN message subsystem.

## Message size

```text
Header   32 bits
ObjectId 16 bits
Value    16 bits
Total    64 bits / 8 bytes
```

## Header layout

```text
31..27 GlobalReceiver
26..22 GlobalSender
21..16 Receiver
15..10 Sender
 9..8  MessageType
 7..0  MessageId
```

The previous `MessageAttribute` field has been removed. Its bits were redistributed to global device addressing and revised local module addressing.

## Message types

```text
Command  = 0
Event    = 1
System   = 2
Reserved = 3
```

`Command` and `Event` may participate in network exchange. `System` is local to the current device. `Reserved` must not be generated or forwarded in the current protocol version.

The shared transport policy is available through:

```cpp
MessageProtocol::IsNetworkVisible(type)
```

No CAN or other external transport module is implemented in the current brick. When such a module is introduced, it must apply this policy at the network boundary.

## Integrated components

```text
Message
MessageQueue
MessageBus
MessageDispatcher
ModuleTable
Module::Receive()
```

## Current delivery path

```text
Module
    ↓
MessageBus::Publish()
    ↓
FIFO
    ↓
MessageDispatcher
    ↓
ModuleTable
    ↓
Module::Receive()
```

`MessageBus` stores messages without interpreting their fields. The current `MessageDispatcher` performs local routing by `Receiver`. Network transport and filtering by `GlobalReceiver` remain outside this integration brick.
