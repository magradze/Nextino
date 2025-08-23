---
sidebar_position: 2
title: 'Safe Hardware Access'
---

# 🛡️ Safe Hardware Access with the ResourceManager

Have you ever spent hours debugging a project, only to realize that two different modules were trying to use the same pin? 🤯 It's a common and frustrating problem in embedded development.

Nextino provides a powerful built-in service called the **`ResourceManager`** to prevent this. It acts as a traffic controller 🚦 for your board's hardware, ensuring that every resource (like a GPIO pin or an I2C address) is used by **only one module at a time**.

This tutorial will show you how to make your modules "resource-aware" and protect your projects from hardware conflicts forever.

---

## 🎯 The Goal

We will learn how to:

1. ✅ **Declare** a module's hardware resource in its `config.json` file.
2. 💥 Intentionally create a resource conflict between two modules.
3. 👀 See how the `ResourceManager` automatically detects the conflict at startup and provides a clear, life-saving error message.

## Step 1: Declaring a Resource

The key to Nextino's resource management is a standardized, declarative object named `"resource"`. By using it, you're not just setting a pin number—you're officially requesting exclusive access to it.

Let's use the `LedModule` and `ButtonModule` from our previous tutorials.

### 1.1. `LedModule`'s Configuration

Open the `config.json` for your `LedModule`.

```json title="lib/LedFlasher/config.json"
{
  "type": "LedModule",
  "config": {
    "resource": {
      "type": "gpio",
      "pin": 2
    },
    "blink_interval_ms": 500
  }
}
```

**What this means:**

* `"resource"`: This special object tells the framework that the module requires a hardware resource.
* `"type": "gpio"`: Specifies that the resource is a GPIO pin.
* `"pin": 2`: The pin number being requested.

### 1.2. `ButtonModule`'s Configuration

Do the same for your `ButtonModule`.

```json title="lib/ButtonReader/config.json"
{
  "type": "ButtonModule",
  "config": {
    "resource": {
      "type": "gpio",
      "pin": 4
    },
    "long_press_ms": 1000
  }
}
```

The framework now knows that `LedModule` needs pin 2 and `ButtonModule` needs pin 4. So far, so good! 👍

## Step 2: Reading the Resource in Code

Your module's C++ code needs to be updated to read the pin from this new nested structure. It's a one-time change that makes your module compatible with the `ResourceManager`.

```cpp title="lib/LedFlasher/src/LedModule.cpp (Constructor)"
LedModule::LedModule(const JsonObject& config) {
    // Read the pin number from the nested "resource" object
    _pin = config["resource"]["pin"];

    _interval = config["blink_interval_ms"] | 1000;
    // ... other initializations
}
```

Notice that the module's code contains **no locking logic**. It simply trusts the framework to provide the resource.

## Step 3: Let's Create a Conflict! 💥

Now for the fun part! Let's simulate a common mistake and tell both modules to use the **exact same pin**.

1. Open `lib/LedFlasher/config.json`.
2. Change the `pin` number inside the `resource` object from `2` to `4`.

```json title="lib/LedFlasher/config.json (with conflict)"
{
  "type": "LedModule",
  "config": {
    "resource": {
      "type": "gpio",
      "pin": 4 // This now conflicts with ButtonModule!
    },
    "blink_interval_ms": 500
  }
}
```

Now, both `LedModule` and `ButtonModule` are trying to claim ownership of pin 4. Let's see what happens.

## Step 4: Observe the Magic ✨

1. **Build and Upload** your project.
2. **Open the Serial Monitor.**

Instead of your application running unpredictably (or worse, silently failing), you will see a crystal-clear, critical error message from the framework:

```log
[E] [ResManager]: RESOURCE CONFLICT! Resource (Type: 0, ID: 4) is already locked by 'ButtonModule'. Cannot be locked by 'LedModule'.
[E] [SysManager]: RESOURCE CONFLICT DETECTED! System will not start modules.
```

**This is a huge win!** 🎉 Nextino has protected you. It detected the invalid configuration at startup and stopped the program, telling you:

* **WHAT** the conflict is (Resource Conflict).
* **WHERE** it is (Resource Type 0, which is GPIO, and ID 4, which is the pin number).
* **WHO** is involved (`ButtonModule` already has it, `LedModule` wants it).

---

### Best Practices

* Always use the standardized `"resource"` object for your module's primary hardware interface.
* Remember that this pattern works for all supported resource types, like `"i2c"`, `"spi"`, and `"uart"`.

By using Nextino's declarative resource management, you make your modules more robust, your projects more stable, and your development process much, much less frustrating. Happy (and safe) coding! ❤️
