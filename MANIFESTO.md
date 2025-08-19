# The Nextino Manifesto

## Vision: Bringing Architectural Discipline to the Arduino World

Nextino is not just another library. It is a **design philosophy** for building scalable, maintainable, and robust applications on Arduino-compatible platforms like ESP32, STM32, and AVR.

This manifesto outlines the principles that guide Nextino's development:

> "We don't just make things blink. We build systems that last."

---

### 1. The Framework is the Foundation — Structure over Spaghetti

At the heart of Nextino lies a simple yet powerful architecture. The framework does not dictate *what* your modules should do — it defines a clean and predictable way for them to **coexist and operate**.

* The **SystemManager** and **BaseModule** define an enforced lifecycle (`init`, `start`, `loop`) for every component.
* The **ModuleFactory** and **auto-discovery script** wire modules into the system based on a clear configuration, eliminating hidden "magic".
* The **Scheduler** provides a non-blocking foundation, freeing you from the tyranny of `delay()`.

The framework is **the contract** that makes your code predictable — not a suggestion.

### 2. Configuration as the Blueprint — Not Just Variables

Nextino treats configuration (`config.json`) as the declarative blueprint of your project:

* Each module declares its own default configuration.
* The build system automatically discovers these configurations and assembles a single source of truth for the project.
* You build your system by describing it, not by writing complex boilerplate in `main.cpp`.

This promotes consistency and allows you to change your project's behavior without changing its code.

### 3. Asynchrony is a Tool — Not a Barrier

While many embedded platforms force complex asynchronous patterns, Nextino treats asynchrony as an accessible tool for building responsive applications:

* The **Scheduler** makes time-based, non-blocking operations simple and intuitive.
* The **EventBus** (coming soon) will provide a clean way for modules to communicate without being tightly coupled.
* Asynchronous interactions are not "all or nothing" — they are tools you can adopt as your project grows.

The goal is to make robust, responsive applications **accessible**, not complex.

### 4. Extensible Without Being Chaotic

Nextino is intentionally minimal but infinitely extensible:

* It does not include drivers for every sensor or specific business logic.
* Instead, it **provides a composable and structured system** that ensures every new module you add plays by the same rules.

Modules can be added or removed freely, but **they must conform** to the `BaseModule` contract. This is how we achieve freedom through discipline.

### 5. Evolution by Design

Nextino's architecture and build system allow your projects to grow without sacrificing sanity:

* New modules are discovered automatically.
* Configuration is generated, not manually duplicated.
* The framework is designed to be cross-platform, adapting its configuration strategy for different boards (like AVR vs. ESP32).

It is **future-ready** by design — not by retrofit.

---

### Closing Statement

Nextino is not a collection of utilities. It is a declaration of intent for the Arduino ecosystem:

* Modules obey lifecycle rules.
* Time-based operations are managed and non-blocking.
* Configuration is declared and automated.
* The `main.cpp` remains clean, even as the project's complexity grows.

This is not the only path. This is a structured path.

> "We don't seek to replace Arduino's simplicity. We seek to give it a scalable structure."

Nextino is not just how you write sketches.

It's how you **architect** them.
