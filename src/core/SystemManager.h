/**
 * @file        SystemManager.h
 * @title       System Manager
 * @description Defines the `SystemManager` singleton class, which orchestrates the
 *              entire framework lifecycle. It is responsible for registering,
 *              initializing, and running all modules.
 *
 * @author      Giorgi Magradze
 * @date        2025-08-19
 * @version     0.1.0
 *
 * @copyright   (c) 2025 Nextino. All rights reserved.
 * @license     MIT License
 */

#pragma once
#include <vector>
#include "../modules/BaseModule.h"

/**
 * @class SystemManager
 * @brief The central orchestrator of the Nextino framework.
 * @details This singleton class manages the collection of all modules and controls
 *          their lifecycle from initialization to the main operational loop.
 */
class SystemManager {
public:
    /**
     * @brief Gets the singleton instance of the SystemManager.
     * @return A reference to the SystemManager.
     */
    static SystemManager& getInstance();
    
    /**
     * @brief Registers a module with the system.
     * @details This method should be called for each module instance before `begin()` is called.
     *          Typically, this is handled automatically when using a configuration-driven setup.
     * @param module A pointer to the module instance to be registered.
     */
    void registerModule(BaseModule* module);

    /**
     * @brief Initializes and starts all registered modules.
     * @details This method iterates through all registered modules, calling their `init()`
     *          methods first, followed by their `start()` methods. This should be called
     *          at the end of the main `setup()` function.
     */
    void begin();

    /**
     * @brief The main update loop for the entire system.
     * @details This method must be called repeatedly in the main `loop()` function.
     *          It is responsible for calling the `Scheduler::loop()` and the `loop()`
     *          method of every registered module.
     */
    void loop();

private:
    /**
     * @brief Private constructor to enforce the singleton pattern.
     */
    SystemManager() {}

    /**
     * @brief A vector that stores pointers to all registered module instances.
     */
    std::vector<BaseModule*> modules;
};