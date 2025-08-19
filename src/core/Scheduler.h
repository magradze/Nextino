/**
 * @file        Scheduler.h
 * @title       Non-Blocking Task Scheduler
 * @description Defines the `Scheduler` singleton class, providing a non-blocking,
 *              millis-based task scheduler for periodic and one-shot operations.
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
#include <functional>

/**
 * @class Scheduler
 * @brief A singleton class for managing non-blocking, time-based tasks.
 * @details This scheduler avoids the use of `delay()` by checking `millis()` on
 *          every program loop, allowing for cooperative multitasking.
 */
class Scheduler {
public:
    /**
     * @brief Gets the singleton instance of the Scheduler.
     * @return A reference to the Scheduler.
     */
    static Scheduler& getInstance();

    /**
     * @typedef TaskCallback
     * @brief A function type for scheduled tasks.
     */
    using TaskCallback = std::function<void()>;

    /**
     * @brief Schedules a task to be executed only once after a specified delay.
     * @param delayMs The delay in milliseconds before the task is executed.
     * @param callback The function to be executed.
     */
    void scheduleOnce(unsigned long delayMs, TaskCallback callback);

    /**
     * @brief Schedules a task to be executed periodically.
     * @param intervalMs The interval in milliseconds between executions.
     * @param callback The function to be executed.
     */
    void scheduleRecurring(unsigned long intervalMs, TaskCallback callback);

    /**
     * @brief The main update loop for the scheduler.
     * @details This method must be called repeatedly, typically from `SystemManager::loop()`,
     *          to check for and execute any pending tasks.
     */
    void loop();

private:
    /**
     * @brief Private constructor to enforce the singleton pattern.
     */
    Scheduler() {}

    /**
     * @struct ScheduledTask
     * @brief Internal structure to hold information about a scheduled task.
     */
    struct ScheduledTask {
        unsigned long interval;
        unsigned long lastRun;
        TaskCallback callback;
        bool recurring;
    };

    /**
     * @brief A vector that stores all scheduled tasks.
     */
    std::vector<ScheduledTask> tasks;
};