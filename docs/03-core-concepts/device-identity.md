---
sidebar_position: 6
title: 'The Device Identity Service'
---

# 🆔 The Device Identity Service

In the world of IoT, every device needs a unique identity. Whether it's for connecting to an MQTT broker, registering with a cloud service, or simply for unique logging, a stable and unique ID is crucial.

Nextino provides a built-in core service called **`DeviceIdentity`** that automatically solves this problem for you. 🕵️‍♂️

---

## 🎯 The Goal: A "Fire and Forget" Unique ID

The `DeviceIdentity` service is designed to be incredibly simple to use, yet powerful under the hood. It provides a single, consistent way to get a unique identifier for your device, regardless of the underlying hardware.

You simply call `NextinoIdentity().getUniqueId()` anywhere in your code, and you get back a unique, null-terminated string.

```cpp
const char* myId = NextinoIdentity().getUniqueId();
// myId might be "nextino-esp32-aabbccddeeff"
```

## 🧠 The Multi-Tiered Strategy

How does Nextino guarantee a unique and persistent ID across different microcontrollers? It uses a multi-layered, "best-source-first" strategy:

### 🥇 Priority #1: Hardware Unique ID

If the microcontroller has a factory-burned, globally unique identifier, the service will **always** use it. This is the most reliable method.

* **ESP32 / ESP8266:** The device's **MAC address** is used.
* **STM32 (Future Support):** The 96-bit unique chip ID will be used.

**Example:** `nextino-esp32-aabbccddeeff`

### 🥈 Priority #2: Persistent Generated ID

What if the board doesn't have a hardware ID (like an Arduino Uno or Nano)? The `DeviceIdentity` service has a clever fallback plan.

1. **On the very first boot**, it checks a special location in non-volatile memory (like EEPROM).
2. If it's empty, the service **generates a new, pseudo-random ID**. It uses electrical noise from an unconnected analog pin as a "seed" to create a high-quality random number.
3. This newly created ID is then **saved permanently** to the EEPROM.
4. On all subsequent boots, the service reads this saved ID from the EEPROM.

**Result:** Your Arduino Uno now has a persistent unique ID that will survive power cycles and re-flashes! 🤯

**Example:** `nextino-avr-12c3e4a5b6d7`

### 🥉 Priority #3: Volatile ID (Last Resort)

In the rare case that a board has neither a hardware ID nor non-volatile memory, the service will generate a new random ID on every boot. This is not ideal, as the ID is not persistent, but it ensures the device still has an identifier to work with.

---

## 🛠️ How to Use It

The service is available globally via a convenient helper function.

```cpp
#include <Nextino.h>

void MyModule::init() {
    const char* myId = NextinoIdentity().getUniqueId();
    
    // Use it as a hostname
    WiFi.setHostname(myId);

    // Use it for an MQTT client ID
    mqttClient.connect(myId);

    NEXTINO_LOGI(getInstanceName(), "My unique ID is %s", myId);
}
```

The `DeviceIdentity` service is a cornerstone of Nextino, making your devices ready for real-world network and cloud integration right out of the box.
