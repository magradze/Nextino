/**
 * @file        BaseModule.h
 * @title       Base Module Interface
 * @description Defines the abstract `BaseModule` class, which serves as the
 *              fundamental contract for all modules within the Nextino framework.
 *              It specifies the lifecycle methods that every module should implement.
 *
 * @author      Giorgi Magradze
 * @date        2025-08-19
 * @version     0.1.0
 *
 * @copyright   (c) 2025 Nextino. All rights reserved.
 * @license     MIT License
 */

#pragma once
#if defined(ARDUINO)
#include <Arduino.h>
#endif

/**
 * @class BaseModule
 * @brief The abstract base class for all Nextino modules.
 * @details All functional units in a Nextino project should inherit from this class
 *          and override its virtual methods to implement their specific logic.
 */
class BaseModule {
protected:
    /**
     * @brief The unique instance name for this module instance.
     */
    const char *_instanceName;

public:
    /**
     * @brief Virtual destructor.
     */
    virtual ~BaseModule() {}

    /**
     * @brief Base constructor for all modules.
     * @param instanceName The unique name for this specific module instance.
     */
    BaseModule(const char *instanceName) : _instanceName(instanceName) {}

    /**
     * @brief Called once by the SystemManager during the initial setup phase.
     * @details Use this method for one-time initializations like setting pin modes,
     *          initializing hardware, or locking resources.
     */
    virtual void init() {}

    /**
     * @brief Called once by the SystemManager after all modules have been initialized.
     * @details Use this method to start active processes, such as scheduling tasks
     *          with the Scheduler or subscribing to events on the EventBus.
     */
    virtual void start() {}

    /**
     * @brief Called repeatedly in the main program loop by the SystemManager.
     * @details This method should be non-blocking. Use it for continuous polling
     *          or state machine updates that need to run on every loop iteration.
     *          Avoid using `delay()` inside this method.
     */
    virtual void loop() {}

    /**
     * @brief Gets the unique name of the module.
     * @details This is a pure virtual function and must be implemented by all derived classes.
     *          The name is used for logging and service location.
     * @return A constant string representing the module's name.
     */
    virtual const char* getName() const = 0;

    /**
     * @brief Gets the unique instance name assigned in the configuration.
     * @details If no `instance_name` is provided in the config, this will be
     *          the same as the module type (getName()).
     * @return A constant string representing the instance's unique name.
     */
    const char *getInstanceName() const
    {
        return _instanceName;
    }
};