# Checkpoint — DAN Message Header Redistribution

**Project:** DAN Platform 2.0  
**Date:** 2026-07-28  
**Status:** Implemented and host-tested

## Approved message structure

The DAN message remains exactly 8 bytes:

```text
Header   32 bits
ObjectId 16 bits
Value    16 bits
```

The approved header map is:

```text
31..27 GlobalReceiver  5 bits
26..22 GlobalSender    5 bits
21..16 Receiver        6 bits
15..10 Sender          6 bits
 9..8  Type            2 bits
 7..0  MessageId       8 bits
```

## Removed field

`MessageAttribute` was removed because it had no approved semantics in the current low-level DAN Platform scope.

## Addressing capacity

```text
32 global device values
64 local module values
4 message types
256 message identifiers
```

The existing local special identifiers were retained in the six-bit module space:

```text
InvalidModule = 0x00
Broadcast     = 0x3F
```

## Implementation boundary

This checkpoint changes the message representation, packing helpers, message creation sites, tests, and current protocol documentation.

The local dispatcher continues to route by `Receiver`. No device registry, transport routing, or global-address filtering was introduced in this brick.
