---
sidebar_position: 3
title: 'Safe Hardware Access'
---

# 🛡️ Safe Hardware Access with the ResourceManager

Have you ever spent hours debugging a project, only to realize that two different modules were trying to use the same pin? It's a common and frustrating problem in embedded development.

Nextino provides a powerful built-in service called the **`ResourceManager`** to prevent this. It acts as a traffic controller for your board's hardware, ensuring that every resource (like a GPIO pin or an I2C address) is used by only one module at a time.

This tutorial will show you how to make your modules "resource-aware" and protect your projects from hardware conflicts.

---

## 🎯 The Goal

We will learn how to:

1. **Declare** a module's primary hardware resource in its `config.json` file.
2. Intentionally create a resource conflict between two modules.
3. See how the `ResourceManager` automatically detects the conflict at startup and provides a clear error message.

## Step 1: Declaring a Primary Resource

The key to Nextino's resource management is a standardized, declarative object named `"resource"`. Instead of just providing a pin number, you describe the main hardware resource your module needs.

Let's update our modules to use this clean structure.

### 1.1. Update `LedFlasher`'s Configuration

Open the `config.json` for your `LedFlasher` module.

```json title="lib/LedFlasher/config.json"
{
  "type": "LedModule",
  "config": {
    "resource": {
      "type": "gpio",
      "pin": 2,
      "mode": "output"
    },
    "blink_interval_ms": 500
  }
}
```

**What this means:**

* `"resource"`: This special object tells the framework that the module requires a hardware resource.
* `"type": "gpio"`: Specifies that the resource is a GPIO pin.
* `"pin": 2`: The pin number.

### 1.2. Update `ButtonReader`'s Configuration

Do the same for your `ButtonReader` module.

```json title="lib/ButtonReader/config.json"
{
  "type": "ButtonModule",
  "config": {
    "resource": {
      "type": "gpio",
      "pin": 4,
      "mode": "input_pullup"
    },
    "debounce_delay_ms": 50
  }
}
```

The framework now knows that `LedModule` needs pin 2 and `ButtonModule` needs pin 4.

## Step 2: Update the Module Code

Your module's C++ code needs to be updated once to read from this new structure.

```cpp title="lib/LedFlasher/src/LedModule.cpp (Constructor)"
LedModule::LedModule(const JsonObject& config) {
    // Read the pin number from the nested "resource" object
    JsonObject resourceObj = config["resource"];
    ledPin = resourceObj["pin"];

    blinkInterval = config["blink_interval_ms"] | 1000;
}
```

The module's code remains clean and contains no locking logic.

## Step 3: Let's Create a Conflict! 💥

Now, let's simulate a common mistake and tell both modules to use the **same pin**.

1. Open `lib/LedFlasher/config.json`.
2. Change the `pin` number inside the `resource` object from `2` to `4`.

```json title="lib/LedFlasher/config.json (with conflict)"
{
  "type": "LedModule",
  "config": {
    "resource": {
      "type": "gpio",
      "pin": 4,
      "mode": "output"
    },
    "blink_interval_ms": 500
  }
}
```

Now, both modules are trying to claim ownership of pin 4.

## Step 4: Observe the Result

1. **Build and Upload** your project.
2. **Open the Serial Monitor.**

Instead of your application running, you will see a critical error message from the framework:

```log
[E] [ResManager]: PIN CONFLICT! Pin 4 is already locked by 'ButtonModule'. Cannot be locked by 'LedModule'.
[E] [SysManager]: Halting system due to resource conflicts. Please check your configuration.
```

**This is a good thing!** Nextino has protected you. It detected the invalid configuration at startup and stopped the program, giving you a clear and precise error message.

---

### Best Practices

* Use the standardized `"resource"` object for your module's primary hardware interface.
* This pattern can be extended to other resource types like `"i2c"`, `"spi"`, etc., in the future.

By using Nextino's declarative resource management, you make your modules more robust, your projects more stable, and your development process much less frustrating.
