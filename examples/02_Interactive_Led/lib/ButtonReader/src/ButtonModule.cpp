#include "ButtonModule.h"

ButtonModule::ButtonModule(const char* instanceName, const JsonObject& config)
    : BaseModule(instanceName) { // Pass instanceName to the base class constructor
    JsonObject resourceObj = config["resource"];
    _pin = resourceObj["pin"];
    _longPressTime = config["long_press_ms"] | 1000;

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
        NEXTINO_LOGI(getInstanceName(), "Initialized on pin %d.", _pin);
}

void ButtonModule::loop() {
    int reading = digitalRead(_pin);

    // --- Debounce Logic ---
    // If the switch changed, due to noise or pressing, reset the debounce timer
    if (reading != _lastButtonState) {
        _lastDebounceTime = millis();
    }

    // --- State Change Logic ---
    // Only proceed if the reading has been stable for longer than the debounce delay
    if ((millis() - _lastDebounceTime) > 50) {
        // If the button state has changed
        if (reading != _buttonState) {
            _buttonState = reading;

            if (_buttonState == LOW) { // Button was JUST pressed
                _pressStartTime = millis();
                _longPressTriggered = false;
                NEXTINO_LOGD(getInstanceName(), "Button press started.");
            } else { // Button was JUST released
                // Only fire short press if long press hasn't already been triggered
                if (!_longPressTriggered) {
                    NEXTINO_LOGI(getInstanceName(), "Short press detected! Posting event.");
                    NextinoEvent().post("button_short_press");
                }
                // Reset the timer to indicate the press cycle is over
                _pressStartTime = 0;
            }
        }
    }

    // --- Long Press Detection Logic ---
    // This runs independently but relies on the state set above.
    // Check only if the button is currently pressed AND the press cycle has started.
    if (_buttonState == LOW && _pressStartTime != 0 && !_longPressTriggered) {
        if (millis() - _pressStartTime > _longPressTime) {
            _longPressTriggered = true;
            NEXTINO_LOGI(getInstanceName(), "Long press detected! Posting event.");
            NextinoEvent().post("button_long_press");
        }
    }

    _lastButtonState = reading;
}