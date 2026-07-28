# Checkpoint — DAN MessageType Semantics

**Project:** DAN Platform 2.0  
**Date:** 2026-07-28  
**Status:** Implemented and host-tested

## Approved values

The two-bit `MessageType` field is defined as:

```text
0x00 Command
0x01 Event
0x02 System
0x03 Reserved
```

## Semantics

- `Command` requests an operation from the receiver.
- `Event` reports a fact, state, value, or completed result.
- `System` identifies internal platform traffic that remains inside the current device.
- `Reserved` has no semantics in the current protocol version.

A separate request/response type pair is not used. `Get` and `Read` are commands; their returned data is represented as an event. Address fields determine direction.

## Network boundary

The message format remains common for all modules and all transports.

External network transports must apply the following policy:

```text
Command  -> may cross the network boundary
Event    -> may cross the network boundary
System   -> must remain local
Reserved -> must not be forwarded
```

The policy is encoded in `MessageProtocol::IsNetworkVisible()`.

No CAN transport module was added in this checkpoint. The local `MessageBus` and `MessageDispatcher` remain network-agnostic.

## Code changes

- `MessageType` is now a strongly typed `enum class`.
- The temporary `NoType` value was removed.
- Current message creation sites now use `Command` or `Event` explicitly.
- Tests cover numeric mapping, defined values, packing, and network visibility.
- Active protocol and integration documentation were updated.
