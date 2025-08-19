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
    // Use the safer "Construct on First Use" idiom to prevent initialization order issues.
    static Logger *instance = nullptr;
    if (instance == nullptr)
    {
        instance = new Logger();
    }
    return *instance;
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
        // Manual ANSI escape code writing for maximum terminal compatibility.
        switch (level)
        {
        case LogLevel::Error:
            Serial.write(27);
            Serial.print("[31m[E]");
            Serial.write(27);
            Serial.print("[0m ");
            break;
        case LogLevel::Warn:
            Serial.write(27);
            Serial.print("[33m[W]");
            Serial.write(27);
            Serial.print("[0m ");
            break;
        case LogLevel::Info:
            Serial.write(27);
            Serial.print("[32m[I]");
            Serial.write(27);
            Serial.print("[0m ");
            break;
        case LogLevel::Debug:
            Serial.write(27);
            Serial.print("[34m[D]");
            Serial.write(27);
            Serial.print("[0m ");
            break;
        default:
            return;
        }

        if (isCore)
        {
            Serial.write(27);
            Serial.print("[95m["); // Bright Magenta for Core
            Serial.print(tag);
            Serial.write(27);
            Serial.print("[0m]: ");
        }
        else
        {
            Serial.write(27);
            Serial.print("[36m["); // Cyan for Modules
            Serial.print(tag);
            Serial.write(27);
            Serial.print("[0m]: ");
        }

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