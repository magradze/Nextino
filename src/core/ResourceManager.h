/**
 * @file        ResourceManager.h
 * @title       Hardware Resource Manager
 * @description Defines the `ResourceManager` singleton class, a central gatekeeper
 *              to prevent hardware resource conflicts (e.g., multiple modules
 *              using the same pin).
 *
 * @author      Giorgi Magradze
 * @date        2025-08-21
 * @version     0.1.0
 *
 * @copyright   (c) 2025 Nextino. All rights reserved.
 * @license     MIT License
 */

#pragma once
#include <map>
#include <string>
#include <vector>

/**
 * @enum class ResourceType
 * @brief Enumerates the types of hardware resources managed by the framework.
 */
enum class ResourceType {
    GPIO,
    I2C_ADDRESS,
    SPI_CS_PIN, // SPI is often managed by Chip Select pin
    UART_PORT,
    ADC_PIN,
    DAC_PIN
};

/**
 * @class ResourceManager
 * @brief A singleton class that manages exclusive access to hardware resources.
 * @details The SystemManager uses this class to automatically lock resources
 *          declared in module configurations before initializing the modules.
 *          This prevents runtime conflicts.
 */
class ResourceManager {
public:
    /**
     * @brief Gets the singleton instance of the ResourceManager.
     * @return A reference to the ResourceManager.
     */
    static ResourceManager& getInstance();

    /**
     * @brief Attempts to lock a specific resource for a module.
     * @param type The type of the resource (e.g., ResourceType::GPIO).
     * @param id The unique identifier for the resource (e.g., a pin number or I2C address).
     * @param owner A string identifying the module that is locking the resource.
     * @return True if the resource was locked successfully, false if it's already taken.
     */
    bool lock(ResourceType type, int id, const std::string& owner);

    /**
     * @brief Releases a previously locked resource.
     * @param type The type of the resource to release.
     * @param id The unique identifier of the resource to release.
     */
    void release(ResourceType type, int id);

    /**
     * @brief Checks if a specific resource is currently locked.
     * @param type The type of the resource to check.
     * @param id The unique identifier of the resource to check.
     * @return True if the resource is locked, false otherwise.
     */
    bool isLocked(ResourceType type, int id);

    /**
     * @brief Gets the name of the module that owns a specific resource.
     * @param type The type of the resource.
     * @param id The unique identifier of the resource.
     * @return A string with the owner's name, or an empty string if the resource is not locked.
     */
    std::string getOwner(ResourceType type, int id);

private:
    ResourceManager() {} // Private constructor for singleton

    // A map for each resource type, storing the resource ID and its owner.
    // e.g., _pinRegistry[13] = "LedModule"
    std::map<int, std::string> _gpioRegistry;
    std::map<int, std::string> _i2cAddressRegistry;
    std::map<int, std::string> _spiCsPinRegistry;
    std::map<int, std::string> _uartPortRegistry;
    std::map<int, std::string> _adcPinRegistry;
    std::map<int, std::string> _dacPinRegistry;

    // Helper function to get the correct registry map based on type
    std::map<int, std::string>* getRegistryForType(ResourceType type);
};