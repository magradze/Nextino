/**
 * @file        test_scheduler.cpp
 * @title       Unit Tests for the Scheduler
 * @description This file contains unit tests for the Nextino Scheduler component
 *              using the Unity test framework.
 *
 * @author      Giorgi Magradze
 * @date        2025-08-19
 * @version     0.1.0
 */

#include <Arduino.h>
#include <unity.h>
#include "core/Scheduler.h"

bool callbackWasCalled = false;

void test_callback_function() {
    callbackWasCalled = true;
}

void setUp(void) {
    callbackWasCalled = false;
}

void tearDown(void) {}

void test_scheduler_runs_a_task() {
    Scheduler& scheduler = Scheduler::getInstance();
    scheduler.scheduleRecurring(10, test_callback_function);
    
    unsigned long startTime = millis();
    while (millis() - startTime < 50) {
        scheduler.loop();
        delay(1);
    }

    TEST_ASSERT_TRUE(callbackWasCalled);
}

void setup() {
    delay(2000);
    UNITY_BEGIN();
    RUN_TEST(test_scheduler_runs_a_task);
}

void loop() {
    UNITY_END();
}