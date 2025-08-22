/**
 * @file        ModuleFactory.cpp
 * @title       Module Factory Implementation
 * @description Implements the logic for the `ModuleFactory` class, including
 *              registering and creating module types.
 *
 * @author      Giorgi Magradze
 * @date        2025-08-19
 * @version     0.1.0
 *
 * @copyright   (c) 2025 Nextino. All rights reserved.
 * @license     MIT License
 */

#include "ModuleFactory.h"
#include "Logger.h"

// Implementation of the getInstance() method
ModuleFactory& ModuleFactory::getInstance() {
    static ModuleFactory instance;
    return instance;
}

// Registers a new module type and its creation function
void ModuleFactory::registerModule(const std::string& type, ModuleCreationFunction func) {
    NEXTINO_CORE_LOG(LogLevel::Debug, "ModFactory", "Registering type '%s'", type.c_str());
    registry[type] = func;
}

// Creates a module object based on type and configuration
BaseModule* ModuleFactory::createModule(const std::string& type, const JsonObject& config) {
    // Check if the type is registered
    if (registry.find(type) != registry.end()) {
        NEXTINO_CORE_LOG(LogLevel::Debug, "ModFactory", "Creating module of type '%s'...", type.c_str());
        // Call the stored function and pass the configuration
        return registry[type](config);
    }

    NEXTINO_CORE_LOG(LogLevel::Error, "ModFactory", "Unknown module type '%s'", type.c_str());
    return nullptr; // If the type is not found, return nullptr
}