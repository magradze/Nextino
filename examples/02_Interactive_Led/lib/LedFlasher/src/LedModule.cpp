#include "LedModule.h"

LedModule::LedModule(const JsonObject& config) {
    JsonObject resourceObj = config["resource"];
    _pin = resourceObj["pin"];
    _interval = config["blink_interval_ms"] | 500;
    _taskHandle = 0;
    _isOn = false;
}

const char* LedModule::getName() const { return "LedModule"; }

void LedModule::init() {
    pinMode(_pin, OUTPUT);
    digitalWrite(_pin, LOW);
    // Register as a service
    NextinoServices().provide("led_service", this);
    NEXTINO_LOGI(getName(), "Initialized on pin %d and provided 'led_service'.", _pin);
}

void LedModule::start() {
    // Subscribe to button events
    NextinoEvent().on("button_short_press", [this](void* p) { this->handleShortPress(p); });
    NextinoEvent().on("button_long_press", [this](void* p) { this->handleLongPress(p); });
    NEXTINO_LOGI(getName(), "Subscribed to button events.");
}

void LedModule::turnOn() {
    if (_taskHandle != 0) {
        NextinoScheduler().cancel(_taskHandle);
        _taskHandle = 0;
    }
    digitalWrite(_pin, HIGH);
    _isOn = true;
    NEXTINO_LOGI(getName(), "Turned ON via API call.");
}

void LedModule::turnOff() {
    if (_taskHandle != 0) {
        NextinoScheduler().cancel(_taskHandle);
        _taskHandle = 0;
    }
    digitalWrite(_pin, LOW);
    _isOn = false;
    NEXTINO_LOGI(getName(), "Turned OFF via API call.");
}

void LedModule::handleShortPress(void* payload) {
    if (_taskHandle == 0) { // If not blinking, start
        _taskHandle = NextinoScheduler().scheduleRecurring(_interval, [this]() {
            digitalWrite(_pin, !digitalRead(_pin));
        });
        NEXTINO_LOGI(getName(), "Short press: Started blinking.");
    } else { // If blinking, stop
        NextinoScheduler().cancel(_taskHandle);
        _taskHandle = 0;
        digitalWrite(_pin, LOW);
        NEXTINO_LOGI(getName(), "Short press: Stopped blinking.");
    }
}

void LedModule::handleLongPress(void* payload) {
    if (_isOn) {
        turnOff();
    } else {
        turnOn();
    }
}