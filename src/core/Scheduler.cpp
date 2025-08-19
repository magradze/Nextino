/**
 * @file        Scheduler.cpp
 * @title       Scheduler Implementation
 * @description Implements the logic for the non-blocking `Scheduler` class.
 *
 * @author      Giorgi Magradze
 * @date        2025-08-19
 * @version     0.1.0
 *
 * @copyright   (c) 2025 Nextino. All rights reserved.
 * @license     MIT License
 */

#include "Scheduler.h"
#include "Logger.h"
#include <Arduino.h>

Scheduler &Scheduler::getInstance()
{
    static Scheduler instance;
    return instance;
}

void Scheduler::scheduleOnce(unsigned long delayMs, TaskCallback callback)
{
    tasks.push_back({delayMs, millis(), callback, false});
}

void Scheduler::scheduleRecurring(unsigned long intervalMs, TaskCallback callback)
{
    tasks.push_back({intervalMs, millis(), callback, true});
}

void Scheduler::loop()
{

    unsigned long now = millis();
    for (auto it = tasks.begin(); it != tasks.end();)
    {
        if (now - it->lastRun >= it->interval)
        {

            NEXTINO_CORE_LOG(LogLevel::Debug, "Scheduler", "Executing a task");

            it->callback();

            if (it->recurring)
            {
                it->lastRun = now;
                ++it;
            }
            else
            {
                it = tasks.erase(it);
            }
        }
        else
        {
            ++it;
        }
    }
}