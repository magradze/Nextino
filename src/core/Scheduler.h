/**
 * @file        Scheduler.h
 * @title       Non-Blocking Task Scheduler
 * @description Defines the `Scheduler` singleton class, providing a non-blocking,
 *              millis-based task scheduler for periodic and one-shot operations.
 *
 * @author      Giorgi Magradze
 * @date        2025-08-21
 * @version     0.2.0
 *
 * @copyright   (c) 2025 Nextino. All rights reserved.
 * @license     MIT License
 */

#pragma once
#include <vector>
#include <functional>
#include <cstdint> // For uint32_t

/**
 * @class Scheduler
 * @brief A singleton class for managing non-blocking, time-based tasks.
 */
class Scheduler {
public:
    /**
     * @typedef TaskHandle
     * @brief A unique identifier for a scheduled task.
     */
    using TaskHandle = uint32_t;

    /**
     * @typedef TaskCallback
     * @brief A function type for scheduled tasks.
     */
    using TaskCallback = std::function<void()>;

    static Scheduler &getInstance();

    /**
     * @brief Schedules a task to be executed only once after a specified delay.
     * @param delayMs The delay in milliseconds before the task is executed.
     * @param callback The function to be executed.
     * @return A unique handle for the scheduled task.
     */
    TaskHandle scheduleOnce(unsigned long delayMs, TaskCallback callback);

    /**
     * @brief Schedules a task to be executed periodically.
     * @param intervalMs The interval in milliseconds between executions.
     * @param callback The function to be executed.
     * @return A unique handle for the scheduled task.
     */
    TaskHandle scheduleRecurring(unsigned long intervalMs, TaskCallback callback);

    /**
     * @brief Cancels a previously scheduled task.
     * @param handle The handle of the task to cancel, returned by a schedule* method.
     * @return True if the task was found and cancelled, false otherwise.
     */
    bool cancel(TaskHandle handle);

    void loop();

private:
    Scheduler() : _nextTaskHandle(1) {} // Initialize handle counter

    /**
     * @struct ScheduledTask
     * @brief Internal structure to hold information about a scheduled task.
     */
    struct ScheduledTask {
        TaskHandle handle; // Unique ID for this task
        unsigned long interval;
        unsigned long lastRun;
        TaskCallback callback;
        bool recurring;
    };

    std::vector<ScheduledTask> _tasks;
    TaskHandle _nextTaskHandle;
};