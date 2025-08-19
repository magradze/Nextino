/**
 * @file        SystemManager.cpp
 * @title       System Manager Implementation
 * @description Implements the logic for the `SystemManager` class, including
 *              module registration and lifecycle management (begin, loop).
 *
 * @author      Giorgi Magradze
 * @date        2025-08-19
 * @version     0.1.0
 *
 * @copyright   (c) 2025 Nextino. All rights reserved.
 * @license     MIT License
 */

#include "SystemManager.h"
#include "Scheduler.h"
#include "ModuleFactory.h"
#include "Logger.h"
#include <ArduinoJson.h>

SystemManager& SystemManager::getInstance() {
    static SystemManager instance;
    return instance;
}

void SystemManager::registerModule(BaseModule* module) {
    if (module) {
        modules.push_back(module);
        NEXTINO_CORE_LOG(LogLevel::Debug, "SysManager", "Module '%s' registered.", module->getName());
    }
}

void SystemManager::begin() {
    NEXTINO_CORE_LOG(LogLevel::Info, "SysManager", "Initializing all modules...");
    for (auto* module : modules) {
        module->init();
    }
    
    NEXTINO_CORE_LOG(LogLevel::Info, "SysManager", "Starting all modules...");
    for (auto* module : modules) {
        module->start();
    }
}

void SystemManager::loop() {
    Scheduler::getInstance().loop();
    for (auto* module : modules) {
        module->loop();
    }
}