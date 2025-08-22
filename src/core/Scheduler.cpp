/**
 * @file        Scheduler.cpp
 * @title       Scheduler Implementation
 * @description Implements the logic for the non-blocking `Scheduler` class.
 *
 * @author      Giorgi Magradze
 * @date        2025-08-21
 * @version     0.2.0
 *
 * @copyright   (c) 2025 Nextino. All rights reserved.
 * @license     MIT License
 */

#include "Scheduler.h"
#include "Logger.h"
#include <Arduino.h>
#include <algorithm> // For std::remove_if

/**
 * @brief Gets the singleton instance of the Scheduler.
 * @details Uses the Meyers' Singleton pattern for thread-safe, guaranteed initialization.
 * @return A reference to the Scheduler.
 */
Scheduler &Scheduler::getInstance()
{
    static Scheduler instance;
    return instance;
}

Scheduler::TaskHandle Scheduler::scheduleOnce(unsigned long delayMs, TaskCallback callback)
{
    TaskHandle handle = _nextTaskHandle++;
    _tasks.push_back({handle, delayMs, millis(), callback, false});
    NEXTINO_CORE_LOG(LogLevel::Debug, "Scheduler", "Scheduled one-shot task with handle %u.", handle);
    return handle;
}

Scheduler::TaskHandle Scheduler::scheduleRecurring(unsigned long intervalMs, TaskCallback callback)
{
    TaskHandle handle = _nextTaskHandle++;
    _tasks.push_back({handle, intervalMs, millis(), callback, true});
    NEXTINO_CORE_LOG(LogLevel::Debug, "Scheduler", "Scheduled recurring task with handle %u.", handle);
    return handle;
}

bool Scheduler::cancel(TaskHandle handle)
{
    auto it = std::remove_if(_tasks.begin(), _tasks.end(), [handle](const ScheduledTask &task)
                             { return task.handle == handle; });

    if (it != _tasks.end())
    {
        _tasks.erase(it, _tasks.end());
        NEXTINO_CORE_LOG(LogLevel::Debug, "Scheduler", "Cancelled task with handle %u.", handle);
        return true;
    }

    NEXTINO_CORE_LOG(LogLevel::Warn, "Scheduler", "Could not cancel task: handle %u not found.", handle);
    return false;
}

void Scheduler::loop()
{
    unsigned long now = millis();
    for (auto it = _tasks.begin(); it != _tasks.end(); /* no increment here */)
    {
        if (now - it->lastRun >= it->interval)
        {
            NEXTINO_CORE_LOG(LogLevel::Debug, "Scheduler", "Executing task with handle %u.", it->handle);
            it->callback();

            if (it->recurring)
            {
                it->lastRun = now;
                ++it;
            }
            else
            {
                // Erase the one-shot task and get the iterator to the next element
                it = _tasks.erase(it);
            }
        }
        else
        {
            ++it;
        }
    }
}