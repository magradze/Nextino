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

// This ensures Arduino-specific code is only compiled in an Arduino environment.
#if defined(ARDUINO)
#include <Arduino.h>
#endif

Logger &Logger::getInstance()
{
    // Use the modern and thread-safe Meyers' Singleton pattern.
    static Logger instance;
    return instance;
}

void Logger::begin(LogLevel level, LogOutputType outputType)
{
    this->currentLevel = level;
    this->outputType = outputType;

#if defined(ARDUINO)
    if (this->outputType == LogOutputType::Serial)
    {
        if (!Serial)
        {
            Serial.begin(115200);
            // Wait for serial port to connect. Needed for native USB.
            while (!Serial)
            {
                delay(10);
            }
        }
        // A small delay to ensure the monitor is ready to receive data.
        delay(100);
    }
#endif
    // Use its own logging mechanism to announce it's ready.
    logf(LogLevel::Info, true, "Logger", "Logger initialized. Level: %d", (int)level);
}

void Logger::log(LogLevel level, bool isCore, const char *tag, const char *message)
{
    if ((int)level > (int)currentLevel || level == LogLevel::None)
    {
        return;
    }

#if defined(ARDUINO)
    if (this->outputType == LogOutputType::Serial)
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
        Serial.print(LOG_COLOR_RESET);

        // Print Tag: [SysManager], [LedModule], etc.
        const char *tagColor = isCore ? LOG_COLOR_NEON_PURPLE : LOG_COLOR_CYAN;
        Serial.print(tagColor);
        Serial.print("[");
        Serial.print(tag);
        Serial.print("]: ");
        Serial.print(LOG_COLOR_RESET);

        // Print Message
        Serial.println(message);
    }
#else
    // Fallback for non-Arduino (native) environments.
    const char *levelStr = "";
    switch (level)
    {
    case LogLevel::Error:
        levelStr = "[E]";
        break;
    case LogLevel::Warn:
        levelStr = "[W]";
        break;
    case LogLevel::Info:
        levelStr = "[I]";
        break;
    case LogLevel::Debug:
        levelStr = "[D]";
        break;
    default:
        return;
    }
    printf("%s [%s]: %s\n", levelStr, tag, message);
#endif
}

void Logger::logf(LogLevel level, bool isCore, const char *tag, const char *format, ...)
{
    if ((int)level > (int)currentLevel || level == LogLevel::None)
    {
        return;
    }

    char buffer[256];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    // Call the base log function with the formatted buffer.
    log(level, isCore, tag, buffer);
}