/**
 * @file        EventBus.cpp
 * @title       EventBus Implementation
 * @description Implements the logic for the EventBus class.
 *
 * @author      Giorgi Magradze
 * @date        2025-08-21
 * @version     0.1.0
 *
 * @copyright   (c) 2025 Nextino. All rights reserved.
 * @license     MIT License
 */

#include "EventBus.h"
#include "Logger.h" // For internal logging

/**
* @brief Gets the singleton instance of the EventBus.
* @details Uses the Meyers' Singleton pattern for thread-safe, guaranteed initialization.
* @return A reference to the EventBus.
*/
EventBus &EventBus::getInstance() {
    static EventBus instance;
    return instance;
}

void EventBus::on(const std::string& eventName, EventCallback callback) {
    // Add the callback to the vector for the given event name.
    _listeners[eventName].push_back(callback);
    NEXTINO_CORE_LOG(LogLevel::Debug, "EventBus", "New listener subscribed to event '%s'.", eventName.c_str());
}

void EventBus::post(const std::string& eventName, void* payload) {
    NEXTINO_CORE_LOG(LogLevel::Debug, "EventBus", "Posting event '%s'.", eventName.c_str());

    // Check if any listeners are registered for this event.
    if (_listeners.find(eventName) != _listeners.end()) {
        // If so, iterate through all of them and call the callbacks.
        for (auto const& callback : _listeners[eventName]) {
            callback(payload);
        }
    }
}