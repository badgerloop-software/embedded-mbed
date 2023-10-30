#ifndef __TEST_H__
#define __TEST_H__

#include "mbed.h"
#include <cstring>
#include <string>
#include "serialWrapper.h"

using namespace std;

// Serial interface for BTF communication
extern SerialWrapper pc;
// Test result codes
enum resultCode {
    TEST_PASS    = 0,    // Test passed
    TEST_FAIL    = 1,    // Test failed
    TEST_NOEXIST = 2,    // Test does not exist on the BTF
    SERIAL_ERROR = 3,    // Serial communication error
    RESULT_UNDEF = 4     // Unknown result
};
// Test result information
typedef struct result {
    bool pass;           // Pass/Fail
    resultCode code;     // Detailed result code
};


class Test {
    public:
        Test(const string name) {
            this->testName = name;
        }

        // Execute the test, including any setup and teardown
        result execute() {
            result res = {false, RESULT_UNDEF};
            // Signal to BTF to start test (includes BTF test setup)
            if(signalStart(&res.code)) {
                // Run our test setup after the BTF
                setup();
                // Run test
                res.pass = runTest();
                // Signal to BTF to end test (includes BTF test teardown)
                signalEnd(&res);
                // Run our teardown after the BTF
                teardown();
            }

            return res;
        }
    protected:
        // Name of the test, as defined in tests.yml
        string testName;
    private:
        // Setup for a given test. Does nothing by default
        virtual void setup() {};
        // Main test content. Return true on success, false on failure
        virtual bool runTest() = 0;
        // Teardown for a given test. Does nothing by default
        virtual void teardown() {};

        // Signal to the BTF test runner to start the test
        // Returns bool indicating if test was started successfully
        //     - If test start fails, set code to appropriate resultCode
        bool signalStart(resultCode *code) {
            char buff[7];

            // Send start message to BTF test runner
            printf("START TEST %s\n", testName.c_str());

            // Read response from BTF test runner
            size_t nread = pc.readLine(buff, sizeof(buff));

            // Set result code
            if(nread <= 0) {
                // Error reading from serial
                *code = SERIAL_ERROR;
                return false;
            } else if(strcmp(buff, "DNE") == 0) {
                // Test does not exist on the BTF
                *code = TEST_NOEXIST;
                return false;
            }

            // The response should be "READY"
            return strcmp(buff, "READY") == 0;
        }

        // Signal to the BTF test runner to end the test
        // If test end fails, set r's result code to appropriate resultCode
        void signalEnd(result *r) {
            char buff[6];

            // Send end message to BTF test runner
            printf("%s\n", r->pass ? "PASS" : "FAIL");

            // Read response from BTF test runner
            size_t nread = pc.readLine(buff, sizeof(buff));

            // Set result code
            if(nread <= 0) {
                // Error reading from serial
                r->code = SERIAL_ERROR;
            } else if(strcmp(buff, "DONE") == 0) {
                // Ended test successfully
                r->code = r->pass ? TEST_PASS : TEST_FAIL;
            } else {
                // Unknown result
                r->code = RESULT_UNDEF;
            }
        }
};

#endif // __TEST_H__