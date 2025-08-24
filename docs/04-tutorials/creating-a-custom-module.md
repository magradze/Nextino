---
sidebar_position: 2
title: 'Creating a Custom Module'
---

# 🛠️ Creating a Custom Module

This is where the fun begins! This tutorial will guide you through creating your own reusable, plug-and-play Nextino module from scratch. We will build a `ButtonModule` that demonstrates all the core best practices:

* **Proper File Structure**
* **Safe Hardware Access** via the `ResourceManager`
* **Multiple Instance Support**
* **Broadcasting Events** via the `EventBus`

By the end, you will have a powerful, self-contained module that can detect both **short** and **long presses** and notify the entire system about them! 🚀

---

## 🎯 The Goal

We will create a `ButtonModule` with the following features:

1. It will monitor a single GPIO pin connected to a push button.
2. It will be "resource-aware," declaring its pin in a `config.json` file.
3. It will support multiple instances (e.g., you can have 10 buttons in your project using this one module).
4. It will handle debouncing to prevent false triggers.
5. It will post distinct events (`button_short_press`, `button_long_press`) to the `EventBus`.

## Step 1: The Anatomy of a Module 🧬

First, let's create the file structure. In your project's `lib` directory, create a new folder named `ButtonReader`. Inside, we'll need three key files.

```plaintext
lib/
└── ButtonReader/
    ├── config.json       # ⚙️ The default configuration
    ├── library.json      # 📦 The module's "passport"
    └── src/
        ├── ButtonModule.h  # 📜 The public contract (header)
        └── ButtonModule.cpp  # 🧠 The implementation (logic)
```

### 1.1. The Configuration (`config.json`)

This file defines the default instance(s) for our module. As per Nextino's best practice, the root of this file **is always an array `[]`**, even for a single instance.

```json title="lib/ButtonReader/config.json"
[
  {
    "type": "ButtonModule",
    "instance_name": "main_button",
    "config": {
      "resource": {
        "type": "gpio",
        "pin": 4
      },
      "long_press_ms": 1000,
      "debounce_delay_ms": 50
    }
  }
]
```

✨ **Notice the `"resource"` object!** This tells the framework to lock the specified GPIO pin for this instance, preventing hardware conflicts.

### 1.2. The Manifest (`library.json`)

This is the standard PlatformIO manifest file, with one special keyword that makes our module discoverable by Nextino.

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

The header file is the public "contract" of our module. It defines the class, its private members, and the public methods that the framework and other modules will interact with.

```cpp title="lib/ButtonReader/src/ButtonModule.h"
#pragma once
#include <Nextino.h>

class ButtonModule : public BaseModule {
private:
    // Configuration
    int _pin;
    unsigned long _longPressTime;
    unsigned long _debounceDelay;
    
    // Internal state for detection logic
    int _buttonState;
    int _lastButtonState;
    unsigned long _lastDebounceTime;
    unsigned long _pressStartTime;
    bool _longPressTriggered;

public:
    // The constructor now accepts an instanceName, which is mandatory
    ButtonModule(const char* instanceName, const JsonObject& config);
    
    // The static create function must match the ModuleFactory's signature
    static BaseModule* create(const char* instanceName, const JsonObject& config) {
        return new ButtonModule(instanceName, config);
    }

    // --- BaseModule Lifecycle Methods ---
    const char* getName() const override; // Returns the TYPE of module
    void init() override;
    void loop() override;
};
```

## Step 3: The Implementation (`ButtonModule.cpp`)

This is where the magic happens! We'll bring our module to life.

```cpp title="lib/ButtonReader/src/ButtonModule.cpp"
#include "ButtonModule.h"

ButtonModule::ButtonModule(const char* instanceName, const JsonObject& config)
    // CRITICAL: Always pass the instanceName to the BaseModule constructor!
    : BaseModule(instanceName) {

    // Read pin from the "resource" object for ResourceManager compatibility
    _pin = config["resource"]["pin"];

    // Read other parameters with default values
    _longPressTime = config["long_press_ms"] | 1000;
    _debounceDelay = config["debounce_delay_ms"] | 50;

    // Initialize internal state variables
    _lastButtonState = HIGH;
    _buttonState = HIGH;
    _lastDebounceTime = 0;
    _pressStartTime = 0;
    _longPressTriggered = false;
}

// getName() returns the static CLASS/TYPE name of the module.
const char* ButtonModule::getName() const {
    return "ButtonModule";
}

void ButtonModule::init() {
    pinMode(_pin, INPUT_PULLUP);
    // Use getInstanceName() for logging to distinguish between different buttons
    NEXTINO_LOGI(getInstanceName(), "Initialized on pin %d.", _pin);
}

void ButtonModule::loop() {
    int reading = digitalRead(_pin);

    if (reading != _lastButtonState) {
        _lastDebounceTime = millis();
    }

    if ((millis() - _lastDebounceTime) > _debounceDelay) {
        if (reading != _buttonState) {
            _buttonState = reading;

            if (_buttonState == LOW) { // Button was JUST pressed
                _pressStartTime = millis();
                _longPressTriggered = false;
                NEXTINO_LOGD(getInstanceName(), "Press started.");
            } else { // Button was JUST released
                if (!_longPressTriggered) {
                    NEXTINO_LOGI(getInstanceName(), "Short press detected! Posting event.");
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
            NEXTINO_LOGI(getInstanceName(), "Long press detected! Posting event.");
            NextinoEvent().post("button_long_press");
        }
    }

    _lastButtonState = reading;
}
```

## Step 4: Using Your New Module

The process is beautifully simple. Just add your new `ButtonReader` library to your project's `platformio.ini` `lib_deps`.

The `bootstrap.py` script and the `SystemManager` handle everything else automatically: discovery, configuration, creation, and resource protection. **Your `main.cpp` stays clean and untouched!** 😎

---

### Next Steps

Now that you have a module that produces events, let's see how another module can listen for them and create interactive behavior.

➡️ **[Using the EventBus](/tutorials/using-the-eventbus)**
