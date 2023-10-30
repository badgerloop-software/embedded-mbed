# Tests
This is where all test classes will be defined.

## Creating a New Test Class
All custom test classes must inherit the base `Test` class (see [`test.h`](test.h)), as it defines a standard test structure that is required for communicating with the [BTF](https://github.com/badgerloop-software/BTF) test runner. Additionally, `test.h` provides a declaration of `pc`, a `SerialWrapper` object for communicating with the BTF test runner. The following is an example custom test class with implementation notes:
```cpp
#ifndef __CUSTOM_TEST__
#define __CUSTOM_TEST__

#include <cstring>
#include "test.h"


class CustomTest : public Test {
    public:
        /**
         * REQUIRED: Pass the name of the test to the base class' constructor. The test name
         *           should be formatted as "<driver>_<test_name>", using the keys from tests.yml
         *
         * NOTE: If multiple tests have the same setup() and teardown(), you could use
         *       `CustomTest(const string name): Test(name) {` to run different tests in runTest()
         *       based on the name and save on redefining setup() and teardown()
         */
        CustomTest(): Test("custom_drv_test") {
            // Add initialization here
        };
    private:
        /**
         * OPTIONAL: Test setup. If not defined, there is no setup
         *
         * NOTE: This is called after BTF test setup and execution
         */
        void setup() {
            // Add custom setup here
        }

        /**
         * REQUIRED: Main test content
         *
         * Returns true if test passes, false otherwise
         */
        bool runTest() {
            char buf[5];
            // You can use printf() and the pc object to communicate with the BTF test runner
            printf("foo\n"); // pc.write() also works
            pc.readLine(buff, sizeof(buff));
            return (strcmp(buff, "bar") == 0);
        }

        /**
         * OPTIONAL: Test teardown. If not defined, there is no teardown
         *
         * NOTE: This is called after BTF test conclusion and teardown
         */
        void teardown() {
            // Add custom teardown here
        }
};

#endif // __CUSTOM_TEST__
```