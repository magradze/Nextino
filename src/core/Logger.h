/**
 * @file        Logger.h
 * @title       Framework Logger
 * @description Defines the `Logger` singleton class and global logging macros.
 *              Provides a centralized, leveled, colored, and thread-safe
 *              logging system for robust debugging in multitasking environments.
 *
 * @author      Giorgi Magradze
 * @date        2025-08-24
 * @version     0.3.0
 */

#pragma once
#include <Arduino.h>
#include <stdarg.h>
#include "LogColors.h"

// --- Thread-Safety for ESP32 ---
// Include FreeRTOS headers only when compiling for ESP32
#if defined(ESP32)
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#endif

/**
 * @enum LogLevel
 * @brief Defines the severity levels for log messages.
 */
enum class LogLevel {
    None,    /**< No logging. */
    Error,   /**< Critical errors that prevent normal operation. */
    Warn,    /**< Warnings about potential issues. */
    Info,    /**< Informational messages about major lifecycle events. */
    Debug    /**< Detailed messages for debugging. */
};

// Enum for future output types (e.g., File, MQTT).
enum class LogOutputType
{
    Serial
};

/**
 * @class Logger
 * @brief A thread-safe singleton class for handling all log output.
 * @details On ESP32, this class uses a FreeRTOS mutex to protect against
 *          race conditions when logging from different tasks (e.g., the main
 *          loop and a WiFi/MQTT callback).
 */
class Logger {
public:
    /**
     * @brief Gets the singleton instance of the Logger.
     * @return A reference to the Logger.
     */
    static Logger& getInstance();

    /**
     * @brief Initializes the logger and underlying hardware (e.g., Serial port).
     * @param level The maximum log level to display.
     * @param outputType The destination for the log output (currently only Serial).
     */
    void begin(LogLevel level = LogLevel::Info, LogOutputType outputType = LogOutputType::Serial);

    /**
     * @brief The core logging function (thread-safe).
     * @details This is the main entry point for all log messages. It is recommended
     *          to use the `NEXTINO_LOG*` macros instead of calling this directly.
     * @param level The severity level of the message.
     * @param isCore True if the message is from a core component, for special coloring.
     * @param tag The source of the log message (e.g., module instance name).
     * @param format The printf-style format string.
     * @param ... Arguments for the format string.
     */
    void logf(LogLevel level, bool isCore, const char* tag, const char* format, ...);

private:
    /**
     * @brief Private constructor to enforce singleton pattern and initialize mutex.
     */
    Logger();

    // Delete copy constructor and assignment operator.
    Logger(const Logger &) = delete;
    void operator=(const Logger &) = delete;

    /**
     * @brief The non-thread-safe, internal logging implementation.
     * @details This method is always called from within the mutex lock in `logf`.
     */
    void log(LogLevel level, bool isCore, const char *tag, const char *message);

    LogLevel currentLevel;
    LogOutputType outputType;

#if defined(ESP32)
    SemaphoreHandle_t _logMutex; // The FreeRTOS mutex to ensure thread safety
#endif
};

// --- Global Logging Macros (unchanged, they correctly point to `logf`) ---
#define NEXTINO_LOG(level, tag, ...) Logger::getInstance().logf(level, false, tag, __VA_ARGS__)
#define NEXTINO_CORE_LOG(level, tag, ...) Logger::getInstance().logf(level, true, tag, __VA_ARGS__)
#define NEXTINO_LOGE(tag, ...) NEXTINO_LOG(LogLevel::Error, tag, __VA_ARGS__)
#define NEXTINO_LOGW(tag, ...) NEXTINO_LOG(LogLevel::Warn, tag, __VA_ARGS__)
#define NEXTINO_LOGI(tag, ...) NEXTINO_LOG(LogLevel::Info, tag, __VA_ARGS__)
#define NEXTINO_LOGD(tag, ...) NEXTINO_LOG(LogLevel::Debug, tag, __VA_ARGS__)