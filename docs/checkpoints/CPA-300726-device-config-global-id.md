{
  "checkpoint_schema": "dan-platform-checkpoint-v1",
  "checkpoint": "CPA-300726-device-config-global-id",
  "created_at": "2026-07-30",
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
    "goal": "Move the firmware GlobalID out of the core and place it directly after Version in the device configuration.",
    "status": "implemented_and_target_verified",
    "base_commit": "54b2705",
    "source_changes": [
      {
        "file": "src/config/device_config.hpp",
        "changes": [
          "Added compile-time device configuration outside the core.",
          "Defined Version first.",
          "Defined GlobalId immediately after Version.",
          "Validated GlobalId against the protocol device range 1 through 30."
        ]
      },
      {
        "file": "src/core/config/runtime_config.hpp",
        "changes": [
          "Removed the device-specific LocalDeviceId constant.",
          "Removed the now-unneeded message protocol dependency."
        ]
      },
      {
        "file": "src/main.cpp",
        "changes": [
          "Made the firmware composition root depend on device_config.hpp.",
          "Passed dan::config::GlobalId to System initialization."
        ]
      },
      {
        "file": "src/core/system/system.hpp and src/core/system/system.cpp",
        "changes": [
          "Accepted GlobalId explicitly during initialization.",
          "Forwarded GlobalId to Runtime."
        ]
      },
      {
        "file": "src/core/runtime/runtime.hpp and src/core/runtime/runtime.cpp",
        "changes": [
          "Accepted GlobalId explicitly during initialization.",
          "Forwarded GlobalId to MessageDispatcher.",
          "Removed the dependency on device-specific runtime configuration."
        ]
      },
      {
        "file": "src/core/diagnostics/routing_device_test.cpp",
        "changes": [
          "Removed the dependency on LocalDeviceId.",
          "Selected a remote test ID relative to the initialized dispatcher local ID."
        ]
      },
      {
        "file": "src/modules/test/test_module.cpp",
        "changes": [
          "Used the initialized dispatcher local ID as the message global receiver."
        ]
      }
    ]
  },
  "frozen_decisions": [
    "The device configuration stores Version followed immediately by GlobalId.",
    "The firmware composition root passes the device identity into the core.",
    "The DAN core does not own the identity of a concrete device.",
    "GlobalId is a compile-time constant and must be in the protocol range 1 through 30.",
    "Application module settings belong to their respective modules.",
    "Logic is an independently developed application module.",
    "The core provides lifecycle management and message delivery between modules and an initialized network transport.",
    "A central ConfigurationTable for application module settings is not part of the architecture.",
    "Reserved ModuleID Configuration 0x06 remains reserved without requiring an active Configuration module."
  ],
  "verification": {
    "passed": [
      "message_core_test",
      "empty Runtime lifecycle host smoke test",
      "host compilation with -Wall -Wextra -Werror",
      "GlobalId compile-time range assertion",
      "git diff --check",
      "PlatformIO nanoatmega328 build",
      "firmware upload to a physical ATmega328P",
      "core startup on the physical target"
    ],
    "target_verification": {
      "target": "ATmega328P",
      "reported_by": "user",
      "reported_at": "2026-07-30",
      "result": "built_uploaded_and_working"
    }
  },
  "explicitly_excluded": [
    "application module settings",
    "central configuration table",
    "EEPROM storage",
    "runtime GlobalId mutation",
    "loading GlobalId from external storage",
    "network transport implementation",
    "new addressable runtime modules",
    "uncommitted diagnostics Flash-string changes"
  ],
  "assistant_resume_instruction": "Restore this checkpoint and continue brick by brick. Device identity is defined in src/config/device_config.hpp as Version followed by GlobalId and is passed explicitly through System and Runtime to MessageDispatcher. Keep application settings inside their modules. Do not introduce a central ConfigurationTable for module settings. Preserve unrelated uncommitted diagnostics Flash-string changes outside this brick."
}
