#pragma once
#include <Nextino.h>

class LedModule : public BaseModule {
private:
    int _pin;
    unsigned long _interval;
    uint32_t _taskHandle;
    bool _isOn;

    // Event handlers
    void handleShortPress(void* payload);
    void handleLongPress(void* payload);

public:
    LedModule(const JsonObject& config);
    static BaseModule* create(const JsonObject& config) { return new LedModule(config); }

    const char* getName() const override;
    void init() override;
    void start() override;

    // Public API for ServiceLocator
    void turnOn();
    void turnOff();
};