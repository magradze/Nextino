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
    ButtonModule(const JsonObject& config);
    
    static BaseModule* create(const JsonObject& config) {
        return new ButtonModule(config);
    }

    const char* getName() const override;
    void init() override;
    void loop() override;
};