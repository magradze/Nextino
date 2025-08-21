/**
 * @file        ResourceManager.cpp
 * @title       ResourceManager Implementation
 * @description Implements the logic for the hardware resource manager.
 *
 * @author      Giorgi Magradze
 * @date        2025-08-21
 * @version     0.1.0
 *
 * @copyright   (c) 2025 Nextino. All rights reserved.
 * @license     MIT License
 */

#include "ResourceManager.h"
#include "Logger.h"

ResourceManager& ResourceManager::getInstance() {
    static ResourceManager* instance = nullptr;
    if (instance == nullptr) {
        instance = new ResourceManager();
    }
    return *instance;
}

// Private helper function to select the correct map based on resource type.
// This makes the public methods much cleaner and avoids code duplication.
std::map<int, std::string>* ResourceManager::getRegistryForType(ResourceType type) {
    switch (type) {
        case ResourceType::GPIO:        return &_gpioRegistry;
        case ResourceType::I2C_ADDRESS: return &_i2cAddressRegistry;
        case ResourceType::SPI_CS_PIN:  return &_spiCsPinRegistry;
        case ResourceType::UART_PORT:   return &_uartPortRegistry;
        case ResourceType::ADC_PIN:     return &_adcPinRegistry;
        case ResourceType::DAC_PIN:     return &_dacPinRegistry;
        default:                        return nullptr;
    }
}

bool ResourceManager::lock(ResourceType type, int id, const std::string& owner) {
    auto registry = getRegistryForType(type);
    if (!registry) {
        NEXTINO_CORE_LOG(LogLevel::Error, "ResManager", "Attempted to lock an unknown resource type.");
        return false;
    }

    // Check if the resource is already in the map (i.e., locked).
    if (registry->count(id)) {
        NEXTINO_CORE_LOG(LogLevel::Error, "ResManager", "RESOURCE CONFLICT! Resource (Type: %d, ID: %d) is already locked by '%s'. Cannot be locked by '%s'.", (int)type, id, (*registry)[id].c_str(), owner.c_str());
        return false;
    }

    // Lock the resource by adding it to the map.
    (*registry)[id] = owner;
    NEXTINO_CORE_LOG(LogLevel::Debug, "ResManager", "Resource (Type: %d, ID: %d) locked successfully by '%s'.", (int)type, id, owner.c_str());
    return true;
}

void ResourceManager::release(ResourceType type, int id) {
    auto registry = getRegistryForType(type);
    if (registry && registry->count(id)) {
        registry->erase(id);
        NEXTINO_CORE_LOG(LogLevel::Debug, "ResManager", "Resource (Type: %d, ID: %d) released.", (int)type, id);
    }
}

bool ResourceManager::isLocked(ResourceType type, int id) {
    auto registry = getRegistryForType(type);
    if (!registry) return false;
    return registry->count(id) > 0;
}

std::string ResourceManager::getOwner(ResourceType type, int id) {
    auto registry = getRegistryForType(type);
    if (registry && registry->count(id)) {
        return (*registry)[id];
    }
    return ""; // Return empty string if not found
}