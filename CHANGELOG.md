# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### ✨ Added

### 🛠️ Changed

### 🐞 Fixed

---

## [0.3.0] - 2025-08-24 - The "Connectivity & Scalability" Release

This major release introduces foundational features for building complex, scalable, and connected IoT devices. It adds support for multiple module instances from a single configuration and lays the groundwork for robust device interaction with new core services.

### ✨ Added

* **🏗️ Core Architecture - Multiple Module Instances (Scalability):**
  * The configuration system now supports defining an **array of module instances** in `config.json` files, allowing multiple devices of the same type (e.g., several relays or sensors).
  * Introduced a mandatory `instance_name` field in the configuration for unique module identification.
  * `BaseModule` now accepts and stores `instanceName` for use in logging, services, and commands.
* **🔌 New Core Service - `DeviceIdentity`:**
  * Implemented a multi-platform service to provide a consistent and unique device ID.
  * Generates ID from MAC address on ESP32/ESP8266 platforms.
  * Generates and stores a persistent, pseudo-random ID in EEPROM on AVR platforms.
* **📡 New Core Service - `CommandRouter`:**
  * Implemented a central service for registering and executing text-based commands (`<instance_name> <command> [args]`).
  * Added a new `registerCommands()` lifecycle method to `BaseModule`, allowing each module to expose its own command set.
* **🛡️ Thread-Safe Logger:**
  * The `Logger` is now thread-safe on ESP32, using a FreeRTOS mutex to prevent corrupted output from concurrent tasks (like MQTT callbacks), resolving garbled log tags.

### 🛠️ Changed

* **🐍 Build System:** The Python build script (`bootstrap.py`) was refactored into a modular package (`nextino_scripts`) and updated to correctly aggregate arrays of module configurations.
* **🔩 Core API:** The `SystemManager` and `ModuleFactory` APIs have been updated to handle the new `instance_name` during the module creation lifecycle.
* **🏷️ Service & Resource Naming:** The unique `instance_name` is now used for resource locking in `ResourceManager` and for creating unique, predictable service names in `ServiceLocator`.

---

## [0.2.0] - 2025-08-24 - The "Architecture Solidified" Release

This release focused on solidifying the core architecture, introducing powerful communication patterns, and significantly improving the framework's stability and developer experience. All core components are now more robust and consistent.

### ✨ Added

* **🚀 Core Communication Patterns:**
  * `EventBus`: Implemented a full publish-subscribe system.
  * `ServiceLocator`: Implemented a service locator pattern.
* **🛡️ Expanded ResourceManager:**
  * Added support for locking all major resource types: `I2C`, `SPI`, `UART`, `ADC`, and `DAC`.
* **⏱️ Advanced Scheduler Control:**
  * Added `cancel()` method and `TaskHandle` return values.

### 🛠️ Changed

* **🏗️ Architectural Refactoring:**
  * Standardized all singletons to use the Meyers' Singleton pattern.
  * `ModuleFactory` now uses the central `Logger`.
* **🎨 Improved Logger:**
  * Refactored color output to use predefined macros.

### 🐞 Fixed

* **🐛 Module Logic:** Corrected critical state management and timing bugs in example modules.
* **🔌 Serial Monitor Race Condition:** Addressed the "empty serial monitor" issue via documentation.

---

## [0.1.0] - 2025-08-19 - The "Birthday" Release

This is the very first public release of the Nextino Framework.
... (დანარჩენი `v0.1.0` ტექსტი უცვლელია)

[Unreleased]: https://github.com/magradze/Nextino/compare/v0.3.0...HEAD
[0.3.0]: https://github.com/magradze/Nextino/compare/v0.2.0...v0.3.0
[0.2.0]: https://github.com/magradze/Nextino/compare/v0.1.0...v0.2.0
[0.1.0]: https://github.com/magradze/Nextino/releases/tag/v0.1.0
