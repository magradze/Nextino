#pragma once
#include <Nextino.h>
#include <string>

class LedModule : public BaseModule {
private:
    // Define clear states for the module
    enum class LedState {
        OFF,
        ON,
        BLINKING
    };

    int _pin;
    unsigned long _interval;
    uint32_t _taskHandle;
    LedState _currentState; // Use a single state variable

    // Event handlers
    void handleShortPress(void* payload);
    void handleLongPress(void* payload);

    // Private method to change state cleanly
    void setState(LedState newState);

public:
    // Updated constructor to accept the instance name
    LedModule(const char* instanceName, const JsonObject& config);

    // Updated static create function to match the ModuleCreationFunction signature
    static BaseModule* create(const char* instanceName, const JsonObject& config) {
        return new LedModule(instanceName, config);
    }

    const char* getName() const override;
    void init() override;
    void start() override;

    // Public API for ServiceLocator
    void turnOn();
    void turnOff();
};