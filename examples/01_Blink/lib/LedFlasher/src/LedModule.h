#pragma once
#include <Nextino.h>

class LedModule : public BaseModule
{
private:
    int _pin;
    unsigned long _interval;
    bool _ledState;

public:
    LedModule(const JsonObject &config);

    static BaseModule* create(const JsonObject& config) {
        return new LedModule(config);
    }

    const char *getName() const override;
    void init() override;
    void start() override;
};