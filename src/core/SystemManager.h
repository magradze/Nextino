/**
 * @file        SystemManager.h
 * @title       System Manager
 * @description Defines the `SystemManager` singleton class, which orchestrates the
 *              entire framework lifecycle. It is responsible for registering,
 *              initializing, and running all modules.
 *
 * @author      Giorgi Magradze
 * @date        2025-08-22
 * @version     0.3.0
 *
 * @copyright   (c) 2025 Nextino. All rights reserved.
 * @license     MIT License
 */

#pragma once
#include <vector>

// Forward declaration of BaseModule to avoid circular dependencies.
class BaseModule;

/**
 * @class SystemManager
 * @brief The central orchestrator of the Nextino framework.
 * @details This singleton class manages the collection of all modules and controls
 *          their lifecycle from initialization to the main operational loop.
 */
class SystemManager
{
public:
    /**
     * @brief Gets the singleton instance of the SystemManager.
     * @return A reference to the SystemManager.
     */
    static SystemManager &getInstance();

    /**
     * @brief Registers a module with the system.
     * @param module A pointer to the module instance to be registered.
     */
    void registerModule(BaseModule *module);

    /**
     * @brief Initializes and starts all modules based on the provided configuration.
     * @details This method performs a multi-phase startup. If a critical error
     *          (like a resource conflict) is detected, it will log the error
     *          and enter a safe, non-operational state instead of halting.
     * @param configJson A constant C-string containing the project's configuration.
     */
    void begin(const char *configJson);

    /**
     * @brief The main update loop for the entire system.
     * @details Must be called repeatedly in the main `loop()` function.
     */
    void loop();

private:
    /**
     * @brief Private constructor to enforce the singleton pattern.
     */
    SystemManager() : _isInErrorState(false) {}

    std::vector<BaseModule *> _modules;
    bool _isInErrorState; // Flag to indicate a critical startup failure.
};