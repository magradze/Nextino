---
sidebar_position: 4
title: 'Using the ServiceLocator'
---

# 📞 Using the ServiceLocator for Direct Control

While the `EventBus` is perfect for broadcasting notifications (one-to-many), sometimes you need one module to **directly command a specific instance** of another module (one-to-one). This is where the **`ServiceLocator`** shines. 🎯

The `ServiceLocator` acts like a central phone book 📖 for your project. A module can "provide" itself as a uniquely named service, and any other module can then "get" that specific service to call its public methods.

This tutorial will demonstrate how to control one specific LED out of two, using its unique `instance_name`.

---

## 🎯 The Goal

We will build a system with multiple module instances:

1. **Two `LedModule` instances:** `status_led` and `error_led`, each with a public API (`turnOn`, `turnOff`). Each one will register itself as a unique service (e.g., `"LedModule:status_led"`).
2. **`ControlModule`**: A new module that, upon startup, will find **only the `error_led`** service and command it to blink an S.O.S. pattern, leaving the `status_led` untouched.

This demonstrates how to achieve precise, direct control in a multi-component system.

## Step 1: Make `LedModule` a "Service Provider"

First, we'll update our `LedModule` to expose a public API and, crucially, to register itself with a unique service name built from its `instance_name`.

### 1.1. Update the Header (`LedModule.h`)

We must first ensure our `LedModule` is compatible with the `instance_name` architecture.

```cpp title="lib/LedFlasher/src/LedModule.h"
#pragma once
#include <Nextino.h>
#include <string> // Required for std::string operations

class LedModule : public BaseModule {
private:
    int _pin;
    // ... other private members

public:
    // Constructor updated for multiple instances
    LedModule(const char* instanceName, const JsonObject& config);
    
    // Create function updated for multiple instances
    static BaseModule* create(const char* instanceName, const JsonObject& config) {
        return new LedModule(instanceName, config);
    }

    // --- BaseModule Lifecycle Methods ---
    const char* getName() const override;
    void init() override;
    void start() override;

    // --- ✨ Public API for ServiceLocator ---
    void turnOn();
    void turnOff();
};
```

### 1.2. Update the Main Implementation (`LedModule.cpp`)

The key change is in the `init()` method. Instead of a hard-coded service name, we will generate a unique one. The API methods (`turnOn`/`turnOff`) can be placed in a separate `LedModule_api.cpp` for better organization.

```cpp title="lib/LedFlasher/src/LedModule.cpp"
#include "LedModule.h"

// Constructor updated to call the parent
LedModule::LedModule(const char* instanceName, const JsonObject& config)
    : BaseModule(instanceName) {
    _pin = config["resource"]["pin"];
    // ... other initializations
}

const char* LedModule::getName() const { return "LedModule"; }

void LedModule::init() {
    pinMode(_pin, OUTPUT);
    
    // ✨ Create and provide a UNIQUE service name ✨
    // e.g., for an instance_name "error_led", the service will be "LedModule:error_led"
    std::string serviceName = std::string(getName()) + ":" + getInstanceName();
    NextinoServices().provide(serviceName.c_str(), this);

    // Use getInstanceName() for unique logs
    NEXTINO_LOGI(getInstanceName(), "Initialized on pin %d and provided service '%s'.", _pin, serviceName.c_str());
}

// ... (start() and other methods)
```

`(Implementation for turnOn/turnOff goes here or in a separate _api.cpp file)`

Our `LedModule` is now a fully-fledged, instance-aware service provider! 🏆

## Step 2: Configure Multiple LEDs

Let's tell our project we want two LEDs. We do this in a `config.json` file inside a library (e.g., `lib/ProjectHardware`).

```json title="lib/ProjectHardware/config.json"
[
  {
    "type": "LedModule",
    "instance_name": "status_led",
    "config": {
      "resource": { "type": "gpio", "pin": 2 }
    }
  },
  {
    "type": "LedModule",
    "instance_name": "error_led",
    "config": {
      "resource": { "type": "gpio", "pin": 4 }
    }
  }
]
```

## Step 3: Create the `ControlModule` (Service Consumer)

Now, let's create a module that will find and use **only the `error_led`**. Create a new library `lib/Controller`.

### 3.1. `config.json` and `library.json`

```json title="lib/Controller/config.json"
[
  {
    "type": "ControlModule",
    "instance_name": "main_controller"
  }
]
```

_(The manifest `library.json` is standard)_

### 3.2. The Code (`ControlModule.h` & `ControlModule.cpp`)

```cpp title="lib/Controller/src/ControlModule.h"
#pragma once
#include <Nextino.h>
#include "LedModule.h" // Note: See previous tutorial for dependency explanation

class ControlModule : public BaseModule {
private:
    LedModule* _errorLed; // A pointer to hold the specific LED service

public:
    ControlModule(const char* instanceName, const JsonObject& config);
    
    static BaseModule* create(const char* instanceName, const JsonObject& config) {
        return new ControlModule(instanceName, config);
    }

    const char* getName() const override;
    void init() override;
    void start() override;
};
```

```cpp title="lib/Controller/src/ControlModule.cpp"
#include "ControlModule.h"

ControlModule::ControlModule(const char* instanceName, const JsonObject& config)
    : BaseModule(instanceName) {
    _errorLed = nullptr; // Always initialize pointers!
}

const char* ControlModule::getName() const { return "ControlModule"; }

void ControlModule::init() {
    // We look for a VERY SPECIFIC service by its unique composite name
    _errorLed = NextinoServices().get<LedModule>("LedModule:error_led");

    if (_errorLed) {
        NEXTINO_LOGI(getInstanceName(), "Successfully located 'LedModule:error_led'.");
    } else {
        NEXTINO_LOGE(getInstanceName(), "Failed to locate 'LedModule:error_led'!");
    }
}

void ControlModule::start() {
    if (_errorLed) {
        NEXTINO_LOGI(getInstanceName(), "Commanding error_led to blink S.O.S.");
        // S.O.S: 3 short, 3 long, 3 short
        // For simplicity, we'll just blink it fast.
        _errorLed->turnOn(); // We can now call its public methods directly!
        NextinoScheduler().scheduleOnce(100, [this]() { _errorLed->turnOff(); });
        NextinoScheduler().scheduleOnce(200, [this]() { _errorLed->turnOn(); });
        NextinoScheduler().scheduleOnce(300, [this]() { _errorLed->turnOff(); });
    }
}
```

## Step 4: Run and See the Result 🔬

1. Add all necessary libraries (`LedFlasher`, `Controller`, etc.) to your `platformio.ini`.
2. Upload the code.

When the system boots, you will see the `status_led` remain off. The `ControlModule` will find only the `error_led` service and cause it to blink rapidly, demonstrating precise, instance-based control.

You have now mastered both of Nextino's powerful communication patterns! 🎉

---

### Next Steps

Congratulations! 🎉 You are now equipped with the core knowledge to build amazing, structured, and scalable embedded applications with Nextino. Happy coding! ❤️
