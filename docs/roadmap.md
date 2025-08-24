---
sidebar_position: 5
title: 'Nextino Roadmap'
---

# 📦 Nextino Roadmap

Nextino is rapidly evolving! This document provides a transparent view of what we've accomplished, what we're working on now, and what exciting features are planned for the future.

---

## ✅ Version 0.3.0: Connectivity & Scalability

This version transformed Nextino into a true IoT framework, adding foundational features for building complex, scalable, and connected devices.

* **Core Architecture - Multiple Instances:**
  * The framework now supports creating **multiple instances** of the same module type from an array-based configuration.
  * A unique `instance_name` is now used for precise logging, resource management, and service location.
* **Core Services:**
  * **`DeviceIdentity`**: A new multi-platform service provides a persistent, unique ID for each device.
  * **`CommandRouter`**: A new service acts as a central engine for text-based commands, allowing control from any source (Serial, MQTT, etc.).
* **Connectivity Foundation:**
  * **Declarative MQTT Interface:** The build system now parses `mqtt_interface` definitions from `config.json` files to create a "Single Source of Truth" for all MQTT topics (`generated_mqtt.h`).
  * **(Proof of Concept) Connectivity Modules:** Basic, functional `NextinoWifi` (wrapping WiFiManager) and `MqttClientModule` modules have been developed to prove the new architecture.

## ✅ Version 0.2.0: Architecture Solidified

This version established the rock-solid foundation for communication and core services.

* **Core Services:** `SystemManager`, `ModuleFactory`, `Scheduler` (with cancellation), `ResourceManager` (expanded), `EventBus`, and `ServiceLocator`.
* **Build System:** Python script for automatic module discovery.
* **Documentation:** Official Docusaurus website with CI/CD.

---

## 🚧 Next Up: Version 0.4.0 - The "Polish & Production" Release

The next release will focus on refining the existing features, improving stability, and providing the tools needed for production-ready applications.

### Core Features

* **Full Declarative MQTT Integration:**
  * The `MqttClientModule` will be enhanced to **fully automate** subscriptions and message routing based on the `mqtt_interface` definitions, removing the need for manual `if/else` logic in the callback.
* **Config Manager v2:**
  * **Schema Validation:** Define a `schema.json` in your module to automatically validate its `config.json` against a ruleset.
  * **Configuration Overlays:** Support for multiple config files (e.g., `config.default.json`, `config.local.json`) to override default settings for different environments.
* **Unit Testing Framework:**
  * Provide official guides and tools for writing unit tests for your own modules, including mocking of Nextino's core services.

### Official Modules

* **Refine `NextinoWifi` & `MqttClient`:** Turn the proof-of-concept modules into robust, feature-complete official modules.
* **Persistent Storage Module:** A standard module for easy key-value storage in EEPROM or LittleFS.
* **First Sensor Module:** Release the first official sensor module (e.g., a BME280 module) as a template for the community.

---

## 🔜 Further Down the Road (Post-v0.4.0)

* **Hardware Abstraction Layer (HAL):** To make modules fully platform-agnostic.
* **Advanced Task Handling:** Exploring `async/await` patterns.
* **Visual Configuration Tools:** A UI-based tool for editing `config.json` files.

---

## 🎯 Our Vision

Nextino will grow into a **structured, scalable framework** that bridges the simplicity of Arduino with the power of professional embedded frameworks.

Stay tuned — we are just getting started! 🚀
