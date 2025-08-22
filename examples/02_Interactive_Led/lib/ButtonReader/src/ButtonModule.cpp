#include "ButtonModule.h"

ButtonModule::ButtonModule(const JsonObject& config) {
    JsonObject resourceObj = config["resource"];
    _pin = resourceObj["pin"];
    _longPressTime = config["long_press_ms"] | 1000; // Default 1 second

    _lastButtonState = HIGH;
    _buttonState = HIGH;
    _lastDebounceTime = 0;
    _pressStartTime = 0;
    _longPressTriggered = false;
}

const char* ButtonModule::getName() const {
    return "ButtonModule";
}

void ButtonModule::init() {
    pinMode(_pin, INPUT_PULLUP);
    NEXTINO_LOGI(getName(), "Initialized on pin %d.", _pin);
}

void ButtonModule::loop() {
    int reading = digitalRead(_pin);

    if (reading != _lastButtonState) {
        _lastDebounceTime = millis();
    }

    if ((millis() - _lastDebounceTime) > 50) { // 50ms debounce delay
        if (reading != _buttonState) {
            _buttonState = reading;

            if (_buttonState == LOW) { // Button was pressed
                _pressStartTime = millis();
                _longPressTriggered = false;
                NEXTINO_LOGD(getName(), "Button press started.");
            } else { // Button was released
                if (!_longPressTriggered) {
                    NEXTINO_LOGI(getName(), "Short press detected! Posting event.");
                    NextinoEvent().post("button_short_press");
                }
                _pressStartTime = 0;
            }
        }
    }

    // Long press detection
    if (_buttonState == LOW && !_longPressTriggered && (millis() - _pressStartTime) > _longPressTime) {
        _longPressTriggered = true;
        NEXTINO_LOGI(getName(), "Long press detected! Posting event.");
        NextinoEvent().post("button_long_press");
    }

    _lastButtonState = reading;
}