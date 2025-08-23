#pragma once
#include <Nextino.h>

class ButtonModule : public BaseModule {
private:
    int _pin;
    unsigned long _longPressTime;
    
    // Internal state for debouncing and long press detection
    bool _buttonState;
    bool _lastButtonState;
    unsigned long _lastDebounceTime;
    unsigned long _pressStartTime;
    bool _longPressTriggered;

public:
    // Updated constructor to accept the instance name
    ButtonModule(const char* instanceName, const JsonObject& config);
    
    // Updated static create function to match the ModuleCreationFunction signature
    static BaseModule* create(const char* instanceName, const JsonObject& config) {
        return new ButtonModule(instanceName, config);
    }

    const char* getName() const override;
    void init() override;
    void loop() override;
};