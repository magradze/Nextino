# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### ✨ Added

### 🛠️ Changed

### 🐞 Fixed

### 🗑️ Removed

---

## [0.2.0] - 2025-08-24 - The "Architecture Solidified" Release

This release focuses on solidifying the core architecture, introducing powerful communication patterns, and significantly improving the framework's stability and developer experience. All core components are now more robust and consistent.

### ✨ Added

* **🚀 Core Communication Patterns:**
  * `EventBus`: Implemented a full publish-subscribe system for decoupled, one-to-many module communication.
  * `ServiceLocator`: Implemented a service locator pattern for direct, yet decoupled, one-to-one module communication.
* **🛡️ Expanded ResourceManager:**
  * Added support for locking all major resource types: `I2C`, `SPI`, `UART`, `ADC`, and `DAC`.
  * `SystemManager` now automatically parses and locks these resources from `config.json`.
* **⏱️ Advanced Scheduler Control:**
  * `scheduleRecurring` and `scheduleOnce` now return a `TaskHandle`.
  * Added a `cancel()` method to the `Scheduler` to stop tasks dynamically.

### 🛠️ Changed

* **🏗️ Architectural Refactoring:**
  * **Singleton Pattern:** All core singletons (`Logger`, `SystemManager`, `ResourceManager`) now use the modern and thread-safe Meyers' Singleton pattern for consistency.
  * **Centralized Logging:** `ModuleFactory` now uses the central `Logger` instead of direct `Serial` calls, respecting log levels and formatting.
* **🎨 Improved Logger:**
  * Refactored the `Logger`'s color output to use predefined macros from `LogColors.h`, making it more maintainable.

### 🐞 Fixed

* **🐛 Module Logic:** Corrected critical state management and timing bugs in the `ButtonModule` and `LedModule` examples, ensuring reliable operation.
* **🔌 Serial Monitor Race Condition:** Addressed the "empty serial monitor" issue by providing clear documentation and troubleshooting steps for developers.

---

## [0.1.0] - 2025-08-19 - The "Birthday" Release

This is the very first public release of the Nextino Framework. It establishes the foundational architecture and introduces the core concepts of modular, configuration-driven development for the Arduino ecosystem.

This initial release is dedicated to my son on his birthday. May your curiosity and creativity build amazing things, just as we hope developers will with this framework.

### ✨ Added

* **Core Framework (`v0.1.0`):**
  * `SystemManager`: Central orchestrator for module lifecycle management.
  * `BaseModule`: The abstract contract for all modules.
  * `Scheduler`: A non-blocking, `millis()`-based task scheduler.
  * `Logger`: A professional, leveled, and colored logging system.
  * `ModuleFactory`: A factory for dynamically creating modules from configuration.
* **Build System:**
  * Introduced an advanced Python build script (`bootstrap.py`) for automatic module discovery and configuration generation.
* **Example Project:**
  * Added the `01_PlugAndPlay_Blink` example to demonstrate the core "Plug-and-Play" philosophy.
* **Documentation:**
  * Created initial project documentation including `README.md`, `LICENSE`, `CONTRIBUTING.md`, `MANIFESTO.md`, and `TESTING.md`.

### 🧪 Tests

* Set up the testing infrastructure with PlatformIO and Unity.
* Added initial unit tests for the `Scheduler` component.

[Unreleased]: https://github.com/magradze/Nextino/compare/v0.2.0...HEAD
[0.2.0]: https://github.com/magradze/Nextino/compare/v0.1.0...v0.2.0
[0.1.0]: https://github.com/magradze/Nextino/releases/tag/v0.1.0
