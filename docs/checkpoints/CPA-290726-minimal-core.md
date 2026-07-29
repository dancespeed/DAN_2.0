{
  "checkpoint_schema": "dan-platform-checkpoint-v1",
  "checkpoint": "CPA-290726-minimal-core",
  "created_at": "2026-07-29",
  "project": {
    "name": "DAN Platform 2.0",
    "domain": "embedded systems",
    "language": "C++17",
    "target": "ATmega328P",
    "environment": [
      "Visual Studio Code",
      "PlatformIO"
    ],
    "assistant_name": "Architect"
  },
  "brick": {
    "goal": "Allow the standard DAN core to initialize, start, run, and stop without Logic or any application module.",
    "status": "implemented_and_host_verified",
    "base_commit": "8cad083",
    "source_changes": [
      {
        "file": "src/core/runtime/runtime.cpp",
        "changes": [
          "Removed the TestModule include.",
          "Removed the static TestModule instance.",
          "Removed automatic application-module registration.",
          "Kept ModuleTable::InitializeModules() valid for an empty table."
        ]
      },
      {
        "file": "src/main.cpp",
        "changes": [
          "Removed the routing_device_test include.",
          "Removed the automatic RoutingDeviceTest::Run() call.",
          "Kept normal System initialization, startup, and run loop."
        ]
      }
    ]
  },
  "frozen_decisions": [
    "Runtime must not construct or register application modules.",
    "An empty ModuleTable is a valid standard-core state.",
    "Logic is device-specific and is not required for core startup.",
    "TestModule and RoutingDeviceTest remain diagnostic sources and are not part of normal startup.",
    "MessageBus and MessageDispatcher remain active in the minimal core.",
    "This brick does not introduce a second configuration path."
  ],
  "current_runtime_sequence": [
    "ModuleTable::Initialize()",
    "MessageBus::Initialize()",
    "MessageDispatcher::Initialize(config::LocalDeviceId)",
    "ModuleTable::InitializeModules()",
    "MessageDispatcher::Start()",
    "ModuleTable::Start()",
    "MessageDispatcher::ProcessAll()",
    "ModuleTable::Run()"
  ],
  "configuration_state": {
    "local_device_id": "compile-time config::LocalDeviceId",
    "configuration_table": "not implemented",
    "eeprom": "not implemented",
    "system_module_ids": "not yet frozen in code",
    "application_module_registration": "not yet introduced after removal of TestModule hard-coding"
  },
  "verification": {
    "passed": [
      "empty Runtime lifecycle host smoke test",
      "message_core_test",
      "host compilation with -Wall -Wextra -Werror",
      "git diff --check"
    ],
    "not_run": [
      "PlatformIO AVR build",
      "avr-g++ build",
      "target-device startup verification"
    ],
    "not_run_reason": "PlatformIO and avr-g++ are unavailable in the current environment."
  },
  "explicitly_excluded": [
    "system ModuleID registry",
    "EEPROM module",
    "ConfigurationTable playback",
    "GlobalID storage decision",
    "Logic",
    "BLE, PWM, channel expander, encoder, keyboard, buttons, and sensor modules"
  ],
  "next_brick": {
    "goal": "Freeze the immutable system ModuleID registry.",
    "ids": {
      "System": "0x01",
      "Message": "0x02",
      "EEPROM": "0x03",
      "Network": "0x04",
      "Runtime": "0x05"
    },
    "do_not_include": [
      "EEPROM implementation",
      "configuration playback",
      "application module registration"
    ]
  },
  "assistant_resume_instruction": "Restore this checkpoint and continue brick by brick. The core already runs without application modules. Discuss and implement only the immutable system ModuleID registry next; do not add EEPROM or configuration playback in the same brick."
}
