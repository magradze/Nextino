---
sidebar_position: 2
title: 'Naming Conventions'
---

# 🏷️ Naming Conventions

This document provides specific and detailed naming conventions for all major entities within the Nextino framework. A consistent naming scheme is one of the most effective ways to make code self-documenting and easy to understand.

---

## 1. General Principles

* **Be Descriptive:** Names should clearly communicate the purpose of the entity. Avoid abbreviations that are not widely understood (e.g., use `blinkInterval` instead of `bInt`).
* **Be Consistent:** Follow the patterns outlined below across the entire codebase.
* **Language:** All names must be in **English**.

---

## 2. File Naming

* **Header Files:** Use `PascalCase` and match the primary class name defined within.
  * `LedModule.h`
  * `SystemManager.h`
* **Implementation Files:** Use `PascalCase` and match the corresponding header file.
  * `LedModule.cpp`
  * `SystemManager.cpp`
* **Logical Splits:** For complex modules, use a `_` suffix to denote responsibility.
  * `MyModule_events.cpp`
  * `MyModule_api.cpp`
* **Configuration Files:** Use `lowercase` and `.json` extension.
  * `config.json`
  * `library.json`
* **Scripts:** Use `snake_case`.
  * `bootstrap.py`

---

## 3. C++ Naming Conventions

### Classes, Structs, and Enums

* **Type Names:** Use `PascalCase`.
  * `class ModuleFactory`
  * `struct ScheduledTask`
  * `enum class LogLevel`
* **Abstract Interfaces (Future):** Use an `I` prefix.
  * `class IGpio`
  * `class II2cBus`

### Methods and Functions

* **Public Methods:** Use `camelCase`.
  * `void registerModule(BaseModule* module);`
  * `void scheduleRecurring(unsigned long intervalMs, TaskCallback callback);`
* **Private Methods:** Use `camelCase`. The distinction is made by placing them in the `private:` section of the class.
  * `void checkButton();`

### Variables

* **Local Variables & Parameters:** Use `camelCase`.
  * `int ledPin;`
  * `unsigned long lastDebounceTime;`
* **Private Member Variables:** Use a `_` prefix followed by `camelCase`. This clearly distinguishes member state from local variables.
  * `private: int _pin;`
  * `private: bool _isBlinking;`
* **Global Variables:** **Avoid global variables.** If absolutely necessary for inter-operation with C libraries, use a `g_` prefix (e.g., `g_myGlobalFlag`), but this should be an exception.
* **Static Variables:** Use an `s_` prefix for static variables that are not class members.
  * `static Logger* s_instance = nullptr;`

### Constants, Enums, and Macros

* **Constants:** Use `k` prefix followed by `PascalCase`.
  * `const int kMaxLogBufferSize = 256;`
* **Enum Members:** Use `PascalCase`.
  * `enum class LogLevel { None, Error, Warn, Info, Debug };`
* **Macros:** Use `ALL_CAPS_SNAKE_CASE`.
  * `#define NEXTINO_LOGI(tag, ...)`

---

## 4. Configuration (`config.json`) Naming

* **Keys:** Use `snake_case`. This is the standard convention for JSON and makes it distinct from the C++ code.
  * `"blink_interval_ms"`
  * `"debounce_delay"`
* **Resource Objects:** Use a standardized name for the primary hardware resource object.
  * `"resource": { "type": "gpio", "pin": 4 }`

---

### Quick Reference Table

| Entity Type               | Convention         | Example                               |
| ------------------------- | ------------------ | ------------------------------------- |
| Class / Struct / Enum     | `PascalCase`       | `class LedModule;`                    |
| Method / Function         | `camelCase`        | `void start();`                       |
| Local Variable / Parameter| `camelCase`        | `int pinNumber;`                      |
| Private Member Variable   | `_camelCase`       | `int _pin;`                           |
| Constant (`const`)        | `kPascalCase`      | `const int kMaxModules = 10;`         |
| Enum Member               | `PascalCase`       | `LogLevel::Debug`                     |
| Macro                     | `ALL_CAPS_SNAKE_CASE` | `#define MY_MACRO`                  |
| JSON Key                  | `snake_case`       | `"blink_interval_ms": 500`            |

---

### Next Steps

With a clear naming system, let's define the standard file and directory structure for modules.

➡️ **[Module Structure](./module-structure.md)**
