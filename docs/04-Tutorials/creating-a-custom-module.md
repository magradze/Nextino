---
sidebar_position: 1
title: 'Creating a Custom Module'
---

# 🛠️ Creating a Custom Module

This tutorial will guide you through the process of creating your own reusable, plug-and-play module for the Nextino framework. We will build a `ButtonModule` that demonstrates best practices for module structure and code organization, mirroring the clean, separated logic of the original Synapse Framework.

By the end, you will have a self-contained module with its logic split into distinct, responsible files.

---

## 🎯 The Goal

We will create a `ButtonModule` with the following features:

1. It will monitor a GPIO pin connected to a push button.
2. It will handle debouncing to prevent multiple triggers.
3. When a valid press is detected, it will **post an event** to the `EventBus`.
4. Its code will be cleanly separated into `ButtonModule.cpp` (lifecycle) and `ButtonModule_events.cpp` (logic).

## Step 1: Module File Structure

First, let's create the file structure for our new module. In your project's `lib` directory, create a new folder named `ButtonReader`.

```
lib/
└── ButtonReader/
    ├── config.json
    ├── library.json
    └── src/
        ├── ButtonModule.h
        ├── ButtonModule.cpp         // For constructor and lifecycle methods
        └── ButtonModule_events.cpp  // For event handling and core logic
```

:::info File Organization Principle
Nextino encourages separating code by responsibility to keep modules clean and maintainable:
- **`ModuleName.cpp`**: This file should contain the constructor and the main `BaseModule` lifecycle methods (`init`, `start`, `loop`). It acts as the module's "orchestrator".
- **`ModuleName_events.cpp`**: This file is ideal for the core logic, especially event handling or, as in this tutorial, the logic that *produces* events.
- **`ModuleName_api.cpp` (Future)**: This file would contain the implementation for a `ServiceLocator` API.
- **`ModuleName_cmd.cpp` (Future)**: This file would contain handlers for CLI commands.
:::

### 1.1. The Configuration (`config.json`)

```json title="lib/ButtonReader/config.json"
{
  "type": "ButtonModule",
  "config": {
    "pin": 4,
    "debounce_delay_ms": 50
  }
}
```

### 1.2. The Manifest (`library.json`)

```json title="lib/ButtonReader/library.json"
{
    "name": "ButtonReader",
    "version": "1.0.0",
    "description": "A non-blocking button reader module for the Nextino framework.",
    "keywords": "nextino-module, button, input",
    "dependencies": { "Nextino": "*" }
}
```

## Step 2: The Header File (`ButtonModule.h`)

The header file defines the class interface and declares all methods, including those that will be implemented in separate `.cpp` files.

```cpp title="lib/ButtonReader/src/ButtonModule.h"
#pragma once
#include <Nextino.h>

class ButtonModule : public BaseModule {
private:
    // Configuration
    int buttonPin;
    unsigned long debounceDelay;

    // Internal State
    int lastButtonState;
    unsigned long lastDebounceTime;

    // --- Method Declarations ---
    // This method is part of the core logic and will be defined in ButtonModule_events.cpp
    void checkButtonAndPublishEvent();

public:
    ButtonModule(const JsonObject& config);
    
    static BaseModule* create(const JsonObject& config) {
        return new ButtonModule(config);
    }

    // --- BaseModule Lifecycle Methods (implemented in ButtonModule.cpp) ---
    const char* getName() const override;
    void init() override;
    void loop() override;
};
```

## Step 3: The Implementation Files

Now we'll implement the declared methods in their respective files.

### 3.1. Main Implementation (`ButtonModule.cpp`)

This file is responsible only for the "official" `BaseModule` contract.

```cpp title="lib/ButtonReader/src/ButtonModule.cpp"
#include "ButtonModule.h"

ButtonModule::ButtonModule(const JsonObject& config) {
    buttonPin = config["pin"];
    debounceDelay = config["debounce_delay_ms"] | 50;
    lastButtonState = HIGH; // Assuming pull-up resistor
    lastDebounceTime = 0;
}

const char* ButtonModule::getName() const {
    return "ButtonModule";
}

void ButtonModule::init() {
    pinMode(buttonPin, INPUT_PULLUP);
    NEXTINO_LOGI(getName(), "Initialized on pin %d.", buttonPin);
}

void ButtonModule::loop() {
    // The main loop delegates the core work to the logic function.
    checkButtonAndPublishEvent();
}
```

### 3.2. Event Logic (`ButtonModule_events.cpp`)

This file contains the actual "work" of the module.

```cpp title="lib/ButtonReader/src/ButtonModule_events.cpp"
#include "ButtonModule.h"

// This is a private method of the ButtonModule class, but defined here
// to keep the logic separated.
void ButtonModule::checkButtonAndPublishEvent() {
    int reading = digitalRead(buttonPin);

    // If the switch changed, due to noise or pressing, reset the timer
    if (reading != lastButtonState) {
        lastDebounceTime = millis();
    }

    // Check if the state has been stable for longer than the debounce delay
    if ((millis() - lastDebounceTime) > debounceDelay) {
        // A valid state change has occurred
        if (reading == LOW && lastButtonState == HIGH) { // Button was just pressed
            NEXTINO_LOGI(getName(), "Button Pressed! Posting event...");
            
            // Post an event to the EventBus for other modules to hear
            NextinoEvent().post("button_pressed");
        }
    }

    lastButtonState = reading;
}
```

## Step 4: Using Your New Module

The process remains the same. Add `ButtonReader` to your project's `lib_deps`, and the `bootstrap.py` script will discover it and generate the necessary configuration. The `SystemManager` will then create and run it automatically.

---

### Next Steps

Now that you have a module that produces events, let's see how another module can listen for them.

➡️ **[Using the EventBus](./using-the-eventbus.md)**
