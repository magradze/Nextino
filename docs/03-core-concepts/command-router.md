---
sidebar_position: 7
title: '⌨️ The Command Router'
---

# ⌨️ The Command Router

One of the most powerful features for debugging and controlling an embedded device is a **Command-Line Interface (CLI)**. Nextino provides a flexible and powerful core service called the **`CommandRouter`** that acts as a central "brain" 🧠 for processing text-based commands.

The `CommandRouter` allows any module to register its own commands, which can then be triggered from any source, like the Serial Monitor, MQTT, or even a Bluetooth connection.

---

## 🎯 The Philosophy: Decoupled Command Handling

The `CommandRouter` is designed with a clean separation of concerns:

1. **Modules** are the "experts." They know *what* to do and *how* to do it. They register their capabilities (e.g., `"on"`, `"off"`, `"set_temp"`) with the router.
2. **Input Sources** (like a `SerialCliModule` or `MqttClientModule`) are the "messengers." Their only job is to receive a string and pass it to the router. They don't know what the commands mean.
3. The **`CommandRouter`** is the "operator." Its only job is to parse the incoming string, find the correct module and command handler that was registered for it, and execute it. It doesn't know *what* the command does, only *who* is responsible for it.

This architecture makes the entire system incredibly flexible and extensible.

## ⚙️ How It Works

### 1. Registering a Command

Modules register their commands inside the `registerCommands()` lifecycle method. This method is called once for every module during system startup.

The command registration requires three pieces of information:

* **The instance name:** Which specific module instance owns this command (e.g., `"status_led"`).
* **The command name:** What to type to trigger the action (e.g., `"on"`).
* **The handler function:** The actual C++ code to run. This is usually a C++ lambda function.

```cpp title="Example: LedModule_cmd.cpp"
#include "LedModule.h"

void LedModule::registerCommands() {
    // Define the handler for the "on" command
    CommandHandler onHandler = [this](const std::vector<std::string>& args) -> std::string {
        this->turnOn();
        // Return a success message as a string
        return "OK: Turned ON.";
    };

    // Register the command for this specific instance
    NextinoCommands().registerCommand(getInstanceName(), "on", onHandler);
}
```

### 2. Executing a Command

Any part of the system can now execute this command by passing a simple string to the router's `execute()` method.

The command string format is always: `"<instance_name> <command> [arg1] [arg2] ..."`

```cpp
// This might come from Serial, MQTT, etc.
std::string inputString = "status_led on";

// Any module can call the global execute function
std::string result = NextinoCommands().execute(inputString);

// `result` will now contain "OK: Turned ON."
```

The `CommandRouter` handles parsing the string into parts, finding the registered `onHandler` for the `"status_led"` instance, and executing it.

---

## 💡 Practical Use Cases

* **Interactive Debugging:** Quickly test your modules' functions directly from the Serial Monitor without having to re-flash your code.
* **Remote Control:** Trigger commands over MQTT or a web interface for remote device control.
* **Automated Testing:** Write scripts that send a series of commands to your device to test its functionality.
* **Factory Configuration:** Use commands to set initial calibration values or device parameters during manufacturing.

The `CommandRouter` transforms your Nextino application from a passive device into an interactive and easily controllable system.
