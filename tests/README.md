# Tests
This is where all test classes will be defined.

## Creating a New Test Class
All custom test classes must inherit the base `Test` class, as it defines a standard test structure that is required for communicating with the [BTF](https://github.com/badgerloop-software/BTF) test runner. Additionally, `test.h` provides a declaration of `pc`, a `SerialWrapper` object for communicating with the BTF test runner. The following is an example custom test class with implementation notes:
```cpp
#ifndef __CUSTOM_TEST__
#define __CUSTOM_TEST__

#include "test.h"
#include <string>

using namespace std;


class CustomTest : public Test {
    public:
        // REQUIRED: Pass the name of the test to the base class' constructor
        //           The test name should be formatted as shown below, using the keys from tests.yml
        // NOTE: If multiple tests have the same setup() and teardown(), you could use
        //       `CustomTest(const string name): Test(name) {` to run different tests in runTest()
        //       based on the name and save on redefining setup() and teardown()
        CustomTest(): Test("<driver>_<test_name>") {
            // Add initialization here
        };
    private:
        // OPTIONAL: Test setup. If not defined, there is no setup
        // This is called before starting the test in the BTF
        void setup() {
            // Add custom setup here
        }

        // REQUIRED: Main test content
        // Returns true if test passes, false otherwise
        bool runTest() {
            char buf[5];
            // You can use printf() and the pc object to communicate with the BTF
            printf("foo\n"); // pc.write() also works
            pc.readLine(buff, sizeof(buff));
            if(strcmp(buff, "bar") == 0) {
                // TODO REMOVE Be sure to let the BTF know if the test passes or fails!
                // printf("PASS\n");
                return true;
            }
            // TODO REMOVE Be sure to let the BTF know if the test passes or fails!
            // printf("FAIL\n");
            return false;
        }

        // OPTIONAL: Test teardown. If not defined, there is no teardown
        // This is called after ending the test in the BTF
        void teardown() {
            // Add custom teardown here
        }
};

#endif // __CUSTOM_TEST__
```