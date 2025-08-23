/**
 * @file        ModuleFactory.h
 * @title       Module Factory
 * @description Defines the `ModuleFactory` singleton class, responsible for
 *              dynamically creating module instances based on a string type
 *              and a JSON configuration.
 *
 * @author      Giorgi Magradze
 * @date        2025-08-19
 * @version     0.1.0
 *
 * @copyright   (c) 2025 Nextino. All rights reserved.
 * @license     MIT License
 */

#pragma once
#include "../modules/BaseModule.h"
#include <string>
#include <map>
#include <functional>
#include <ArduinoJson.h>

/**
 * @typedef ModuleCreationFunction
 * @brief A function pointer type for a factory function that creates a module instance.
 * @param instanceName The unique instance name for this module, read from the config.
 * @param config A constant reference to a `JsonObject` containing the specific
 *               configuration for the module to be created.
 * @return A pointer to the newly created `BaseModule` instance.
 */
using ModuleCreationFunction = std::function<BaseModule *(const char *instanceName, const JsonObject &)>;

/**
 * @class ModuleFactory
 * @brief A singleton class that implements the Factory Method design pattern for modules.
 * @details It maintains a registry of module types and their corresponding creation
 *          functions, allowing the `SystemManager` to instantiate modules dynamically
 *          from a configuration file.
 */
class ModuleFactory {
public:
    /**
     * @brief Gets the singleton instance of the ModuleFactory.
     * @return A reference to the ModuleFactory.
     */
    static ModuleFactory& getInstance();

    /**
     * @brief Registers a new module type with its creation function.
     * @details This method should be called in the `registerAllModuleTypes()` function
     *          for each available module in the project.
     * @param type The string identifier for the module type (e.g., "LedModule").
     * @param func The factory function that can create an instance of this module.
     */
    void registerModule(const char *type, ModuleCreationFunction func);

    /**
     * @brief Creates an instance of a module given its type, instance name, and configuration.
     * @param type The string identifier of the module to create (e.g., "LedModule").
     * @param instanceName The unique name for this specific instance (e.g., "main_light").
     * @param config The `JsonObject` containing the configuration for this specific instance.
     * @return A pointer to the newly created `BaseModule`, or `nullptr` if the type is unknown.
     */
    BaseModule *createModule(const char *type, const char *instanceName, const JsonObject &config);

private:
    /**
     * @brief Private constructor to enforce the singleton pattern.
     */
    ModuleFactory() {}

    /**
     * @brief A map that stores the creation function for each registered module type.
     */
    std::map<std::string, ModuleCreationFunction> registry;
};