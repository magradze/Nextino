/**
 * @file        SystemManager.cpp
 * @title       System Manager Implementation
 * @description Implements the logic for the `SystemManager` class, including
 *              module registration, resource management, and lifecycle control.
 *
 * @author      Giorgi Magradze
 * @date        2025-08-22
 * @version     0.3.0
 *
 * @copyright   (c) 2025 Nextino. All rights reserved.
 * @license     MIT License
 */

#include "SystemManager.h"
#include "Scheduler.h"
#include "ModuleFactory.h"
#include "Logger.h"
#include "ResourceManager.h"
#include "modules/BaseModule.h"
#include <ArduinoJson.h>

SystemManager &SystemManager::getInstance()
{
    // Use the modern and thread-safe Meyers' Singleton pattern.
    static SystemManager instance;
    return instance;
}

void SystemManager::registerModule(BaseModule *module)
{
    if (module)
    {
        _modules.push_back(module);
    }
}

void SystemManager::begin(const char *configJson)
{
    Logger::getInstance().logf(LogLevel::Info, true, "SysManager", "System startup sequence initiated.");

    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, configJson);

    if (error)
    {
        Logger::getInstance().logf(LogLevel::Error, true, "SysManager", "Failed to parse JSON config: %s. Halting.", error.c_str());
        _isInErrorState = true;
        return; // Exit begin() gracefully
    }

    // --- PHASE 1: RESOURCE RESERVATION ---
    Logger::getInstance().logf(LogLevel::Info, true, "SysManager", "Phase 1: Locking all declared hardware resources...");
    bool allResourcesLocked = true;
    JsonArray modulesConfig = doc["modules"];

    for (JsonObject moduleConf : modulesConfig)
    {
        const char *moduleType = moduleConf["type"];
        const char *instanceName = moduleConf["instance_name"] | moduleType;
        if (!moduleType)
            continue;

        if (moduleConf["config"].is<JsonObject>())
        {
            JsonObject config = moduleConf["config"];
            if (config["resource"].is<JsonObject>())
            {
                JsonObject resourceObj = config["resource"];
                const char *resourceTypeStr = resourceObj["type"];
                if (!resourceTypeStr)
                    continue;

                if (strcmp(resourceTypeStr, "gpio") == 0 && resourceObj["pin"].is<int>())
                {
                    if (!ResourceManager::getInstance().lock(ResourceType::GPIO, resourceObj["pin"], instanceName))
                    {
                        allResourcesLocked = false;
                    }
                }
                // --- I2C Resource ---
                else if (strcmp(resourceTypeStr, "i2c") == 0 && resourceObj["address"].is<const char *>())
                {
                    // Convert hex string "0x76" to integer
                    int address = strtol(resourceObj["address"], NULL, 16);
                    if (!ResourceManager::getInstance().lock(ResourceType::I2C_ADDRESS, address, instanceName))
                    {
                        allResourcesLocked = false;
                    }
                }
                // --- SPI Resource (by CS Pin) ---
                else if (strcmp(resourceTypeStr, "spi") == 0 && resourceObj["cs_pin"].is<int>())
                {
                    if (!ResourceManager::getInstance().lock(ResourceType::SPI_CS_PIN, resourceObj["cs_pin"], instanceName))
                    {
                        allResourcesLocked = false;
                    }
                }
                // --- UART Resource ---
                else if (strcmp(resourceTypeStr, "uart") == 0 && resourceObj["port"].is<int>())
                {
                    if (!ResourceManager::getInstance().lock(ResourceType::UART_PORT, resourceObj["port"], instanceName))
                    {
                        allResourcesLocked = false;
                    }
                }
                // --- ADC Resource ---
                else if (strcmp(resourceTypeStr, "adc") == 0 && resourceObj["pin"].is<int>())
                {
                    if (!ResourceManager::getInstance().lock(ResourceType::ADC_PIN, resourceObj["pin"], instanceName))
                    {
                        allResourcesLocked = false;
                    }
                }
                // --- DAC Resource ---
                else if (strcmp(resourceTypeStr, "dac") == 0 && resourceObj["pin"].is<int>())
                {
                    if (!ResourceManager::getInstance().lock(ResourceType::DAC_PIN, resourceObj["pin"], instanceName))
                    {
                        allResourcesLocked = false;
                    }
                }
                else
                {
                    // Log a warning if the resource type is unknown or parameters are missing
                    NEXTINO_CORE_LOG(LogLevel::Warn, "SysManager", "Module '%s' has an unknown or malformed resource object. Type: '%s'. Skipping.", instanceName, resourceTypeStr);
                }
            }
        }
    }

    if (!allResourcesLocked)
    {
        Logger::getInstance().logf(LogLevel::Error, true, "SysManager", "RESOURCE CONFLICT DETECTED! System will not start modules.");
        _isInErrorState = true; // Set the error flag
        return;                 // Exit begin() gracefully instead of halting
    }
    Logger::getInstance().logf(LogLevel::Info, true, "SysManager", "All resources locked successfully.");

    // --- PHASE 2: MODULE INSTANTIATION ---
    Logger::getInstance().logf(LogLevel::Info, true, "SysManager", "Phase 2: Creating and registering module instances...");
    for (JsonObject moduleConf : modulesConfig)
    {
        // Get the module type as a C-style string, which is what ArduinoJson provides.
        const char *type = moduleConf["type"];
        if (!type)
        {
            NEXTINO_CORE_LOG(LogLevel::Warn, "SysManager", "Skipping a module config entry with no 'type'.");
            continue;
        }

        // Use 'instance_name' if available, otherwise fall back to the module 'type'.
        const char *instanceName = moduleConf["instance_name"] | type;

        // Pass the config object. Create an empty one if it's missing.
        JsonObject config = moduleConf["config"];
        if (config.isNull())
        {
            NEXTINO_CORE_LOG(LogLevel::Warn, "SysManager", "Module config for '%s' is null or missing. Creating with empty config.", instanceName);
        }

        // Call the updated ModuleFactory method which now accepts const char* for the type.
        // No std::string conversion is needed here anymore.
        BaseModule *module = ModuleFactory::getInstance().createModule(type, instanceName, config);

        if (module)
        {
            registerModule(module);
            NEXTINO_CORE_LOG(LogLevel::Debug, "SysManager", "Module '%s' (%s) created and registered.", instanceName, type);
        }
    }

    // --- PHASE 3: MODULE LIFECYCLE EXECUTION ---
    Logger::getInstance().logf(LogLevel::Info, true, "SysManager", "Phase 3: Initializing all %d modules...", _modules.size());
    for (auto *module : _modules)
    {
        module->init();
    }

    Logger::getInstance().logf(LogLevel::Info, true, "SysManager", "Phase 4: Starting all modules...");
    for (auto *module : _modules)
    {
        module->start();
    }

    // --- PHASE 3.5: COMMAND REGISTRATION ---
    Logger::getInstance().logf(LogLevel::Info, true, "SysManager", "Phase 3.5: Registering all module commands...");
    for (auto *module : _modules)
    {
        module->registerCommands();
    }
}

void SystemManager::loop()
{
    // If a critical error occurred during startup, do nothing in the main loop.
    if (_isInErrorState)
    {
        // Optional: Add visual error indication here, like a fast-blinking LED,
        // without blocking the watchdog timer.
        return;
    }

    Scheduler::getInstance().loop();
    for (auto *module : _modules)
    {
        module->loop();
    }
}