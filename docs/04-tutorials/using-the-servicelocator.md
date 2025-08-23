---
sidebar_position: 4 
title: 'Using the ServiceLocator'
---

# 📞 Using the ServiceLocator for Direct Control

While the `EventBus` is perfect for broadcasting notifications (one-to-many), sometimes one module needs to **directly command** another (one-to-one). For this, Nextino provides the **`ServiceLocator`** pattern.

This pattern allows a module to "provide" itself as a named service. Any other module can then "get" a pointer to this service and call its public methods directly. It's a powerful way to achieve **one-to-one communication** while still keeping the modules decoupled.

---

## 🎯 The Goal

We will build a system where:

1. Our `LedModule` registers itself as a service named `"led_service"`.
2. The service's API logic (`turnOn`, `turnOff`) will live in its own dedicated `LedModule_api.cpp` file.
3. We will create a new, simple `ControlModule` that finds the `"led_service"` and commands it to blink, demonstrating direct control.

## Step 1: Make `LedModule` a "Service Provider"

First, we need to update our `LedModule` to expose a public API and register itself as a service. We will follow our best practice of separating the API logic into its own file.

### 1.1. Update the File Structure

Let's add a new file to our `LedFlasher` library for the API implementation.

```plaintext
lib/LedFlasher/
└── src/
    ├── LedModule.h
    ├── LedModule.cpp         // Constructor & Lifecycle
    ├── LedModule_events.cpp  // Event handling logic
    └── LedModule_api.cpp     // ✨ NEW: Service API logic
```

### 1.2. Update the Header (`LedModule.h`)

Add the `turnOn()` and `turnOff()` methods to the public interface. The header file defines the "contract" for what the service can do.

```cpp title="lib/LedFlasher/src/LedModule.h"
#pragma once
#include <Nextino.h>

class LedModule : public BaseModule {
private:
    // Define clear states for the module
    enum class LedState {
        OFF,
        ON,
        BLINKING
    };

    // Configuration
    int _pin;
    unsigned long _interval;

    // Internal State
    uint32_t _taskHandle;
    LedState _currentState;

    // --- Event Handlers ---
    void handleShortPress(void* payload);
    void handleLongPress(void* payload);

    // Private method to change state cleanly
    void setState(LedState newState);

public:
    LedModule(const JsonObject& config);
    
    static BaseModule* create(const JsonObject& config) {
        return new LedModule(config);
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

### 1.3. Update the Main Implementation (`LedModule.cpp`)

The main `.cpp` file is the "orchestrator". Its `init()` method is the perfect place to register the service with the `ServiceLocator`.

```cpp title="lib/LedFlasher/src/LedModule.cpp"
#include "LedModule.h"

// ... (Constructor and getName() remain the same)

void LedModule::init() {
    pinMode(_pin, OUTPUT);
    digitalWrite(_pin, LOW);
    
    // ✨ Register this instance as a service!
    NextinoServices().provide("led_service", this);

    NEXTINO_LOGI(getName(), "Initialized on pin %d and provided 'led_service'.", _pin);
}

// ... (start() method remains the same)
```

### 1.4. Create the API Implementation (`LedModule_api.cpp`)

This new file will contain the actual logic for the public API methods. This keeps our main `.cpp` file clean and focused on the module's lifecycle.

```cpp title="lib/LedFlasher/src/LedModule_api.cpp"
#include "LedModule.h"

// --- Public API Implementation ---

void LedModule::turnOn() {
    // For simplicity, this just turns the LED on.
    // In a real module, this would interact with the state machine.
    digitalWrite(_pin, HIGH);
    NEXTINO_LOGI(getName(), "Turned ON via API call.");
}

void LedModule::turnOff() {
    digitalWrite(_pin, LOW);
    NEXTINO_LOGI(getName(), "Turned OFF via API call.");
}
```

Our `LedModule` is now a perfectly structured service provider! 🏆

## Step 2: Create the `ControlModule` (Service Consumer)

Now, let's create a new module that will find and use our service. The code for this module remains the same as before, as it only interacts with the public `LedModule.h` interface and doesn't care how the implementation is structured. This is the beauty of encapsulation!

Create a new library in your `lib` folder named `Controller`.

### 2.1. `config.json` and `library.json`

```json title="lib/Controller/config.json"
{
  "type": "ControlModule",
  "config": {}
}
```

```json title="lib/Controller/library.json"
{
    "name": "Controller",
    "version": "1.0.0",
    "description": "A module to demonstrate the ServiceLocator.",
    "keywords": "nextino-module, controller, example",
    "dependencies": { "Nextino": "*" }
}
```

### 2.2. The Code (`ControlModule.h` & `ControlModule.cpp`)

```cpp title="lib/Controller/src/ControlModule.h"
#pragma once
#include <Nextino.h>
#include "LedModule.h" // ⚠️ See note below!

class ControlModule : public BaseModule {
private:
    LedModule* _led; // A pointer to hold the located service

public:
    ControlModule(const JsonObject& config);
    
    static BaseModule* create(const JsonObject& config) {
        return new ControlModule(config);
    }

    const char* getName() const override;
    void init() override;
    void start() override;
};
```

:::danger ⚠️ A Note on Including Headers
Wait, didn't we say "modules must never directly include each other's header files"? Yes!

In a **perfectly decoupled system**, the `ControlModule` would not know about the `LedModule` class. It would ask the `ServiceLocator` for a generic `ILedService` interface.

However, for simplicity in this tutorial, we are including the concrete `LedModule.h` header. This creates a **one-way dependency** (`ControlModule` depends on `LedModule`, but not vice-versa). This is an acceptable trade-off for simpler projects but should be avoided in large, complex applications by using abstract interfaces.
:::

```cpp title="lib/Controller/src/ControlModule.cpp"
#include "ControlModule.h"

ControlModule::ControlModule(const JsonObject& config) {
    _led = nullptr; // Always initialize pointers to nullptr!
}

const char* ControlModule::getName() const { return "ControlModule"; }

void ControlModule::init() {
    // In init(), we find the service.
    // We do this here to ensure the service is available before we try to use it in start().
    _led = NextinoServices().get<LedModule>("led_service");

    if (_led) {
        NEXTINO_LOGI(getName(), "Successfully located 'led_service'.");
    } else {
        NEXTINO_LOGE(getName(), "Failed to locate 'led_service'! The module may not work correctly.");
    }
}

void ControlModule::start() {
    // Now that we have the service, let's use it!
    if (_led) {
        NEXTINO_LOGI(getName(), "Commanding LED to blink a pattern.");
        // A quick blink pattern to show we have control
        NextinoScheduler().scheduleOnce(500, [this]() { _led->turnOn(); });
        NextinoScheduler().scheduleOnce(700, [this]() { _led->turnOff(); });
        NextinoScheduler().scheduleOnce(900, [this]() { _led->turnOn(); });
        NextinoScheduler().scheduleOnce(1100, [this]() { _led->turnOff(); });
    }
}
```

## Step 3: Run and See the Result

1. Add the new `Controller` library to your `platformio.ini`.
2. Upload the code.

When the system boots, you will see in the logs that the `ControlModule` finds the `led_service` and then, after a short delay, the LED will blink twice, commanded directly by the `ControlModule`.

You have now mastered both of Nextino's powerful communication patterns while following best practices for code organization! 🎉

---

### Next Steps

You have learned all the core concepts of Nextino. Now it's time to explore the API reference and start building your own amazing projects!

➡️ **[API Reference (Coming Soon)](/api)**
