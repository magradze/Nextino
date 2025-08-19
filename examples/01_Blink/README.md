# Example: 01 - Plug-and-Play Blink

This example demonstrates the core Plug-and-Play functionality of the Nextino framework.

## How it Works

1. The `platformio.ini` file lists the `Nextino` framework and the local `LedFlasher` module as dependencies.
2. The `LedFlasher` library contains a `config.json` file that defines the LED's pin.
3. Before compilation, a Python script (`bootstrap.py` from the Nextino library) runs automatically. It finds the `LedFlasher` module, reads its `config.json`, and generates an `include/generated_config.h` file.
4. The `main.cpp` file is minimal. It includes the auto-generated header and calls `NextinoSystem().begin()`.
5. The `SystemManager` then reads the configuration from the generated header, creates the `LedModule` instance using the `ModuleFactory`, and starts its blinking task via the `Scheduler`.

The `main.cpp` file has no direct knowledge of the `LedModule`. You can add or remove modules from the `lib` folder, and the project will adapt automatically.
