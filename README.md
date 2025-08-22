<p align="center">
  <img src="assets/NEXT.ino.png" alt="Nextino Logo" width="200"/>
</p>

# Nextino Framework

**Nextino: The Next Step for Arduino. A structured, scalable, and modern C++ framework for Arduino, ESP32, STM32, and beyond.**

[![PlatformIO CI](https://github.com/magradze/Nextino/actions/workflows/build.yml/badge.svg)](https://github.com/magradze/Nextino/actions)
[![Website](https://img.shields.io/badge/Docs-Website-blue.svg)](https://magradze.github.io/Nextino/)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

---

Tired of messy `.ino` files and endless `loop()` functions? Nextino brings the power of modern software architecture to the embedded world, allowing you to build complex, maintainable, and reusable projects with ease.

This framework is built for developers who love the simplicity of the Arduino ecosystem but need the structure and power of a professional IoT framework.

---

## ✨ Key Features

* **🧩 Modular Architecture:** Build your application from independent, reusable modules (`BaseModule`).
* **🔌 Plug-and-Play:** Add modules to your project's `lib` folder, and Nextino's build script **automatically discovers and configures them**. Your `main.cpp` stays clean!
* **📜 Configuration-Driven:** Define your system's components and their parameters in `config.json` files. Change hardware without changing your application code.
* **💬 Decoupled Communication:** Modules communicate safely using the built-in **`EventBus`** (for broadcasting) and **`ServiceLocator`** (for direct requests), eliminating tight coupling.
* **⏱️ Advanced Scheduler:** Run periodic or one-shot tasks efficiently without using `delay()`. Tasks can be **dynamically cancelled** at runtime.
* **🛡️ Hardware Safety:** The `ResourceManager` prevents hardware conflicts (e.g., two modules using the same pin) by locking resources like **GPIO, I2C, SPI, and UART** at startup.
* **📝 Professional Logging:** A built-in, colored, and leveled logger that makes debugging a pleasure.

---

## 📚 Documentation

Our comprehensive documentation is the best place to start your journey with Nextino. It includes tutorials, core concept guides, and API references.

### ➡️ [Visit the Official Documentation Website](https://magradze.github.io/Nextino/)

---

## 🚀 Getting Started

Getting started with Nextino is a simple, three-step process:

**1. Visit the Documentation:**
Start with our **[Your First Project](https://magradze.github.io/Nextino/getting-started/your-first-project)** tutorial on the documentation site.

**2. Add Nextino to Your Project:**
In your `platformio.ini` file, add the Nextino repository to your `lib_deps`:

```ini
lib_deps =
    https://github.com/magradze/Nextino.git
```

**3. Explore the Examples:**
Check out the official **[examples directory](https://github.com/magradze/Nextino/tree/main/examples)** to see Nextino in action.

---

## ❓ Why Nextino?

Arduino made embedded development accessible, but as projects grow, spaghetti `loop()` logic becomes unmanageable. Nextino solves this by providing:

* A **clear structure** for scalable applications.
* **Automatic module discovery**, so you focus on logic, not boilerplate.
* A **professional development experience** while staying compatible with the Arduino ecosystem.
* A path to migrate from hobby projects to production-ready IoT solutions.

Think of it as **Next.js for Arduino**: the same ecosystem, but modernized, structured, and future-proof.

---

## 📦 Roadmap

Curious about what's next for Nextino? We maintain a public roadmap to keep the community informed about our plans and priorities.

➡️ **[View the Project Roadmap](https://github.com/magradze/Nextino/blob/main/roadmap.md)**

---

## 🎉 The Story Behind Nextino's First Release

Nextino's first public release (`v0.1.0`) coincided with a very special day for me — **my son's birthday**. This isn't just a technical milestone; it's a personal one.

On that day, I watched my child grow one year older while also giving life to a framework that I hope will empower countless developers. Just like my son, Nextino is **on a journey of growth**, full of potential and possibilities. This story is a reminder that creation, in any form, is always meaningful.

> "Two birthdays on the same day: one of my son, one of my creation. Both equally precious." 🎈

---

## 🤝 Contributing

Contributions are welcome! Please read our **[Contributing Guide](CONTRIBUTING.md)** to get started.

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
