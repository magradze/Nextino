/**
 * @file        ServiceLocator.h
 * @title       Service Locator
 * @description Defines the `ServiceLocator` singleton class, which provides a
 *              central registry for modules to offer and discover services.
 *              This enables direct, yet decoupled, one-to-one communication.
 *
 * @author      Giorgi Magradze
 * @date        2025-08-21
 * @version     0.1.0
 *
 * @copyright   (c) 2025 Nextino. All rights reserved.
 * @license     MIT License
 */

#pragma once
#include <map>
#include <string>

/**
 * @class ServiceLocator
 * @brief A singleton class that implements the Service Locator design pattern.
 * @details Modules can register themselves as a "service" under a unique name.
 *          Other modules can then "get" a pointer to that service by its name,
 *          allowing them to call its public methods.
 */
class ServiceLocator {
public:
    /**
     * @brief Gets the singleton instance of the ServiceLocator.
     * @return A reference to the ServiceLocator.
     */
    static ServiceLocator& getInstance();

    /**
     * @brief Registers a service (typically a module instance) with a unique name.
     * @tparam T The type of the service being provided.
     * @param name The unique string name to register the service under (e.g., "led_service").
     * @param service A pointer to the service instance.
     */
    template<typename T>
    void provide(const std::string& name, T* service) {
        // We store the pointer as void* to hold any type.
        _services[name] = static_cast<void*>(service);
    }

    /**
     * @brief Retrieves a service by its registered name.
     * @tparam T The expected type of the service.
     * @param name The unique name of the service to retrieve.
     * @return A pointer to the service of the requested type, or `nullptr` if not found.
     *         The caller is responsible for checking if the returned pointer is null.
     */
    template<typename T>
    T* get(const std::string& name) {
        if (_services.find(name) == _services.end()) {
            // Service not found
            return nullptr;
        }
        // We cast the stored void* back to the requested type.
        return static_cast<T*>(_services[name]);
    }

private:
    /**
     * @brief Private constructor to enforce the singleton pattern.
     */
    ServiceLocator() {}

    // Delete copy constructor and assignment operator to prevent copies
    ServiceLocator(const ServiceLocator&) = delete;
    void operator=(const ServiceLocator&) = delete;

    /**
     * @brief A map that stores a void pointer to a service for each registered name.
     */
    std::map<std::string, void*> _services;
};