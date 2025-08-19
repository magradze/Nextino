/**
 * @file        Logger.h
 * @title       Framework Logger
 * @description Defines the `Logger` singleton class and global logging macros
 *              (NEXTINO_LOGI, NEXTINO_LOGD, etc.). Provides a centralized,
 *              leveled, and colored logging system.
 *
 * @author      Giorgi Magradze
 * @date        2025-08-19
 * @version     0.1.0
 *
 * @copyright   (c) 2025 Nextino. All rights reserved.
 * @license     MIT License
 */

#pragma once
#include <Arduino.h>
#include <stdarg.h>
#include "LogColors.h"

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

/**
 * @class Logger
 * @brief A singleton class for handling all log output.
 */
class Logger {
public:
    /**
     * @brief Gets the singleton instance of the Logger.
     * @return A reference to the Logger.
     */
    static Logger& getInstance();

    /**
     * @brief Initializes the logger and the underlying Serial port.
     * @details This should be the very first function called in `setup()`.
     * @param level The maximum log level to display. Messages with a higher level will be ignored.
     */
    void begin(LogLevel level = LogLevel::Info);

    /**
     * @brief Logs a message.
     * @param level The severity level of the message.
     * @param isCore True if the message is from a core component, for special coloring.
     * @param tag The source of the log message (e.g., module name).
     * @param message The log message.
     */
    void log(LogLevel level, bool isCore, const char* tag, const char* message);
    
    /**
     * @brief The core logging function.
     * @details It is recommended to use the `NEXTINO_LOG*` macros instead of calling this directly.
     * @param level The severity level of the message.
     * @param isCore True if the message is from a core component, for special coloring.
     * @param tag The source of the log message (e.g., module name).
     * @param format The printf-style format string.
     * @param ... Arguments for the format string.
     */
    void logf(LogLevel level, bool isCore, const char* tag, const char* format, ...);

private:
    Logger() : currentLevel(LogLevel::None) {}
    LogLevel currentLevel;
};

// --- Global Logging Macros ---

/**
 * @def NEXTINO_LOG(level, tag, ...)
 * @brief Generic logging macro for modules.
 */
#define NEXTINO_LOG(level, tag, ...) Logger::getInstance().logf(level, false, tag, __VA_ARGS__)

/**
 * @def NEXTINO_CORE_LOG(level, tag, ...)
 * @brief Special logging macro for core framework components.
 */
#define NEXTINO_CORE_LOG(level, tag, ...) Logger::getInstance().logf(level, true, tag, __VA_ARGS__)

/**
 * @def NEXTINO_LOGE(tag, ...)
 * @brief Logs an ERROR level message.
 */
#define NEXTINO_LOGE(tag, ...) NEXTINO_LOG(LogLevel::Error, tag, __VA_ARGS__)

/**
 * @def NEXTINO_LOGW(tag, ...)
 * @brief Logs a WARNING level message.
 */
#define NEXTINO_LOGW(tag, ...) NEXTINO_LOG(LogLevel::Warn,  tag, __VA_ARGS__)

/**
 * @def NEXTINO_LOGI(tag, ...)
 * @brief Logs an INFO level message.
 */
#define NEXTINO_LOGI(tag, ...) NEXTINO_LOG(LogLevel::Info,  tag, __VA_ARGS__)

/**
 * @def NEXTINO_LOGD(tag, ...)
 * @brief Logs a DEBUG level message.
 */
#define NEXTINO_LOGD(tag, ...) NEXTINO_LOG(LogLevel::Debug, tag, __VA_ARGS__)