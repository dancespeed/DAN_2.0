# Checkpoint — Local Message Routing Verified on Device

**Project:** DAN Platform 2.0  
**Date:** 2026-07-28  
**Status:** Firmware build and local routing path verified on the target device

## Checkpoint scope

This checkpoint freezes the first verified routing path:

```text
Local module
    -> MessageDispatcher::SubmitLocal()
    -> GlobalSender replaced with LocalDeviceId
    -> local route selected
    -> MessageBus::Publish()
    -> MessageDispatcher::ProcessOne()
    -> target Module::Receive()
```

The active local device identifier is:

```text
LocalDeviceId = 1
```

## Verified behavior

The following behavior was observed on the target device:

- `MessageDispatcher::SubmitLocal()` is entered;
- the dispatcher replaces `GlobalSender` with `LocalDeviceId`;
- a message addressed to the local device selects local delivery;
- the message is accepted by `MessageBus`;
- network delivery is not requested for a local destination;
- queued messages are dequeued and delivered to `TestModule`;
- module lifecycle diagnostics continue to operate.

## Device diagnostic trace

```text
[DIAG] Routing stage start
[BUS] Initialized
[DISPATCH] Initialized
[MODULE] Initialize OK: TestModule
[DISPATCH] Started
[DISPATCH] SubmitLocal: entered
[DISPATCH] SubmitLocal: GlobalSender <- LocalDeviceId
[DISPATCH] SubmitLocal: route -> local
[BUS] Publish OK
[DISPATCH] SubmitLocal: local accepted
[DISPATCH] SubmitLocal: network not requested
[MODULE] Start OK: TestModule
[BOOT] System started
[DISPATCH] SubmitLocal: entered
[DISPATCH] SubmitLocal: GlobalSender <- LocalDeviceId
[DISPATCH] SubmitLocal: route -> local
[BUS] Publish OK
[DISPATCH] SubmitLocal: local accepted
[DISPATCH] SubmitLocal: network not requested
[BUS] Receive OK
[DISPATCH] Dequeue
[DISPATCH] Route -> TestModule
[DISPATCH] Delivered
[BUS] Receive OK
[DISPATCH] Dequeue
[DISPATCH] Route -> TestModule
[DISPATCH] Delivered
[MODULE] Run: TestModule
```

Two messages are expected in this test configuration:

1. an event submitted from `TestModule::OnStart()`;
2. a command submitted from `setup()` after the system starts.

## Frozen decisions included in this snapshot

- `SubmitLocal()` and `SubmitNetwork()` are separate origin entry points;
- local messages cannot spoof another device because the dispatcher owns `GlobalSender`;
- local, remote, and global-broadcast routing decisions remain in `MessageDispatcher`;
- network-origin messages cannot be returned to the network;
- `System` messages remain local;
- `Reserved` messages are rejected;
- `INetworkTransport` is non-owning and optional;
- transport send results are `Accepted`, `NotReady`, `QueueFull`, and `Rejected`;
- the standard future transport TX queue capacity is eight messages.

## Not yet verified on hardware

This checkpoint does not claim device verification of:

- a remote destination routed through an active transport;
- global broadcast routed both locally and to the network;
- `SubmitNetwork()` local delivery;
- transport results `Accepted`, `NotReady`, `QueueFull`, or `Rejected`;
- a physical CAN transport or its TX queue.

## Known test-suite inconsistency

The firmware snapshot builds and runs on the target device, but
`test/message_core_test.cpp` still targets the removed legacy
`MessageAttribute` layout and the old `MessageHeader::Create()` signature.
It is therefore not a valid host test for this checkpoint and must be adapted
in a separate approved brick. No source-code change was made silently during
checkpoint creation.

## Source snapshot

The checkpoint was created from the user-provided archive with SHA-256:

```text
cf4798845661929cdee16ee034d22bfa2b228f05a7fed7881128b16e63b91d35
```
