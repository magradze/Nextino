/**
 * @file        EventBus.h
 * @title       Asynchronous Event Bus
 * @description Defines the `EventBus` singleton class, which provides a centralized
 *              publish-subscribe mechanism for decoupled, asynchronous communication
 *              between modules.
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
#include <vector>
#include <functional>
#include <string>

/**
 * @class EventBus
 * @brief A singleton class for managing and dispatching events.
 * @details Modules can subscribe to named events and publish events to notify
 *          other parts of the system without direct dependencies. This class
 *          is not thread-safe and is intended for single-threaded environments
 *          or to be called from the main `loop()`.
 */
class EventBus {
public:
    /**
     * @brief Gets the singleton instance of the EventBus.
     * @return A reference to the EventBus.
     */
    static EventBus& getInstance();

    /**
     * @typedef EventCallback
     * @brief A function type for event listeners.
     * @param payload A void pointer to the event's data payload. The callback
     *                is responsible for casting it to the correct type.
     */
    using EventCallback = std::function<void(void*)>;

    /**
     * @brief Subscribes a callback function to a specific event.
     * @param eventName The name of the event to listen for (e.g., "button_pressed").
     * @param callback The function to be executed when the event is posted.
     */
    void on(const std::string& eventName, EventCallback callback);

    /**
     * @brief Publishes (posts) an event to all subscribed listeners.
     * @details This method immediately calls all registered callbacks for the given event.
     * @param eventName The name of the event to publish.
     * @param payload (Optional) A void pointer to data to be passed to the listeners.
     *                Defaults to nullptr if no data is needed.
     */
    void post(const std::string& eventName, void* payload = nullptr);

private:
    /**
     * @brief Private constructor to enforce the singleton pattern.
     */
    EventBus() {}
    
    // Delete copy constructor and assignment operator to prevent copies
    EventBus(const EventBus&) = delete;
    void operator=(const EventBus&) = delete;

    /**
     * @brief A map that stores a vector of callbacks for each event name.
     */
    std::map<std::string, std::vector<EventCallback>> _listeners;
};