# Testing Guide for the Nextino Framework

This document outlines the testing strategy, tools, and procedures for the Nextino framework. A robust testing suite is crucial for ensuring the stability, reliability, and correctness of the core components.

## Testing Philosophy

Our goal is to maintain a comprehensive suite of unit tests that cover the core functionalities of the framework. This helps us:

- Verify that each component works as expected.
- Prevent regressions when adding new features or refactoring code.
- Provide a clear, executable specification of how each component should behave.

All tests are designed to run on actual target hardware (e.g., ESP32) to ensure they are validated in a real-world environment.

## Tools Used

- **PlatformIO Unit Testing:** The primary engine for running tests.
- **Unity:** A lightweight unit testing framework for C, which is integrated into PlatformIO.

## Test Structure

All tests are located in the `test/` directory at the root of the repository. The structure mirrors the `src/` directory:

```plaintext
Nextino/
└── test/
    └── test_core/
        ├── test_scheduler.cpp
        └── ... (other core component tests)
```

- Each test file should be named `test_*.cpp`.
- Each test file should focus on testing a single class or component.

## How to Run Tests

All tests are executed on a physical target board.

1. Connect your development board (e.g., ESP32) to your computer.
2. Open the PlatformIO "Test" sidebar in VS Code (the beaker icon).
3. Find the appropriate environment for your board (e.g., `esp32dev`).
4. Click the **"Test"** button next to the environment name.

Alternatively, you can run the test from the command line:

```bash
# Run tests for the esp32dev environment
pio test -e esp32dev
```

PlatformIO will compile the tests, upload the firmware to the board, and display the results from the Serial Monitor.

## How to Add a New Test

1. Create a new file in the appropriate subdirectory under `test/`, named `test_*.cpp`.
2. Include `<Arduino.h>`, `<unity.h>`, and the header file of the component you want to test.
3. Write your test functions using Unity's assertion macros (e.g., `TEST_ASSERT_EQUAL`, `TEST_ASSERT_TRUE`).
4. Create a `setup()` and `loop()` function. The `setup()` function should call `UNITY_BEGIN()`, `RUN_TEST(your_test_function)`, and `UNITY_END()`.
5. PlatformIO will automatically discover and run the new test file.

### Example Test Function

```cpp
#include <Arduino.h>
#include <unity.h>
#include "core/MyComponent.h"

void test_my_component_does_something() {
    // Arrange: Set up the initial state
    MyComponent component;
    int initialValue = 5;

    // Act: Call the function to be tested
    int result = component.doSomething(initialValue);

    // Assert: Check if the result is what you expect
    TEST_ASSERT_EQUAL(10, result);
}

void setup() {
    delay(2000); // A short delay to allow the serial monitor to connect
    UNITY_BEGIN();
    RUN_TEST(test_my_component_does_something);
}

void loop() {
    UNITY_END();
}
```

By following these guidelines, we can build a high-quality, reliable, and well-tested framework together.
