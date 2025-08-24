#include "LedModule.h"

// The constructor now receives the unique instanceName and passes it to the parent BaseModule
LedModule::LedModule(const char *instanceName, const JsonObject &config)
    : BaseModule(instanceName)
{

    // Read pin from the "resource" object
    _pin = config["resource"]["pin"];
    _interval = config["blink_interval_ms"] | 1000;
    _ledState = false;
}

// getName() returns the generic TYPE of the module
const char* LedModule::getName() const {
    return "LedModule";
}

void LedModule::init() {
    pinMode(_pin, OUTPUT);
    digitalWrite(_pin, _ledState);
    // Use getInstanceName() for logs to show WHICH instance is talking!
    NEXTINO_LOGI(getInstanceName(), "Initialized on pin %d.", _pin);
}

void LedModule::start() {
    NextinoScheduler().scheduleRecurring(_interval, [this]()
                                         {
        _ledState = !_ledState;
        digitalWrite(_pin, _ledState); });
    NEXTINO_LOGI(getInstanceName(), "Blink task scheduled every %lu ms.", _interval);
}