---
sidebar_position: 1
title: 'What is Nextino?'
---

# 🚀 What is Nextino?

Welcome to the future of embedded development!

**Nextino** is a structured, scalable, and modern C++ framework designed to bring architectural discipline and professional development practices to the Arduino ecosystem. It's built for developers who love the simplicity of Arduino but crave the power and organization of a high-level framework.

---

## 🤯 The Problem: "Sketch" vs. "System"

The traditional Arduino workflow is fantastic for getting started. A `setup()` and a `loop()` are all you need to make an LED blink. But what happens when your project grows?

* Your `loop()` function becomes a tangled mess of `if` statements and `delay()` calls.
* Code for different sensors and actuators gets mixed together, making it hard to read and debug.
* Reusing code in your next project means tedious copy-pasting.
* Your simple "sketch" has become a complex, fragile "spaghetti" system.

This is the problem Nextino solves.

---

## ✨ The Solution: A Framework for Grown-Up Projects

Nextino provides a solid architectural foundation, allowing you to build your application from independent, reusable **Modules**.

| Feature                  | How Nextino Solves It                                                                                                                            |
| ------------------------ | ------------------------------------------------------------------------------------------------------------------------------------------------ |
| **🧩 Modular Architecture** | Every piece of functionality (a sensor, a display, a network client) is a self-contained `BaseModule`. No more spaghetti code.                 |
| **🔌 Plug-and-Play**       | Add a module library to your project, and Nextino's build script **automatically discovers and configures it**. Your `main.cpp` stays minimal. |
| **📜 Config-Driven**      | Define your system's components and their parameters in `config.json` files. Change hardware without changing your application code.           |
| **⏱️ Non-Blocking Core**   | The built-in `Scheduler` lets you run tasks at any interval without a single `delay()`, keeping your application responsive.                 |
| **🎨 Professional Tools** | A powerful, colored `Logger` and a suite of core components (`ModuleFactory`, `ResourceManager`) give you the tools to build robust systems. |

Think of it as **Next.js for Arduino**: the same familiar ecosystem, but modernized, structured, and ready to scale.

---

## 🎯 Who is Nextino For?

* **Hobbyists** whose projects are outgrowing the simple `.ino` file.
* **Students** learning about software architecture and design patterns in an embedded context.
* **Professionals** who need to rapidly prototype production-ready IoT devices.
* **Anyone** who believes that writing embedded code should be organized, scalable, and fun.

---

### Next Steps

Ready to see it in action? Let's move on to the next step:

➡️ **[Installation](./installation)**
