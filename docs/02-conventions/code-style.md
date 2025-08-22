---
sidebar_position: 1
title: 'Code Style Guide'
---

# ✍️ Code Style Guide

This document defines the official C++ coding style for the Nextino framework. Adhering to a consistent style is crucial for maintaining code readability, reducing errors, and simplifying collaboration.

**The rules in this document are mandatory.** All contributions must follow this style guide.

---

## 1. Naming Conventions

* **Classes and Structs:** Use `PascalCase` (e.g., `SystemManager`, `LedModule`).
* **Methods and Functions:** Use `camelCase` (e.g., `registerModule`, `scheduleRecurring`).
* **Variables and Parameters:** Use `camelCase` (e.g., `ledPin`, `blinkInterval`).
* **Private Member Variables:** Use a `_` prefix followed by `camelCase` (e.g., `_ledPin`, `_isBlinking`).
* **Constants and Enums:** Use `ALL_CAPS_SNAKE_CASE` (e.g., `LED_PIN`, `LOG_LEVEL_INFO`).
* **Macros:** Use `ALL_CAPS_SNAKE_CASE` (e.g., `NEXTINO_LOGI`).

## 2. Formatting

* **Braces (`{}`):** Use the "Allman" style, where braces are on their own line.

    ```cpp
    // Correct
    class MyClass
    {
    public:
        void myFunction()
        {
            if (condition)
            {
                // ...
            }
        }
    };

    // Incorrect
    class MyClass {
        void myFunction() {
            if (condition) {
                // ...
            }
        }
    };
    ```

* **Indentation:** Use **4 spaces** for indentation. Do not use tabs.
* **Line Length:** Aim for a maximum line length of 120 characters.

## 3. Comments

* **Doxygen for Public APIs:** All public classes, methods, and enums in header files (`.h`) **must** be documented using Doxygen-style comments (`/** ... */`).
* **Implementation Comments:** Use `//` for single-line comments and `/* ... */` for multi-line comments to explain complex logic within `.cpp` files.

## 4. Class and Struct Design

* **Header Guards:** All header files must use `#pragma once`.
* **Declaration Order:** Within a class, the recommended order is `public`, then `protected`, then `private`.
* **Single Responsibility:** Each class should have a single, well-defined responsibility.

## 5. Pointers and References

* **Prefer References:** When a function needs to modify an object and `nullptr` is not a valid value, prefer passing by reference (`&`) over passing a pointer (`*`).
* **Pointer Style:** Place the asterisk (`*`) next to the type, not the variable name.

    ```cpp
    // Correct
    BaseModule* myModule;

    // Incorrect
    BaseModule *myModule;
    ```

---

### Next Steps

Now that we've established the basic style, let's define the specific naming conventions for different parts of the framework.

➡️ **[Naming Conventions](./naming-conventions)**
