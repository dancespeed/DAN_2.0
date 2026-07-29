{
  "checkpoint_schema": "dan-platform-checkpoint-v1",
  "checkpoint": "CPA-290726-system-module-ids",
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
    "goal": "Freeze the immutable DAN system ModuleID registry and separate it from application module IDs.",
    "status": "implemented_and_host_verified",
    "base_commit": "a1b7758",
    "source_changes": [
      {
        "file": "src/core/module/module_ids.hpp",
        "changes": [
          "Defined System as 0x01.",
          "Defined Message as 0x02.",
          "Defined Eeprom as 0x03.",
          "Defined Network as 0x04.",
          "Defined Runtime as 0x05.",
          "Defined Configuration as 0x06.",
          "Reserved the complete core range 0x01 through 0x0F.",
          "Defined the application range 0x10 through 0x3E.",
          "Added constexpr range checks and static assertions."
        ]
      },
      {
        "file": "src/modules/test/test_module.hpp",
        "changes": [
          "Moved the diagnostic TestModule ID out of the core registry.",
          "Assigned TestModule the first application ID, 0x10."
        ]
      },
      {
        "file": "src/modules/test/test_module.cpp",
        "changes": [
          "Return the module-owned TestModule ID."
        ]
      },
      {
        "file": "src/core/diagnostics/routing_device_test.cpp",
        "changes": [
          "Use the module-owned TestModule ID."
        ]
      },
      {
        "file": "test/message_core_test.cpp",
        "changes": [
          "Use application-range IDs for host test modules.",
          "Compile-time check both test IDs against the application range."
        ]
      }
    ]
  },
  "immutable_registry": {
    "invalid": "0x00",
    "system": "0x01",
    "message": "0x02",
    "eeprom": "0x03",
    "network": "0x04",
    "runtime": "0x05",
    "configuration": "0x06",
    "core_reserved": "0x07-0x0F",
    "application": "0x10-0x3E",
    "broadcast": "0x3F"
  },
  "frozen_decisions": [
    "The entire 0x01 through 0x0F ModuleID range belongs to the DAN core standard.",
    "System IDs are identical across all DAN devices.",
    "An omitted system component does not release its ID for application use.",
    "Application modules, including Logic, use only 0x10 through 0x3E.",
    "Invalid 0x00 and Broadcast 0x3F remain protocol sentinels.",
    "Reserving an ID does not require the corresponding core component to derive from Module.",
    "TestModule is diagnostic application code and uses 0x10."
  ],
  "verification": {
    "passed": [
      "module registry compile-time assertions",
      "message_core_test",
      "empty Runtime lifecycle host smoke test",
      "host compilation with -Wall -Wextra -Werror",
      "git diff --check"
    ],
    "not_run": [
      "PlatformIO AVR build",
      "avr-g++ build",
      "target-device verification"
    ],
    "not_run_reason": "PlatformIO and avr-g++ are unavailable in the current environment."
  },
  "explicitly_excluded": [
    "addressable implementations for reserved system IDs",
    "EEPROM implementation",
    "ConfigurationTable playback",
    "runtime ModuleID assignment",
    "GlobalID storage decision",
    "Logic and other device-specific modules"
  ],
  "assistant_resume_instruction": "Restore this checkpoint and continue brick by brick. The immutable ModuleID registry is frozen. Do not reassign 0x01 through 0x0F to application modules. EEPROM, ConfigurationTable playback, runtime ID assignment, and GlobalID storage remain separate future bricks."
}
