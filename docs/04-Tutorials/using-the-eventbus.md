---
sidebar_position: 2
title: 'Using the EventBus'
---

# 📡 Using the EventBus for Module Communication

You've created a module that can act on its own, but the true power of a modular framework is unlocked when modules can communicate. In Nextino, the primary way to achieve this is through the **`EventBus`**.

This tutorial builds upon the previous one. We will modify our `LedModule` to listen for the `"button_pressed"` event published by our `ButtonModule`, following our best practice of separating logic into different files.

---

## 🎯 The Goal

We will create a system where:

1. The `ButtonModule` detects a press and posts a `"button_pressed"` event.
2. The `LedModule` subscribes to this event.
3. When the event is received, the `LedModule` will toggle its blinking state (on/off).

This creates an interactive system where two modules collaborate without having any direct knowledge of each other.

## Step 1: Update the `LedModule` to be Event-Aware

First, we need to give our `LedModule` the ability to subscribe to events and a state to toggle.

### 1.1. Update the Header (`LedModule.h`)

Let's add a state variable, a task handle, and declare the new event handler method.

```cpp title="lib/LedFlasher/src/LedModule.h"
#pragma once
#include <Nextino.h>

class LedModule : public BaseModule {
private:
    // Configuration
    int ledPin;
    unsigned long blinkInterval;

    // Internal State
    bool isBlinking;
    uint32_t blinkTaskHandle;

    // --- Method Declarations ---
    // This method will be implemented in LedModule_events.cpp
    void handleButtonPress(void* payload);

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

### 1.2. Update the Implementation Files

Now, let's separate the implementation. The `start()` method in `LedModule.cpp` will subscribe to the event, and the actual logic will live in a new `LedModule_events.cpp` file.

**`lib/LedFlasher/src/LedModule.cpp` (Lifecycle Methods):**

```cpp
#include "LedModule.h"

LedModule::LedModule(const JsonObject& config) {
    ledPin = config["pin"] | LED_BUILTIN;
    blinkInterval = config["interval_ms"] | 1000;
    isBlinking = false; // Start with blinking disabled
    blinkTaskHandle = 0;
}

const char* LedModule::getName() const {
    return "LedModule";
}

void LedModule::init() {
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, LOW); // Ensure LED is off initially
    NEXTINO_LOGI(getName(), "Initialized on pin %d.", ledPin);
}

void LedModule::start() {
    // The start() method's only job is to set up subscriptions.
    // The logic itself is in another file.
    NextinoEvent().on("button_pressed", [this](void* payload) {
        this->handleButtonPress(payload);
    });
    NEXTINO_LOGI(getName(), "Subscribed to 'button_pressed' event.");
}
```

**`lib/LedFlasher/src/LedModule_events.cpp` (Event Handling Logic - NEW FILE):**

```cpp
#include "LedModule.h"

// This is the implementation of our event handler.
// It's part of the LedModule class but lives in its own file for clarity.
void LedModule::handleButtonPress(void* payload) {
    isBlinking = !isBlinking; // Toggle the blinking state

    if (isBlinking) {
        NEXTINO_LOGI(getName(), "Button event received! Starting blink.");
        // If a task handle doesn't exist, create one.
        if (blinkTaskHandle == 0) {
            blinkTaskHandle = NextinoScheduler().scheduleRecurring(blinkInterval, [this]() {
                digitalWrite(ledPin, !digitalRead(ledPin));
            });
        }
    } else {
        NEXTINO_LOGI(getName(), "Button event received! Stopping blink.");
        // If a task handle exists, remove it from the scheduler.
        if (blinkTaskHandle != 0) {
            NextinoScheduler().cancel(blinkTaskHandle);
            blinkTaskHandle = 0;
            digitalWrite(ledPin, LOW); // Ensure LED is turned off
        }
    }
}
```

:::danger Important: Scheduler Update
To make this work, you will need to add a `cancel(uint32_t handle)` method to your `Scheduler` class. The `scheduleRecurring` method should now return a unique handle (e.g., an incrementing integer) for each task.
:::

## Step 2: Update Your Project

The process to update your project remains the same.

1. **Create the `ButtonReader` module** in your `lib` folder as described in the [previous tutorial](./creating-a-custom-module).
2. **Update `platformio.ini`** to include both modules as dependencies.

    ```ini title="platformio.ini"
    lib_deps =
        ../../      # The Nextino Framework itself
        lib/LedFlasher
        lib/ButtonReader
    ```

3. **The `bootstrap.py` script will do the rest!** Your `main.cpp` **does not need to change at all**.

## Step 3: Run and Test

Upload the code to your board. The behavior will be the same as before, but now your `LedModule`'s code is cleanly separated by responsibility, making it much easier to read and maintain.

You have successfully created a system where two modules communicate via events, following a clean and structured file organization.

---

### Next Steps

Now that you've mastered module creation and communication, let's look at how to manage hardware resources safely.

➡️ **[Using the ResourceManager](./using-the-resourcemanager.md)**
