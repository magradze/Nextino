#pragma once
#include <Nextino.h>

class LedModule : public BaseModule
{
private:
    int _pin;
    unsigned long _interval;
    bool _ledState;

public:
    // Constructor updated for multiple instances
    LedModule(const char *instanceName, const JsonObject &config);

    // Static create function updated to match the new factory signature
    static BaseModule *create(const char *instanceName, const JsonObject &config)
    {
        return new LedModule(instanceName, config);
    }

    const char *getName() const override;
    void init() override;
    void start() override;
};