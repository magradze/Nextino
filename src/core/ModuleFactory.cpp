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

// Registers a new module type. Accepts a const char* for easier use.
void ModuleFactory::registerModule(const char *type, ModuleCreationFunction func)
{
    NEXTINO_CORE_LOG(LogLevel::Debug, "ModFactory", "Registering type '%s'", type);
    // Convert to std::string for storing in the map
    registry[std::string(type)] = func;
}

// Creates a module object. Accepts a const char* for the type.
BaseModule *ModuleFactory::createModule(const char *type, const char *instanceName, const JsonObject &config)
{
    // Convert to std::string for map lookup
    std::string type_str(type);

    // Check if the type is registered
    if (registry.find(type_str) != registry.end())
    {
        NEXTINO_CORE_LOG(LogLevel::Debug, "ModFactory", "Creating module instance '%s' of type '%s'...", instanceName, type);
        // Call the stored creation function, passing both instanceName and config
        return registry[type_str](instanceName, config);
    }

    NEXTINO_CORE_LOG(LogLevel::Error, "ModFactory", "Unknown module type '%s'", type);
    return nullptr; // If the type is not found, return nullptr
}