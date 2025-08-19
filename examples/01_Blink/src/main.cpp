#include <Arduino.h>
#include <Nextino.h>

// ეს ფაილი ავტომატურად იქმნება build სკრიპტით.
// ის შეიცავს ყველა საჭირო #include-ს და registerAllModuleTypes() ფუნქციას.
#include "generated_config.h"

void setup() {
    // 1. ლოგერის ინიციალიზაცია
    Logger::getInstance().begin(LOG_LEVEL_DEBUG);
    
    NEXTINO_LOGI("Main", "--- Nextino Blink Demo ---");

    // 2. ყველა ავტომატურად აღმოჩენილი მოდულის ტიპის რეგისტრაცია
    registerAllModuleTypes();

    // 3. სისტემის გაშვება. ის თავად შექმნის და გაუშვებს მოდულებს.
    NextinoSystem().begin();
    
    NEXTINO_LOGI("Main", "System is running.");
}

void loop() {
    NextinoSystem().loop();
}