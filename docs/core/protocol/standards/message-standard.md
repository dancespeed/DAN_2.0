# DAN Internal Message Standard

## Status

Integration candidate based on the working first DAN Platform implementation and the current DAN architecture decisions.

## Fixed size

Every internal DAN message is exactly 8 bytes.

```text
Header   32 bits
ObjectId 16 bits
Value    16 bits
```

## Header

```text
31..24 Sender
23..16 Receiver
15..14 MessageType
13..12 MessageAttribute
11..0  MessageId
```

## Semantics

- `Sender` identifies the module that created the message.
- `Receiver` identifies the destination module.
- `MessageType` classifies the message.
- `MessageAttribute` carries two protocol attribute bits.
- `MessageId` identifies the command, event, request, or description.
- `ObjectId` identifies the subject inside the receiver's standardized protocol.
- `Value` is a raw 16-bit value interpreted by that protocol.

## Delivery

```text
Sender -> MessageBus FIFO -> MessageDispatcher -> ModuleTable -> Module::Receive()
```

The bus stores and transports messages only. The receiving module interprets `ObjectId`, `MessageId`, and `Value`.
