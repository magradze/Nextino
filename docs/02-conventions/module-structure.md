---
sidebar_position: 3
title: 'Module Structure'
---

# 📦 Module Structure Convention

A consistent and predictable file structure is the backbone of a scalable framework. This document defines the **mandatory** file and directory structure for all Nextino modules. Following this convention ensures that the framework's auto-discovery scripts work correctly and that new developers can easily navigate the codebase.

---

## 📂 The Anatomy of a Nextino Module

Every Nextino module is a self-contained PlatformIO library. It must reside in the project's `lib/` directory and follow the structure below. This structure promotes a clean separation of concerns.

```plaintext
lib/
└── MyAwesomeModule/
    ├── src/
    │   ├── MyAwesomeModule.h           // 📜 The Public Contract (Header)
    │   ├── MyAwesomeModule.cpp         // 🏗️ Lifecycle & Constructor
    │   ├── MyAwesomeModule_events.cpp  // 🧠 Event Handling Logic
    │   └── MyAwesomeModule_cmd.cpp     // ⌨️ Command Router Logic
    │
    ├── config.json                     // ⚙️ Default Configuration (Always an array!)
    ├── library.json                    // 📦 The Module's "Passport"
    ├── schema.json                     // 📜 Configuration rules for validation
    └── README.md                       // 📖 User Manual
```

### 📜 `src/MyAwesomeModule.h` - The Public Contract

This header file defines the module's class and its public interface. This is the only file that another module *might* need to include (though direct dependencies are discouraged).

* **Must** contain the class definition, inheriting from `BaseModule`.
* **Must** declare all public methods, lifecycle overrides, and the static `create` factory method.
* **Should** declare all private methods and member variables.

### 🏗️ `src/MyAwesomeModule.cpp` - The Lifecycle Orchestrator

This file is the module's "main entry point" for the framework. It should be clean and simple, focusing only on the module's lifecycle.

* **Must** implement the constructor (`MyAwesomeModule(...)`), passing the `instanceName` to the `BaseModule` parent.
* **Must** implement the core lifecycle methods: `getName()`, `init()`, `start()`, and `loop()`.
* Its job is to handle the "what" and "when" (lifecycle), delegating the "how" (the actual logic) to other specialized files.

### 🧠 `src/MyAwesomeModule_events.cpp` - The Event Logic

This is where the module's reactive logic lives.

* **Should** implement the handler methods for events the module subscribes to via the `EventBus`.
* This separation keeps the main `.cpp` file uncluttered and focused on the framework contract.

### ⌨️ `src/MyAwesomeModule_cmd.cpp` - The Command Logic

This file connects your module to the `CommandRouter`, making it controllable via text-based commands.

* **Must** implement the `registerCommands()` lifecycle method.
* Inside this method, it calls `NextinoCommands().registerCommand()` to expose the module's public functions as commands.

:::tip Expanding Your Module! 🚀
For more complex modules, you can add even more specialized files:

* `MyAwesomeModule_api.cpp`: For implementing a `ServiceLocator` public API.
* `MyAwesomeModule_hal.cpp`: For low-level, hardware-specific logic.
:::

### ⚙️ `config.json` - The Default Blueprint

This file defines the module's default instance(s) and their configuration.

* **Must** be a valid JSON file.
* The root element **must** be a **JSON Array `[]`**, even for a single instance.
* Each object in the array **must** contain `type` and `instance_name` keys.

### 📦 `library.json` - The Module's Passport

This is the standard PlatformIO manifest file, with one special requirement for Nextino.

* **Must** include `"nextino-module"` in the `keywords` array or string. This is the "magic word" 🧙 our `bootstrap.py` script uses to discover the module.

### 📖 `README.md` - The User Manual

Every module should have a simple `README.md` that explains:

* What the module does.
* All available parameters in its `config.json`.
* A simple usage example.

---

This clean, logical, and fully automated structure ensures that any module you create will seamlessly integrate into any Nextino project.

---

### Next Steps

With the code and file structure defined, let's establish the rules for documenting our code.

➡️ **[Documentation Style](./documentation-style)**
