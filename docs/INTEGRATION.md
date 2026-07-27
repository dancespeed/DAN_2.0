# DAN Message Integration — Checkpoint 1

This package contains the first migration brick:

- fixed 8-byte `Message`;
- split 4-bit legacy attribute field into 2-bit `MessageType` and 2-bit `MessageAttribute`;
- static FIFO ring buffer;
- static `MessageBus`;
- thin dispatcher candidate for `ModuleTable -> Module::Receive()`.

## Files that can be integrated immediately

Copy the following directory into the current project:

```text
src/core/message/
```

The following files are independent from module implementation and were compile-tested:

```text
message.hpp
message_queue.hpp
message_bus_config.hpp
message_bus.hpp
message_bus.cpp
```

## Dispatcher dependencies

`message_dispatcher.cpp` expects the current platform to provide:

```cpp
Module* ModuleTable::Find(ModuleId id);
void ModuleTable::ReceiveBroadcast(const Message& message);
void Module::Receive(const Message& message);
```

These exact current module files were not present in the supplied archive, so they were not overwritten blindly.

## Runtime integration point

Expected order:

```text
Runtime::Initialize()
    MessageBus::Initialize()
    ModuleTable::Initialize()
    RegisterModules()
    ModuleTable::InitializeModules()

Runtime::Run()
    MessageDispatcher::ProcessOne()
    ModuleTable::Run()
```

Whether dispatch happens before or after `ModuleTable::Run()` must be frozen against the actual current `runtime.cpp` before final merge.
