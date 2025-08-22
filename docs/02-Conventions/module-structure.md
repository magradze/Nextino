---
sidebar_position: 3
title: 'Module Structure'
---

# 📦 Module Structure Convention

A consistent and predictable file structure is the backbone of a scalable framework. This document defines the **mandatory** file and directory structure for all Nextino modules. Following this convention ensures that the framework's auto-discovery scripts work correctly and that new developers can easily navigate the codebase.

---

## 📂 The Anatomy of a Nextino Module

Every Nextino module is a self-contained PlatformIO library. It must reside in the project's `lib/` directory and follow the structure below.

```plaintext
lib/
└── MyAwesomeModule/
    ├── src/
    │   ├── MyAwesomeModule.h         // 📜 The Public Contract (Header)
    │   ├── MyAwesomeModule.cpp         // 🏗️ Lifecycle & Constructor
    │   └── MyAwesomeModule_events.cpp  // 🧠 Core Logic & Event Handling
    │
    ├── config.json                     // ⚙️ Default Configuration
    ├── library.json                    // 📦 The Module's "Passport"
    └── README.md                       // 📖 User Manual
```

### 📜 `src/MyAwesomeModule.h` - The Public Contract

This header file defines the module's class and its interface. While it is a public header within the library, it is **not meant to be included directly** by the user's `main.cpp`. Instead, our `bootstrap.py` script discovers this header automatically and includes it in the auto-generated `generated_config.h` file.

* **Must** contain the class definition, inheriting from `BaseModule`.
* **Must** declare all public methods and the static `create` factory method.
* **Should** declare private methods and member variables.

### 🏗️ `src/MyAwesomeModule.cpp` - The Lifecycle Orchestrator

This file is responsible for the module's integration with the framework.

* **Must** implement the constructor (`MyAwesomeModule(...)`).
* **Must** implement the core lifecycle methods: `getName()`, `init()`, `start()`, and `loop()`.
* Its primary job is to handle the "what" and "when" (lifecycle), delegating the "how" (the actual logic) to other files.

### 🧠 `src/MyAwesomeModule_events.cpp` - The Core Logic

This is where the real work happens!

* **Must** implement the module's private methods, especially those related to event handling or complex logic.
* This separation keeps the main `.cpp` file clean and focused on the framework contract.

:::tip Expanding Your Module!
For more complex modules, you can add more specialized files:

* `MyAwesomeModule_api.cpp`: For implementing a `ServiceLocator` API.
* `MyAwesomeModule_hal.cpp`: For hardware-specific logic.
:::

### ⚙️ `config.json` - The Default Blueprint

This file defines the module's default configuration.

* **Must** be a valid JSON file.
* **Must** contain a `type` key that exactly matches the module's class name (e.g., `"MyAwesomeModule"`).
* **Must** contain a `config` object with the module's parameters.
* **Should** include a `resource` object if the module requires exclusive hardware access.

### 📦 `library.json` - The Module's Passport

This is the standard PlatformIO manifest file, with one special requirement for Nextino.

* **Must** include `"nextino-module"` in the `keywords` string. This is the "magic word" that our `bootstrap.py` script uses to discover the module.

### 📖 `README.md` - The User Manual

Every module should have a simple `README.md` that explains:

* What the module does.
* What parameters are available in its `config.json`.
* A simple usage example.

---

## 🗺️ Example: A Complete `ButtonReader` Module

```plaintext
lib/ButtonReader/
├── src/
│   ├── ButtonModule.h
│   ├── ButtonModule.cpp
│   └── ButtonModule_events.cpp
├── config.json
├── library.json
└── README.md
```

This structure is clean, logical, and fully automated. By following it, you ensure your module seamlessly integrates into any Nextino project.

---

### Next Steps

With the code and file structure defined, let's establish the rules for documenting our code.

➡️ **[Documentation Style](./documentation-style)**
