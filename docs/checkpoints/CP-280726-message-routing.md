# Checkpoint: Message Routing Core

## Status

Implemented and host-tested.

## Fixed protocol

```text
GlobalId:
0      Invalid
1..30  Devices
31     GlobalBroadcast

ModuleId:
0      Invalid
1..62  Modules
63     Broadcast

MessageType:
0 Command
1 Event
2 System
3 Reserved
```

## Implemented architecture

```text
Local module
    -> MessageDispatcher::SubmitLocal()
       -> local MessageBus
       -> optional INetworkTransport

Network transport
    -> MessageDispatcher::SubmitNetwork()
       -> local MessageBus only
```

`GlobalSender` of a local message is always replaced with `LocalDeviceId` by the dispatcher.

## Routing policy

- local destination: local queue;
- remote destination: network transport;
- global broadcast: local queue plus network transport;
- incoming global broadcast: local queue only;
- system message: local destination only;
- reserved message: rejected.

## Transport contract

```text
TransportSendResult:
Accepted
NotReady
QueueFull
Rejected
```

Dispatcher-level network result additionally contains:

```text
NotRequested
NoTransport
```

The standard future transport TX queue capacity is eight messages.

## Files added

```text
src/core/network/network_transport.hpp
src/core/network/network_transport_config.hpp
```

## Files adapted

```text
src/core/message/message.hpp
src/core/message/message_dispatcher.hpp
src/core/message/message_dispatcher.cpp
src/core/config/runtime_config.hpp
src/core/runtime/runtime.cpp
src/modules/test/test_module.cpp
src/main.cpp
test/message_core_test.cpp
```
