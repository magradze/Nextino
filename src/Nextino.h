/**
 * @file        Nextino.h
 * @title       Nextino Framework Public API
 * @description The main public header and entry point for the Nextino framework.
 *              This file includes all necessary core components and provides
 *              convenient helper functions for accessing framework singletons.
 *
 * @author      Giorgi Magradze
 * @date        2025-08-19
 * @version     0.1.0
 *
 * @copyright   (c) 2025 Nextino. All rights reserved.
 * @license     MIT License
 */

#pragma once

// --- Public Contracts ---
#include "modules/BaseModule.h"

// --- Core Singletons (Internal Headers) ---
#include "core/SystemManager.h"
#include "core/Scheduler.h"
#include "core/Logger.h"
#include "core/ModuleFactory.h"
#include "core/ResourceManager.h"

/**
 * @brief Provides access to the global SystemManager instance.
 * @return A reference to the SystemManager singleton.
 */
inline SystemManager& NextinoSystem() { return SystemManager::getInstance(); }

/**
 * @brief Provides access to the global Scheduler instance.
 * @return A reference to the Scheduler singleton.
 */
inline Scheduler& NextinoScheduler() { return Scheduler::getInstance(); }

/**
 * @brief Provides access to the global ModuleFactory instance.
 * @return A reference to the ModuleFactory singleton.
 */
inline ModuleFactory &NextinoFactory() { return ModuleFactory::getInstance(); }

/**
 * @brief Provides access to the global ResourceManager instance.
 * @return A reference to the ResourceManager singleton.
 */
inline ResourceManager &NextinoResource() { return ResourceManager::getInstance(); }
