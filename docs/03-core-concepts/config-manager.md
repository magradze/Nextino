---
sidebar_position: 4
title: 'Config Manager v2'
---

# 💾 Config Manager v2: Validation & Overlays

As your Nextino projects grow, managing configurations becomes critically important. `Config Manager v2` is an umbrella term for a suite of advanced features designed to make your configurations more robust, flexible, and error-proof.

The first major feature of this system is **Build-Time Schema Validation**.

---

## ✅ Build-Time Schema Validation

Have you ever spent hours debugging strange behavior, only to find you misspelled a key in your `config.json` file? Schema validation solves this problem forever! 💥

By creating a simple `schema.json` file inside your module's library, you can define the "rules" for your module's configuration. The Nextino build script will then automatically validate every instance's configuration against these rules **before compiling your code**.

If any rule is violated, the build process will stop and show you a precise error message, telling you exactly what is wrong and where.

### 🎯 The Goal: Fail Fast, Fail Loudly

The philosophy is simple: it's better to find a configuration error at compile-time than to debug a mysterious bug at runtime.

### How to Create a `schema.json`

Alongside your module's `config.json` and `library.json`, simply create a new file named `schema.json`.

**Example: A `schema.json` for a hypothetical `LedModule`**

```json title="lib/LedFlasher/schema.json"
{
  "resource": {
    "type": "object",
    "required": true,
    "description": "The hardware pin definition for the LED."
  },
  "blink_interval_ms": {
    "type": "integer",
    "required": false,
    "default": 1000,
    "description": "The blinking interval in milliseconds. If not provided, defaults to 1000."
  },
  "initial_state": {
    "type": "string",
    "required": false,
    "options": ["ON", "OFF"],
    "description": "The initial state of the LED on boot."
  }
}
```

### Supported Schema Rules (v0.4.0)

For now, the validator supports a simple but powerful set of rules:

* `"type"`: Defines the expected JSON data type. Supported values are:
  * `"object"` (for nested objects like `resource`)
  * `"string"`
  * `"integer"`
  * `"number"` (for floating-point values)
  * `"boolean"` (`true` or `false`)
  * `"array"`
* `"required"`: A boolean (`true` or `false`). If `true`, the build will fail if this key is missing from the `config` object.
* `"description"`: A brief explanation of the parameter. (For future documentation tools).
* `"default"`: The default value used if the key is not provided. (For future documentation tools).
* `"options"`: An array of allowed string values for a key. (Currently for documentation, future validation).

### 🧐 What Happens When Validation Fails?

Let's say you make a typo in your `config.json` and set `blink_interval_ms` to a string instead of an integer.

When you run `pio run`, instead of a successful build, you will see this:

```log
--- [Nextino Bootstrap] Phase: Validating module configurations... ---
  -> Validating 'status_led' (LedModule)...
ERROR: Invalid type for key 'blink_interval_ms' in instance 'status_led'. Expected 'integer', got 'str'.
FATAL ERROR: Configuration validation failed. Halting build.
```

The build stops and tells you exactly what went wrong! 🎉 This simple feature will save you countless hours of debugging.