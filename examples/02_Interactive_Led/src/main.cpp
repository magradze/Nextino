#include <Arduino.h>
#include <Nextino.h>
#include "generated_config.h"

void setup() {
    Logger::getInstance().begin(LogLevel::Debug);
    NEXTINO_LOGI("Main", "--- Nextino Interactive LED Demo ---");

    registerAllModuleTypes();
    NextinoSystem().begin();
    
    // Use ServiceLocator to get the LED module and turn it on initially
    LedModule* led = NextinoServices().get<LedModule>("led_service");
    if (led) {
        led->turnOn();
    }
    
    NEXTINO_LOGI("Main", "System is running.");
}

void loop() {
    NextinoSystem().loop();
}