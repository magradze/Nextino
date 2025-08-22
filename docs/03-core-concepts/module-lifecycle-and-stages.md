---
sidebar_position: 2
title: 'The Lifecycle of a Module'
---

# 🔄 The Lifecycle of a Module

Every module in the Nextino framework follows a consistent and predictable lifecycle, managed entirely by the `SystemManager`. Understanding this lifecycle is key to writing clean, efficient, and bug-free modules.

The lifecycle consists of three main phases: **Initialization**, **Starting**, and **Looping**.

---

## The Journey of a Module

Here's a step-by-step breakdown of what happens from the moment your application starts.

```mermaid
graph TD
    A[Application Starts: `setup()`] --> B{`NextinoSystem().begin()` is called};
    B --> C[Phase 1: Initialization];
    C --> D[Phase 2: Starting];
    D --> E[Application Loop: `loop()`];
    E --> F{`NextinoSystem().loop()` is called};
    F --> G[Phase 3: Looping];
    G --> F;

    subgraph C
        C1(For every registered module...)
        C2(Call `module->init()`)
    end

    subgraph D
        D1(For every registered module...)
        D2(Call `module->start()`)
    end

    subgraph G
        G1(Call `Scheduler::loop()`)
        G2(For every registered module...)
        G3(Call `module->loop()`)
    end
```

### Phase 1: Initialization (`init()`)

* **When is it called?** The `init()` method of every registered module is called once, in sequence, when you call `NextinoSystem().begin()`.
* **What should you do here?** This is the place for one-time setup operations that prepare your module but don't yet start any active processes.
  * ✅ **Good:** Setting pin modes (`pinMode()`), initializing hardware libraries (`Wire.begin()`), locking resources with the `ResourceManager`.
  * ❌ **Bad:** Starting a blinking task, subscribing to events, or performing long-running operations.

```cpp title="Example: LedModule::init()"
void LedModule::init() {
    // Set the pin to be an output. This only needs to happen once.
    pinMode(ledPin, OUTPUT);
    NEXTINO_LOGI(getName(), "Initialized on pin %d.", ledPin);
}
```

### Phase 2: Starting (`start()`)

* **When is it called?** The `start()` method is called once for every module, immediately after **all** modules have completed their `init()` phase.
* **Why the separation?** This two-phase approach is crucial. It guarantees that when your module's `start()` method is called, you can safely assume that **all other modules have already been initialized**. This prevents race conditions where one module tries to use another before it's ready.
* **What should you do here?** This is where you kick off the active, ongoing processes of your module.
  * ✅ **Good:** Scheduling tasks with the `Scheduler`, subscribing to events on the `EventBus`, connecting to a WiFi network.
  * ❌ **Bad:** One-time hardware setup (that belongs in `init()`).

```cpp title="Example: LedModule::start()"
void LedModule::start() {
    // Schedule a recurring task to toggle the LED.
    NextinoScheduler().scheduleRecurring(blinkInterval, [this]() {
        digitalWrite(ledPin, !digitalRead(ledPin));
    });
    NEXTINO_LOGI(getName(), "Blink task scheduled.");
}
```

### Phase 3: Looping (`loop()`)

* **When is it called?** The `loop()` method of every module is called on **every single iteration** of the main Arduino `loop()`, after the `Scheduler` has been processed.
* **What should you do here?** This method is for logic that needs to be checked continuously and cannot be handled by a timed scheduler.
  * ✅ **Good:** Reading a sensor value that changes rapidly, updating a state machine, checking for incoming data on a serial port.
  * ⚠️ **Crucial:** The `loop()` method **must be non-blocking**. Never use `delay()` or long-running `while` loops inside it, as this will freeze the entire framework and all other modules.

```cpp title="Example: A hypothetical SensorModule::loop()"
void SensorModule::loop() {
    // Non-blocking check for new data from a sensor library.
    if (sensor.available()) {
        float temperature = sensor.readTemperature();
        // Post an event with the new data
        NextinoEvent().post("temperature_update", &temperature);
    }
}
```

---

### Summary: Where to Put Your Code

| If you need to...                               | Put your code in... |
| ----------------------------------------------- | ------------------- |
| Set up hardware once (e.g., `pinMode`)          | `init()`            |
| Start a recurring task (e.g., blinking an LED)  | `start()`           |
| Subscribe to an event                           | `start()`           |
| Continuously check something (non-blocking)     | `loop()`            |

---

### Next Steps

Now that you understand the lifecycle, let's look at the system that makes it all possible: the configuration.

➡️ **[The Configuration System](./configuration-system)**
