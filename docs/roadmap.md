---
sidebar_position: 5
title: 'Nextino Roadmap'
---

# 📦 Nextino Roadmap

Nextino is rapidly evolving! This document provides a transparent view of what we've accomplished, what we're working on now, and what exciting features are planned for the future.

---

## ✅ Version 0.2.0: Architecture Solidified

This version established a rock-solid foundation with powerful, decoupled communication and a full suite of core services.

* **Core Services:**
  * `SystemManager`: Central orchestrator for the entire framework lifecycle.
  * `ModuleFactory`: For dynamic, configuration-driven module creation.
  * `Scheduler`: Advanced non-blocking scheduler with task cancellation.
  * `ResourceManager`: Complete hardware conflict prevention for GPIO, I2C, SPI, UART, ADC, and DAC.
  * `EventBus`: For decoupled, one-to-many "publish-subscribe" communication.
  * `ServiceLocator`: For direct, one-to-one "request-response" communication.
* **Build System:**
  * Advanced Python script for automatic module discovery and configuration generation.
* **Documentation:**
  * Official documentation website launched with a full CI/CD pipeline.
  * Comprehensive tutorials for core concepts.
* **Examples:**
  * `01_Blink`: Demonstrates the basic "Plug-and-Play" philosophy.
  * `02_Interactive_Led`: Showcases `EventBus` and `ServiceLocator` in action.

---

## 🚧 Next Up: Version 0.3.0 - The "Connectivity & Scalability" Release

The next major release will focus on making Nextino the best choice for building real-world, multi-component IoT devices and improving the overall developer experience.

### Core Architecture Enhancements

* **Multiple Module Instances:**
  * The configuration system will be upgraded to support an **array of module definitions**. This will allow you to instantiate multiple modules of the same type (e.g., several `RelayModule` instances) from a single, clean configuration file.
  * A new `instance_name` field will be introduced to uniquely identify each module instance for logging, `ServiceLocator`, and MQTT integration.
* **Declarative MQTT Interface:**
  * Module `config.json` files will support a new `mqtt_interface` object. This will allow developers to declaratively define the MQTT topics a module **publishes** to and **subscribes** from.
  * A new build script will parse these definitions to create a centralized "Single Source of Truth" for all MQTT topics, eliminating manual errors and improving maintainability.
* **Config Manager v2:**
  * **Schema Validation:** Define a `schema.json` in your module to automatically validate its configuration.
  * **Overlays:** Support for multiple configuration files (e.g., `config.default.json`, `config.local.json`) that can override default settings.

### Core Services & Modules

* **Command Router (CLI):**
  * An optional core service that allows modules to register commands (e.g., `relay main_light on`).
  * These commands can be invoked via the Serial Monitor, creating a powerful debugging and control interface.
* **`WiFiManager` Module:** A robust, plug-and-play module for handling WiFi connectivity, credentials, and automatic reconnection.
* **`MqttClient` Module:** A powerful MQTT module that will automatically integrate with the new `mqtt_interface` definitions, bridging the `EventBus` to your MQTT broker.

---

## 🔜 Further Down the Road (Post-v0.3.0)

These are features we are planning for the long-term evolution of the framework.

* **Advanced Task Handling:**
  * Exploring async/await patterns or Promise-based tasks for more complex asynchronous operations.
* **Hardware Abstraction Layer (HAL):**
  * Introducing abstract interfaces (`IGpio`, `II2c`) to make modules fully platform-agnostic and easier to test.
* **Unit Testing Overhaul:**
  * Tools and guides for easily writing unit tests for your own modules, including mocking of core services.
* **UI-Friendly Configuration:**
  * A web-based or desktop tool to visually configure your project's `config.json` files.
* **More Official Modules:**
  * Sensors (BME280, DHT22, etc.), Displays (OLED), and Actuators (Servos, Relays).

---

## 🎯 Our Vision

Nextino will grow into a **structured, scalable framework** that bridges the simplicity of Arduino with the power of professional embedded frameworks.

Stay tuned — we are just getting started! 🚀
