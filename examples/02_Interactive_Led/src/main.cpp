/**
 * @file        main.cpp
 * @title       Nextino Plug-and-Play Demo
 * @description This file acts as the "assembler" for the application. It is the
 *              only place that knows about both the framework and the project-
 *              specific, auto-generated configuration.
 *
 * @author      Giorgi Magradze
 * @date        2025-08-21
 * @version     0.2.0
 */
#include <Arduino.h>
#include <Nextino.h>
#include "generated_config.h"

void setup() {
    // Step 1: Initialize the hardware Serial port.
    // This is the safest first step.
    Serial.begin(115200);
    while (!Serial)
    {
        delay(10);
    }
    delay(1000); // Give the monitor time to connect.

    // Step 2: Configure the Logger service.
    // It will now use the already-initialized Serial port.
    Logger::getInstance().begin(LogLevel::Debug);

    NEXTINO_LOGI("Main", "--- Nextino Auto-Discovery Project ---");

    // Step 3: Register all discovered module types.
    registerAllModuleTypes();

    // Step 4: Start the Nextino system.
    NextinoSystem().begin(projectConfigJson);

    NEXTINO_LOGI("Main", "System is running.");
}

void loop() {
    NextinoSystem().loop();
}