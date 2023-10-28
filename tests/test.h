#ifndef __TEST_H__
#define __TEST_H__

#include "mbed.h"
#include <cstring>
#include <string>
#include "serialWrapper.h"

using namespace std;

// Serial interface for BTF communication
extern SerialWrapper pc;


class Test {
    public:
        Test(const string name) {
            this->testName = name;
        }

        // Execute the test, including any setup and teardown
        bool execute() {
            bool ret;
            setup();
            if(signalStart()) {
                ret = runTest();
                // Even if the test passed, modify the return value to indicate serial
                // communication status. This is particularly useful in verifying the
                // success of the initial serial test
                ret = signalEnd(ret);
            } else {
                ret = false;
            }
            teardown();

            return ret;
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
        bool signalStart() {
            char buff[7];

            // Send start message to BTF test runner
            printf("START TEST %s\n", testName.c_str());

            // Read response from BTF test runner
            size_t nread = pc.readLine(buff, sizeof(buff));

            // The response should be "READY"
            return (nread == 5) && (strcmp(buff, "READY") == 0);
        }

        // Signal to the BTF test runner to end the test
        bool signalEnd(bool result) {
            char buff[6];

            // Send end message to BTF test runner
            printf("%s\n", result ? "PASS" : "FAIL");

            // Read response from BTF test runner
            size_t nread = pc.readLine(buff, sizeof(buff));

            // The response should be 'DONE'. If so, preserve the test result
            return result && (nread == 4) && (strcmp(buff, "DONE") == 0);
        }
};

#endif // __TEST_H__