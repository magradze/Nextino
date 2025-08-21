---
sidebar_position: 4
title: 'The Scheduler'
---

# ⏱️ The Scheduler: Non-Blocking by Default

One of the biggest challenges in traditional Arduino programming is managing time. The simple `delay()` function is easy to use, but it's a trap: it completely freezes your microcontroller, preventing it from doing anything else.

Nextino solves this with a built-in, non-blocking **Scheduler**. It's the heart of the framework's responsive, multitasking capability.

---

## 🚫 The Problem with `delay()`

Imagine you want to blink an LED every second and also check for a button press. A traditional approach might look like this:

```cpp title="Traditional Arduino Code (The Bad Way)"
void loop() {
  digitalWrite(LED_PIN, HIGH);
  delay(1000); // The MCU is completely frozen here for 1 second!
  digitalWrite(LED_PIN, LOW);
  delay(1000); // And frozen again...

  // What if the user presses a button during the delay?
  // Too bad! The program won't notice until the delay is over.
  checkButton(); 
}
```

This code is **blocking**. While the `delay()` is running, the processor can't read sensors, respond to inputs, or handle network traffic. For any serious project, this is unacceptable.

## ✨ The Nextino Solution: Cooperative Multitasking

The Nextino `Scheduler` allows you to schedule functions (callbacks) to run at specific intervals without ever blocking the main loop. This is a form of **cooperative multitasking**.

The `SystemManager` calls `Scheduler::loop()` on every single iteration of the main program `loop()`. The scheduler quickly checks its list of tasks and executes only those whose time has come.

```mermaid
graph LR
    subgraph "Main &#96;loop()&#96;"
        A["NextinoSystem().loop()"]
    end

    subgraph "&#96;NextinoSystem().loop()&#96;"
        B["Scheduler.loop()"] --> C{"Time for Task A?"};
        C -- Yes --> D["Execute Task A"];
        C -- No --> E{"Time for Task B?"};
        E -- Yes --> F["Execute Task B"];
        E -- No --> G["..."];
    end

    style A fill:#9f6,stroke:#333,stroke-width:2px
```

This entire process is incredibly fast. If no tasks are due, `Scheduler::loop()` finishes in microseconds, allowing other modules to run their `loop()` methods immediately.

---

## 🛠️ How to Use the Scheduler

The Scheduler provides two simple methods for adding tasks. You typically call these from your module's `start()` method.

### `scheduleRecurring(intervalMs, callback)`

This is the most common method. It schedules a function to be called repeatedly with a fixed interval.

* `intervalMs`: The time in milliseconds between each execution.
* `callback`: The function to be called. This is often a C++ lambda function.

```cpp title="Example: LedModule::start()"
void LedModule::start() {
    // Schedule the toggle() method to be called every 500 milliseconds.
    NextinoScheduler().scheduleRecurring(500, [this]() {
        this->toggle(); 
    });
    NEXTINO_LOGI(getName(), "Blink task scheduled.");
}

void LedModule::toggle() {
    digitalWrite(ledPin, !digitalRead(ledPin));
}
```

### `scheduleOnce(delayMs, callback)`

This method schedules a function to be called only **once** after a specified delay.

* `delayMs`: The time in milliseconds to wait before execution.
* `callback`: The function to be called.

```cpp title="Example: A hypothetical WelcomeModule"
void WelcomeModule::start() {
    // After a 2-second delay, print a welcome message.
    NextinoScheduler().scheduleOnce(2000, []() {
        NEXTINO_LOGI("WelcomeModule", "System is ready! Welcome!");
    });
}
```

---

## 💡 Best Practices

* **Keep Callbacks Short and Fast:** Your scheduled functions should execute quickly. If you have a long-running task, break it down into smaller steps or consider using it in the module's `loop()` as a state machine.
* **No `delay()` in Callbacks:** Never use `delay()` inside a scheduled callback. This defeats the entire purpose of the non-blocking scheduler.
* **Register in `start()`:** The best place to schedule your tasks is in your module's `start()` method. This ensures all other modules have been initialized.

By embracing the Scheduler, you can build complex, multi-tasking applications that are responsive, efficient, and easy to reason about.

---

### Next Steps

Now that you understand the core components, let's look at how they communicate.

➡️ **[Communication Patterns](./communication-patterns.md)**
