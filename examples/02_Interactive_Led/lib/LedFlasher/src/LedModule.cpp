#include "LedModule.h"

// --- Constructor and Lifecycle ---

LedModule::LedModule(const char* instanceName, const JsonObject& config)
    : BaseModule(instanceName) { // Pass instanceName to the base class constructor
    JsonObject resourceObj = config["resource"];
    _pin = resourceObj["pin"];
    _interval = config["blink_interval_ms"] | 500;
    _taskHandle = 0;
    _currentState = LedState::OFF; // Initial state
}

const char* LedModule::getName() const { return "LedModule"; }

void LedModule::init() {
    pinMode(_pin, OUTPUT);
    digitalWrite(_pin, LOW);
    
    // Create a unique service name, e.g., "led:main_light"
    std::string serviceName = std::string(getName()) + ":" + _instanceName;
    NextinoServices().provide(serviceName.c_str(), this);
    
    // Use getInstanceName() for logging to distinguish between different instances
    NEXTINO_LOGI(getInstanceName(), "Initialized on pin %d and provided service '%s'.", _pin, serviceName.c_str());
}

void LedModule::start() {
    NextinoEvent().on("button_short_press", [this](void* p) { this->handleShortPress(p); });
    NextinoEvent().on("button_long_press", [this](void* p) { this->handleLongPress(p); });
    NEXTINO_LOGI(getName(), "Subscribed to button events.");
}

// --- Public API Methods ---

void LedModule::turnOn() {
    setState(LedState::ON);
}

void LedModule::turnOff() {
    setState(LedState::OFF);
}

// --- Event Handlers ---

void LedModule::handleShortPress(void* payload) {
    // Short press toggles between BLINKING and OFF
    if (_currentState == LedState::BLINKING) {
        setState(LedState::OFF);
    } else {
        setState(LedState::BLINKING);
    }
}

void LedModule::handleLongPress(void* payload) {
    // Long press toggles between ON and OFF
    if (_currentState == LedState::ON) {
        setState(LedState::OFF);
    } else {
        setState(LedState::ON);
    }
}

// --- Private State Machine ---

void LedModule::setState(LedState newState) {
    if (_currentState == newState) return; // No change

    _currentState = newState;
    NEXTINO_LOGI(getName(), "Changing state to %d", (int)_currentState);

    // First, always clean up the previous state (cancel timers)
    if (_taskHandle != 0) {
        NextinoScheduler().cancel(_taskHandle);
        _taskHandle = 0;
    }

    // Then, apply the new state
    switch (_currentState) {
        case LedState::OFF:
            digitalWrite(_pin, LOW);
            break;
        case LedState::ON:
            digitalWrite(_pin, HIGH);
            break;
        case LedState::BLINKING:
            _taskHandle = NextinoScheduler().scheduleRecurring(_interval, [this]() {
                digitalWrite(_pin, !digitalRead(_pin));
            });
            break;
    }
}