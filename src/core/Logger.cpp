/**
 * @file        Logger.cpp
 * @title       Logger Implementation
 * @description Implements the logic for the `Logger` class, including message
 *              formatting, coloring, and output to the Serial port.
 *
 * @author      Giorgi Magradze
 * @date        2025-08-19
 * @version     0.1.0
 *
 * @copyright   (c) 2025 Nextino. All rights reserved.
 * @license     MIT License
 */

#include "Logger.h"
#include <stdio.h> // For vsnprintf

Logger &Logger::getInstance()
{
    // The Meyers' Singleton pattern is inherently thread-safe for initialization.
    static Logger instance;
    return instance;
}

Logger::Logger() : currentLevel(LogLevel::None)
{
#if defined(ESP32)
    _logMutex = xSemaphoreCreateMutex();
    if (_logMutex == NULL)
    {
        // This is a catastrophic failure, usually out of memory.
        // We can't use our own logger here, as it's not ready.
        Serial.println("FATAL: Could not create Logger mutex!");
    }
#endif
}

void Logger::begin(LogLevel level, LogOutputType outputType)
{
    // It's safe to take the mutex here in case begin() is called from multiple threads,
    // although it is not a typical use case.
#if defined(ESP32)
    if (xSemaphoreTake(_logMutex, portMAX_DELAY) != pdTRUE)
        return;
#endif

    this->currentLevel = level;
    this->outputType = outputType;

    if (this->outputType == LogOutputType::Serial)
    {
        // Start Serial if it's not already running.
        if (!Serial)
        {
            Serial.begin(115200);
            // Wait for serial port to connect on native USB boards
            while (!Serial)
            {
                delay(10);
            }
        }
        delay(100); // Small delay to ensure the monitor is ready.
    }

#if defined(ESP32)
    xSemaphoreGive(_logMutex);
#endif

    // Use its own logging mechanism to announce it's ready. This call will
    // re-acquire the mutex inside logf().
    logf(LogLevel::Info, true, "Logger", "Logger initialized. Level: %d", (int)level);
}

void Logger::logf(LogLevel level, bool isCore, const char *tag, const char *format, ...)
{
    // Basic checks before attempting to take the mutex
    if ((int)level > (int)currentLevel || level == LogLevel::None || !tag || !format)
    {
        return;
    }

#if defined(ESP32)
    // Attempt to take the mutex. If another task is logging, this will block
    // until the mutex is available, ensuring sequential, non-corrupted output.
    if (_logMutex == NULL || xSemaphoreTake(_logMutex, portMAX_DELAY) != pdTRUE)
    {
        return; // Failed to take semaphore, abort to prevent deadlock
    }
#endif

    // --- Critical Section Start ---
    // Only one task can execute this code at a time.

    char buffer[256];
    va_list args;
    va_start(args, format);
    // Use vsnprintf for safe, bounded string formatting
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    // Call the internal, non-thread-safe implementation
    log(level, isCore, tag, buffer);

    // --- Critical Section End ---

#if defined(ESP32)
    // Release the mutex, allowing other tasks to log.
    xSemaphoreGive(_logMutex);
#endif
}

// This is the internal implementation and is NOT thread-safe by itself.
// It must always be called from a function that holds the mutex.
void Logger::log(LogLevel level, bool isCore, const char *tag, const char *message)
{
    if (outputType == LogOutputType::Serial)
    {
        const char *levelColor = LOG_COLOR_RESET;
        const char *levelChar = "";

        switch (level)
        {
        case LogLevel::Error:
            levelColor = LOG_COLOR_RED;
            levelChar = "E";
            break;
        case LogLevel::Warn:
            levelColor = LOG_COLOR_YELLOW;
            levelChar = "W";
            break;
        case LogLevel::Info:
            levelColor = LOG_COLOR_GREEN;
            levelChar = "I";
            break;
        case LogLevel::Debug:
            levelColor = LOG_COLOR_BLUE;
            levelChar = "D";
            break;
        default:
            return;
        }

        // Print Level: [E], [W], etc.
        Serial.print(levelColor);
        Serial.print("[");
        Serial.print(levelChar);
        Serial.print("] ");

        // Print Tag: [SysManager], [instance_name], etc.
        const char *tagColor = isCore ? LOG_COLOR_NEON_PURPLE : LOG_COLOR_CYAN;
        Serial.print(tagColor);
        Serial.print("[");
        Serial.print(tag);
        Serial.print("]: ");
        Serial.print(LOG_COLOR_RESET);

        // Print Message and newline
        Serial.println(message);
        // Ensure the buffer is flushed on error messages
        if (level == LogLevel::Error)
        {
            Serial.flush();
        }
    }
}