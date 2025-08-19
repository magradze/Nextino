/**
 * @file        LedModule.cpp
 * @title       LED Flasher Module Implementation
 * @description Implements the logic for the `LedModule` class.
 *
 * @author      Giorgi Magradze
 * @date        2025-08-19
 * @version     0.1.0
 *
 * @copyright   (c) 2025 Nextino. All rights reserved.
 * @license     MIT License
 */

#include "LedModule.h"

LedModule::LedModule(const JsonObject& config) : ledState(false) {
    ledPin = config["pin"] | -1;

    const char* colorStr = config["log_color"] | "blue"; // If not present, defaults to "blue"

    if (strcmp(colorStr, "green") == 0) {
        logColor = LOG_COLOR_GREEN;
    } else { // Includes "blue" and all other cases
        logColor = LOG_COLOR_BLUE;
    }
}

const char* LedModule::getName() const {
    return "LedModule";
}

void LedModule::init() {
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, ledState);
    
    NEXTINO_LOGI(getName(), "Initialized on pin %d.", ledPin);
}

void LedModule::start() {
    NEXTINO_LOGI(getName(), "Scheduling blink task...");
    
    NextinoScheduler().scheduleRecurring(5000, [this]() {
        this->toggle();
    });
}

void LedModule::toggle() {
    ledState = !ledState;
    digitalWrite(ledPin, ledState);

    NEXTINO_LOGD(getName(), "Toggled LED to %s", ledState ? "ON" : "OFF");
}