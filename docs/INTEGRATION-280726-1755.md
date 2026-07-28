# DAN Message Routing Integration

The project contains the approved eight-byte DAN message format and the first routing layer between local modules and one optional network transport.

## Integrated components

```text
Message
MessageQueue
MessageBus
MessageDispatcher
INetworkTransport
ModuleTable
Module::Receive()
```

## Responsibility split

### MessageDispatcher

- owns the local `DeviceId` used for routing;
- provides `SubmitLocal()` and `SubmitNetwork()`;
- writes the correct `GlobalSender` for local messages;
- validates addresses and message type;
- selects local delivery, network delivery, or both;
- prevents an incoming network broadcast from being sent back to the network;
- knows only `INetworkTransport`, not CAN or another concrete transport.

### MessageBus

- is a passive fixed-size FIFO for local delivery;
- does not decide whether a message is local or remote;
- does not know about CAN or another transport.

### INetworkTransport

- accepts a complete eight-byte `Message` through `Send()`;
- does not change source or destination fields;
- does not interpret `MessageId`, `ObjectId`, or `Value`;
- returns only the result of accepting the message for transmission.

## Local path

```text
Module
    -> MessageDispatcher::SubmitLocal()
    -> MessageBus
    -> MessageDispatcher::ProcessAll()
    -> ModuleTable
    -> Module::Receive()
```

## Outgoing network path

```text
Module
    -> MessageDispatcher::SubmitLocal()
    -> INetworkTransport::Send()
    -> future concrete transport
```

## Incoming network path

```text
future concrete transport
    -> MessageDispatcher::SubmitNetwork()
    -> MessageBus
    -> MessageDispatcher::ProcessAll()
    -> local module
```

## Local device configuration

The initial device identifier is configured in:

```text
src/core/config/runtime_config.hpp
```

Current default:

```text
LocalDeviceId = 1
```

Valid device identifiers are `1..30`. The dispatcher configuration is locked while it is running.

## Dispatch results

`SubmitLocal()` reports local queue acceptance separately from network submission:

```cpp
struct DispatchResult
{
    bool localAccepted;
    NetworkDispatchResult networkResult;
};
```

Possible network results:

```text
NotRequested
Accepted
NoTransport
NotReady
QueueFull
Rejected
```

`localAccepted` means that the message was placed into the local FIFO. It does not guarantee that the destination module exists or has already processed the message.

## Current transport status

No concrete CAN transport is included in this brick. The core supports operation without a transport. A remote message then returns `NoTransport`, while local messages continue to work normally.
