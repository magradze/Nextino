/**
 * @file        ServiceLocator.cpp
 * @title       ServiceLocator Implementation
 * @description Implements the singleton access method for the ServiceLocator.
 *              The main logic is template-based and resides in the header file.
 *
 * @author      Giorgi Magradze
 * @date        2025-08-21
 * @version     0.1.0
 *
 * @copyright   (c) 2025 Nextino. All rights reserved.
 * @license     MIT License
 */

#include "ServiceLocator.h"

/**
 * @brief Gets the singleton instance of the ServiceLocator.
 * @details Uses the Meyers' Singleton pattern for thread-safe, guaranteed initialization.
 * @return A reference to the ServiceLocator.
 */
ServiceLocator &ServiceLocator::getInstance()
{
    static ServiceLocator instance;
    return instance;
}