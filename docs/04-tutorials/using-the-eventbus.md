---
sidebar_position: 3
title: 'Using the EventBus'
---

# 📡 Using the EventBus for Module Communication

You've created a self-contained module, but the true power of a modular framework is unlocked when modules can communicate. In Nextino, the primary way to achieve **decoupled, one-to-many** communication is through the **`EventBus`**. 📢

This tutorial builds upon the `ButtonModule` we created previously. We will modify our `LedModule` to listen for events and react to them, following our best practice of separating logic into dedicated files.

---

## 🎯 The Goal

We will create a fun, interactive system where:

1. The `ButtonModule` detects short and long presses, posting `button_short_press` and `button_long_press` events.
2. The `LedModule` subscribes to both of these events.
3. The event handling logic will live in its own `LedModule_events.cpp` file.
4. A **short press** will toggle the LED's blinking state.
5. A **long press** will toggle the LED's solid state.

This creates a sophisticated, well-structured system where two modules collaborate without any direct knowledge of each other. ✨

## Step 1: Update the `LedModule` to be Event-Aware

First, let's give our `LedModule` a proper state machine and prepare it for event subscriptions.

### 1.1. Update the File Structure

Let's add a new file to our `LedFlasher` library for the event handling logic.

```plaintext
lib/LedFlasher/
└── src/
    ├── LedModule.h
    ├── LedModule.cpp         // Constructor & Lifecycle
    └── LedModule_events.cpp  // ✨ NEW: Event handling logic
```

### 1.2. Update the Header (`LedModule.h`)

The header file defines the module's "contract," including the private methods that will handle the events.

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

    // --- Method Declarations ---
    // These will be implemented in LedModule_events.cpp
    void handleShortPress(void* payload);
    void handleLongPress(void* payload);

    // This will also be in LedModule_events.cpp as it's part of the core logic
    void setState(LedState newState);

public:
    LedModule(const JsonObject& config);
    
    static BaseModule* create(const JsonObject& config) {
        return new LedModule(config);
    }

    // --- BaseModule Lifecycle Methods (implemented in LedModule.cpp) ---
    const char* getName() const override;
    void init() override;
    void start() override;
};
```

### 1.3. Update the Main Implementation (`LedModule.cpp`)

The main `.cpp` file is the "orchestrator." Its job is to handle the lifecycle and set up the subscriptions. The actual logic is delegated to the methods implemented in `LedModule_events.cpp`.

```cpp title="lib/LedFlasher/src/LedModule.cpp"
#include "LedModule.h"

// --- Constructor and Lifecycle ---

LedModule::LedModule(const JsonObject& config) {
    _pin = config["resource"]["pin"];
    _interval = config["blink_interval_ms"] | 500;
    _taskHandle = 0;
    _currentState = LedState::OFF;
}

const char* LedModule::getName() const { return "LedModule"; }

void LedModule::init() {
    pinMode(_pin, OUTPUT);
    digitalWrite(_pin, LOW);
    NEXTINO_LOGI(getName(), "Initialized on pin %d.", _pin);
}

void LedModule::start() {
    // The start() method's only job is to set up subscriptions.
    // The logic itself is in another file.
    NextinoEvent().on("button_short_press", [this](void* p) { this->handleShortPress(p); });
    NextinoEvent().on("button_long_press", [this](void* p) { this->handleLongPress(p); });

    NEXTINO_LOGI(getName(), "Subscribed to button events.");
}
```

### 1.4. Create the Event Logic File (`LedModule_events.cpp`)

This new file contains the "brains" of the module—the state machine and the event handlers.

```cpp title="lib/LedFlasher/src/LedModule_events.cpp"
#include "LedModule.h"

// --- Event Handlers ---

void LedModule::handleShortPress(void* payload) {
    NEXTINO_LOGI(getName(), "Short press event received!");
    // Short press toggles between BLINKING and OFF
    if (_currentState == LedState::BLINKING) {
        setState(LedState::OFF);
    } else {
        setState(LedState::BLINKING);
    }
}

void LedModule::handleLongPress(void* payload) {
    NEXTINO_LOGI(getName(), "Long press event received!");
    // Long press toggles between ON and OFF
    if (_currentState == LedState::ON) {
        setState(LedState::OFF);
    } else {
        setState(LedState::ON);
    }
}

// --- Private State Machine ---

void LedModule::setState(LedState newState) {
    if (_currentState == newState) return; // No change

    _currentState = newState;
    NEXTINO_LOGI(getName(), "Changing state to %d", (int)_currentState);

    // First, always clean up the previous state (cancel timers)
    if (_taskHandle != 0) {
        NextinoScheduler().cancel(_taskHandle);
        _taskHandle = 0;
    }

    // Then, apply the new state
    switch (_currentState) {
        case LedState::OFF:
            digitalWrite(_pin, LOW);
            break;
        case LedState::ON:
            digitalWrite(_pin, HIGH);
            break;
        case LedState::BLINKING:
            _taskHandle = NextinoScheduler().scheduleRecurring(_interval, [this]() {
                digitalWrite(_pin, !digitalRead(_pin));
            });
            break;
    }
}
```

## Step 2: Put It All Together

The process to get your project running is the same as before.

1. **Create the `ButtonReader` module** in your `lib` folder as described in the [previous tutorial](/tutorials/creating-a-custom-module).
2. **Update `platformio.ini`** to include both modules as dependencies.

    ```ini title="platformio.ini"
    lib_deps =
        https://github.com/magradze/Nextino.git
        lib/LedFlasher
        lib/ButtonReader
    ```

3. **The `bootstrap.py` script will do the rest!** Your `main.cpp` **does not need to change at all**. That's the power of plug-and-play! 🔌

## Step 3: Run and Test 🧪

Upload the code to your board and open the Serial Monitor.

* **Press the button briefly:** The LED should start blinking. Press it briefly again, and it should stop.
* **Press and hold the button:** The LED should turn on and stay on. Press and hold it again, and it should turn off.

You have now successfully created a system where two modules communicate via events, following best practices for code organization.

---

### Next Steps

Now that you've mastered event-based communication, let's look at how modules can request services from each other directly using the `ServiceLocator`.

➡️ **[Using the ServiceLocator](/tutorials/using-the-servicelocator)**
