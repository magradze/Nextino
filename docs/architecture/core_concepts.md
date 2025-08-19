# Nextino Core Concepts

Nextino is designed around a few key architectural principles to promote clean, scalable, and maintainable code.

### 1. Everything is a Module

The fundamental building block in Nextino is the **Module**. A module is a self-contained, independent unit of functionality. Whether it's reading a sensor, controlling a motor, or managing a network connection, it should be implemented as a class that inherits from `BaseModule`.

### 2. Configuration-Driven Design

Instead of hard-coding hardware pins or behavior in your `main.cpp`, you define your system's structure in `config.json` files. Each module has its own `config.json` with default settings. The framework's build script automatically finds these files, combines them, and makes them available to the system at runtime.

This separates **what** the system does (configuration) from **how** it does it (code).

### 3. The System Manages Itself

The `SystemManager` is the heart of the framework. You don't need to create module objects manually in your `main.cpp`. The `SystemManager` does this for you:

1. It reads the combined project configuration.
2. It uses the `ModuleFactory` to create instances of each module listed in the configuration.
3. It manages the entire lifecycle of these modules, calling their `init()`, `start()`, and `loop()` methods at the appropriate times.

### 4. Non-Blocking by Default

The built-in `Scheduler` encourages a non-blocking programming model. Instead of using `delay()`, you schedule tasks to run periodically. This ensures your application remains responsive and can handle multiple tasks cooperatively.
