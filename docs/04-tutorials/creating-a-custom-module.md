---
sidebar_position: 1
title: 'Creating a Custom Module'
---

# 🛠️ Creating a Custom Module

This tutorial will guide you through creating your own reusable, plug-and-play module for the Nextino framework. We will build a `ButtonModule` that demonstrates best practices for module structure, code organization, and safe hardware access using the `ResourceManager`.

By the end, you will have a self-contained module that can detect both **short** and **long presses** and broadcast them as separate events! 🚀

---

## 🎯 The Goal

We will create a `ButtonModule` with the following features:

1. It will monitor a GPIO pin connected to a push button.
2. It will be "resource-aware," declaring its pin usage for the `ResourceManager`.
3. It will handle debouncing to prevent false triggers.
4. It will detect the difference between a short press and a long press.
5. It will post distinct events (`button_short_press`, `button_long_press`) to the `EventBus`.

## Step 1: Module File Structure

First, let's create the file structure. In your project's `lib` directory, create a new folder named `ButtonReader`.

```plaintext
lib/
└── ButtonReader/
    ├── config.json
    ├── library.json
    └── src/
        ├── ButtonModule.h
        └── ButtonModule.cpp
```

:::info Splitting Logic into Multiple Files
For more complex modules, Nextino encourages separating code by responsibility. For example:

- **`ModuleName.cpp`**: Contains the constructor and `BaseModule` lifecycle methods (`init`, `start`, `loop`).
- **`ModuleName_events.cpp`**: Contains the core logic, especially for handling or producing events.

For this tutorial, we'll keep the logic in a single `.cpp` file for simplicity.
:::

### 1.1. The Configuration (`config.json`)

This is where we declare our module's parameters and, crucially, the hardware resource it needs.

```json title="lib/ButtonReader/config.json"
{
  "type": "ButtonModule",
  "config": {
    "resource": {
      "type": "gpio",
      "pin": 4
    },
    "long_press_ms": 1000,
    "debounce_delay_ms": 50
  }
}
```

✨ **Notice the `"resource"` object!** This special key tells the `SystemManager` to ask the `ResourceManager` to lock GPIO pin 4 for this module, preventing any other module from using it by mistake.

### 1.2. The Manifest (`library.json`)

This file tells PlatformIO about our library and marks it as a Nextino module.

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

The header file defines the class interface, its private members, and public methods.

```cpp title="lib/ButtonReader/src/ButtonModule.h"
#pragma once
#include <Nextino.h>

class ButtonModule : public BaseModule {
private:
    // Configuration
    int _pin;
    unsigned long _longPressTime;
    unsigned long _debounceDelay;
    
    // Internal state for debouncing and long press detection
    int _buttonState;
    int _lastButtonState;
    unsigned long _lastDebounceTime;
    unsigned long _pressStartTime;
    bool _longPressTriggered;

public:
    ButtonModule(const JsonObject& config);
    
    static BaseModule* create(const JsonObject& config) {
        return new ButtonModule(config);
    }

    // --- BaseModule Lifecycle Methods ---
    const char* getName() const override;
    void init() override;
    void loop() override;
};
```

## Step 3: The Implementation (`ButtonModule.cpp`)

This is where the magic happens! We'll implement the constructor and the lifecycle methods. The core logic will live inside the `loop()` method.

```cpp title="lib/ButtonReader/src/ButtonModule.cpp"
#include "ButtonModule.h"

ButtonModule::ButtonModule(const JsonObject& config) {
    // Read pin from the "resource" object for ResourceManager compatibility
    _pin = config["resource"]["pin"];

    // Read other parameters with default values
    _longPressTime = config["long_press_ms"] | 1000;
    _debounceDelay = config["debounce_delay_ms"] | 50;

    // Initialize internal state
    _lastButtonState = HIGH;
    _buttonState = HIGH;
    _lastDebounceTime = 0;
    _pressStartTime = 0;
    _longPressTriggered = false;
}

const char* ButtonModule::getName() const {
    return "ButtonModule";
}

void ButtonModule::init() {
    pinMode(_pin, INPUT_PULLUP);
    NEXTINO_LOGI(getName(), "Initialized on pin %d.", _pin);
}

void ButtonModule::loop() {
    int reading = digitalRead(_pin);

    // --- Debounce Logic ---
    if (reading != _lastButtonState) {
        _lastDebounceTime = millis();
    }

    // --- State Change Logic ---
    if ((millis() - _lastDebounceTime) > _debounceDelay) {
        if (reading != _buttonState) {
            _buttonState = reading;

            if (_buttonState == LOW) { // Button was JUST pressed
                _pressStartTime = millis();
                _longPressTriggered = false;
                NEXTINO_LOGD(getName(), "Button press started.");
            } else { // Button was JUST released
                if (!_longPressTriggered) {
                    NEXTINO_LOGI(getName(), "Short press detected! Posting event.");
                    NextinoEvent().post("button_short_press");
                }
                _pressStartTime = 0; // Reset timer
            }
        }
    }

    // --- Long Press Detection Logic ---
    if (_buttonState == LOW && _pressStartTime != 0 && !_longPressTriggered) {
        if (millis() - _pressStartTime > _longPressTime) {
            _longPressTriggered = true;
            NEXTINO_LOGI(getName(), "Long press detected! Posting event.");
            NextinoEvent().post("button_long_press");
        }
    }

    _lastButtonState = reading;
}
```

## Step 4: Using Your New Module

The process is beautifully simple. Just add your new `ButtonReader` library to your project's `platformio.ini` `lib_deps`.

The `bootstrap.py` script will automatically discover it, read its `config.json`, and the `SystemManager` will create it and run its lifecycle methods. The `ResourceManager` will ensure its pin is protected. **You don't need to change `main.cpp` at all!**

---

### Next Steps

Now that you have a module that produces events, let's see how another module can listen for them and create interactive behavior.

➡️ **[Using the EventBus](/tutorials/using-the-eventbus)**
