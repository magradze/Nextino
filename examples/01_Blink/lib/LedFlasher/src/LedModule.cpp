#include "LedModule.h"

LedModule::LedModule(const JsonObject &config)
{
    // Read pin from the "resource" object for ResourceManager compatibility
    _pin = config["resource"]["pin"];
    _interval = config["blink_interval_ms"] | 1000; // Default to 1 second
    _ledState = false;
}

const char* LedModule::getName() const {
    return "LedModule";
}

void LedModule::init() {
    pinMode(_pin, OUTPUT);
    digitalWrite(_pin, _ledState);
    NEXTINO_LOGI(getName(), "Initialized on pin %d.", _pin);
}

void LedModule::start() {
    NextinoScheduler().scheduleRecurring(_interval, [this]()
                                         {
        _ledState = !_ledState;
        digitalWrite(_pin, _ledState); });
    NEXTINO_LOGI(getName(), "Blink task scheduled every %lu ms.", _interval);
}