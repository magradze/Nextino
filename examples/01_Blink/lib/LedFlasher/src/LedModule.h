/**
 * @file        LedModule.h
 * @title       LED Flasher Module
 * @description Defines the `LedModule` class, an example module that demonstrates
 *              how to create a simple, non-blocking LED flasher using the
 *              Nextino framework's Scheduler.
 *
 * @author      Giorgi Magradze
 * @date        2025-08-19
 * @version     0.1.0
 *
 * @copyright   (c) 2025 Nextino. All rights reserved.
 * @license     MIT License
 */

#pragma once
#include <Nextino.h>
#include <ArduinoJson.h>

class LedModule : public BaseModule
{
private:
    int ledPin;
    bool ledState;
    const char* logColor;

public:
    // Constructor to get the pin
    LedModule(const JsonObject &config);

    static BaseModule* create(const JsonObject& config) {
        return new LedModule(config);
    }

    // Implementing BaseModule's virtual functions
    const char *getName() const override;
    void init() override;
    void start() override;

    // Module's own public method
    void toggle();
};