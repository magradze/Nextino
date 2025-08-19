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
#include <stdio.h>

Logger &Logger::getInstance()
{
    static Logger instance;
    return instance;
}

void Logger::begin(LogLevel level)
{
    if (!Serial)
    {
        Serial.begin(115200);
        while (!Serial)
        {
        }
    }
    Serial.flush();
    delay(100);

    currentLevel = level;
    NEXTINO_CORE_LOG(LogLevel::Info, "Logger", "Logger initialized. Level: %d", (int)level);
}

void Logger::log(LogLevel level, bool isCore, const char* tag, const char* message)
{
    if ((int)level > (int)currentLevel || level == LogLevel::None)
        return;

    // Enable colors - works in PlatformIO Serial Monitor!
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
        Serial.print("[95m[");
        Serial.print(tag);
        Serial.write(27);
        Serial.print("[0m]: ");
    }
    else
    {
        Serial.write(27);
        Serial.print("[36m[");
        Serial.print(tag);
        Serial.write(27);
        Serial.print("[0m]: ");
    }

    Serial.println(message);
}

void Logger::logf(LogLevel level, bool isCore, const char *tag, const char *format, ...)
{
    if ((int)level > (int)currentLevel || level == LogLevel::None)
        return;

    char buffer[256];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    log(level, isCore, tag, buffer);
}