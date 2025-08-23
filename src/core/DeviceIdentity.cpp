/**
 * @file        DeviceIdentity.cpp
 * @title       Device Identity Service Implementation
 * @description Implements the logic for generating a unique device ID across
 *              different platforms (ESP32, ESP8266, and a fallback for AVR).
 */

#include "DeviceIdentity.h"
#include <stdio.h>

// Platform-specific includes
#if defined(ESP32)
    #include <esp_mac.h>
#elif defined(ESP8266)
    #include <ESP8266WiFi.h>
#elif defined(ARDUINO_ARCH_AVR)
    #include <EEPROM.h>
    #include <Arduino.h> // For randomSeed() and analogRead()
#endif

// Define a "magic number" and EEPROM address for AVR-based persistent ID.
// This helps us verify if a valid ID has been stored previously.
#define EEPROM_MAGIC_NUMBER 0xDEADBEEF
#define EEPROM_ADDR_MAGIC   0
#define EEPROM_ADDR_ID      (EEPROM_ADDR_MAGIC + sizeof(uint32_t))
#define UNIQUE_ID_LENGTH    6 // 6 bytes for a MAC-like ID

DeviceIdentity& DeviceIdentity::getInstance() {
    static DeviceIdentity instance;
    return instance;
}

const char* DeviceIdentity::getUniqueId() {
    if (!_isIdGenerated) {
        generateId();
        _isIdGenerated = true;
    }
    return _buffer;
}

void DeviceIdentity::generateId() {
    uint8_t idBytes[UNIQUE_ID_LENGTH] = {0};
    const char* platform = "unknown";

#if defined(ESP32)
    platform = "esp32";
    esp_read_mac(idBytes, ESP_MAC_WIFI_STA);

#elif defined(ESP8266)
    platform = "esp8266";
    WiFi.macAddress(idBytes);

#elif defined(ARDUINO_ARCH_AVR)
    platform = "avr";
    uint32_t magic;
    EEPROM.get(EEPROM_ADDR_MAGIC, magic);

    if (magic == EEPROM_MAGIC_NUMBER) {
        // A valid ID already exists, read it.
        for (int i = 0; i < UNIQUE_ID_LENGTH; ++i) {
            idBytes[i] = EEPROM.read(EEPROM_ADDR_ID + i);
        }
    } else {
        // No valid ID found, let's generate and save a new one.
        // Use a floating analog pin for a pseudo-random seed.
        // Make sure A0 is not connected to anything.
        randomSeed(analogRead(A0));

        for (int i = 0; i < UNIQUE_ID_LENGTH; ++i) {
            idBytes[i] = random(0, 256);
        }
        
        // Save the new ID and magic number to EEPROM.
        EEPROM.put(EEPROM_ADDR_MAGIC, (uint32_t)EEPROM_MAGIC_NUMBER);
        for (int i = 0; i < UNIQUE_ID_LENGTH; ++i) {
            EEPROM.write(EEPROM_ADDR_ID + i, idBytes[i]);
        }
    }

#else
    // Fallback for any other unsupported platform
    // This part will use a non-persistent random ID for now.
    // Ideally, specific implementations should be added for other platforms like STM32.
    randomSeed(millis()); // Not a great seed, but better than nothing
    for (int i = 0; i < UNIQUE_ID_LENGTH; ++i) {
        idBytes[i] = random(0, 256);
    }
#endif

    // Format the final ID string
    snprintf(_buffer, sizeof(_buffer), "nextino-%s-%02x%02x%02x%02x%02x%02x",
             platform,
             idBytes[0], idBytes[1], idBytes[2],
             idBytes[3], idBytes[4], idBytes[5]);
}