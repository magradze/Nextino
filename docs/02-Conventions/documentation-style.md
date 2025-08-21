---
sidebar_position: 4
title: 'Documentation Style'
---

# ✍️ Documentation Style Guide

Good code explains *how* it works. Great documentation explains *why*.

This document defines the official style for writing comments and documentation for the Nextino framework. Our goal is to create a codebase that is not only functional but also a pleasure to read, understand, and contribute to.

---

## 📜 The Golden Rule: Document the "Why," Not the "How"

Your code should be clean enough to explain *how* a task is accomplished. Your comments should explain the **intent** and the **reasoning** behind the code.

* **Bad Comment (explains the "how"):**

    ```cpp
    // Increment i by 1
    i++; 
    ```

* **Good Comment (explains the "why"):**

    ```cpp
    // We need to process the next item in the buffer
    i++;
    ```

## 🏛️ Doxygen for Public APIs

All public-facing code—especially classes, methods, and enums in `.h` files—**must** be documented using Doxygen-style comments. This allows us to automatically generate a professional API reference in the future.

### File Headers

Every `.h` and `.cpp` file must begin with a standard file header.

```cpp
/**
 * @file        MyModule.h
 * @title       My Awesome Module
 * @description Defines the class and interface for MyAwesomeModule.
 *
 * @author      Your Name
 * @date        2025-08-19
 * @version     1.0.0
 *
 * @copyright   (c) 2025 Nextino. All rights reserved.
 * @license     MIT License
 */
```

### Class Documentation

Document the purpose of the class above its definition.

```cpp
/**
 * @class MyAwesomeModule
 * @brief A brief, one-sentence description of the class.
 * @details A more detailed explanation of the class's responsibilities,
 *          its role in the framework, and any important design notes.
 */
class MyAwesomeModule : public BaseModule {
    // ...
};
```

### Method Documentation

Document every public method in the header file.

```cpp
class MyAwesomeModule {
public:
    /**
     * @brief A brief, one-sentence description of what the method does.
     * @param param1 Description of the first parameter.
     * @param param2 Description of the second parameter.
     * @return Description of the return value.
     * @note (Optional) Any important notes about how to use this method.
     * @warning (Optional) Any critical warnings or potential side effects.
     */
    bool doSomething(int param1, const std::string& param2);
};
```

## 🧠 Implementation Comments (`.cpp` files)

Inside `.cpp` files, use comments to clarify complex, non-obvious, or tricky parts of the code.

* **Use `//` for single-line comments.**
* **Use `/* ... */` for multi-line comments.**

```cpp
void MyAwesomeModule::complexAlgorithm() {
    // We use a left bit-shift here for performance instead of multiplying by 2.
    // This is a critical optimization for the AVR platform.
    int optimizedValue = value << 1;

    /*
     * The following block of code handles a tricky edge case where the
     * sensor might return a transient error. We implement a simple
     * retry mechanism before failing completely.
     */
    for (int i = 0; i < 3; ++i) {
        // ...
    }
}
```

## 📖 `README.md` for Modules

Every module (as a library in the `lib/` folder) should have its own `README.md` file. It should be a mini-guide for that specific module, including:

1. **A brief description** of what the module does.
2. **`config.json` parameters:** A table explaining all available configuration options.
3. **A simple usage example** showing how to configure and use the module in a project.

---

By consistently applying these documentation standards, we create a framework that is not only powerful but also welcoming and easy to learn for developers of all skill levels. 💖
