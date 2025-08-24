/**
 * @file        DeviceIdentity.h
 * @title       Device Identity Service
 * @description Defines the `DeviceIdentity` singleton, a core service for
 *              providing a consistent and unique identifier for the device.
 *
 * @author      Giorgi Magradze
 * @date        2025-08-24
 * @version     0.3.0
 */

#pragma once

class DeviceIdentity {
public:
    /**
     * @brief Gets the singleton instance of the DeviceIdentity service.
     * @return A reference to the DeviceIdentity instance.
     */
    static DeviceIdentity& getInstance();

    /**
     * @brief Gets a unique identifier for this physical device.
     * @details This method provides a persistent and reasonably unique ID based on
     *          the best available source for the current platform:
     *          1. Hardware MAC Address (for ESP32, ESP8266).
     *          2. MCU's unique chip ID (future support for STM32, etc.).
     *          3. A randomly generated ID stored in non-volatile memory (e.g., EEPROM)
     *             for platforms without a hardware ID (like AVR).
     * @return A null-terminated C-string (e.g., "nextino-esp32-aabbccddeeff").
     *         The buffer is managed internally by the class.
     */
    const char* getUniqueId();

private:
    /**
     * @brief Private constructor to enforce the singleton pattern.
     */
    DeviceIdentity() : _isIdGenerated(false) {}

    // Delete copy constructor and assignment operator.
    DeviceIdentity(const DeviceIdentity&) = delete;
    void operator=(const DeviceIdentity&) = delete;

    /**
     * @brief Internal helper to generate the ID on the first call.
     */
    void generateId();

    bool _isIdGenerated;
    char _buffer[40]; // Buffer to hold the generated ID string (e.g., "nextino-platform-mac")
};