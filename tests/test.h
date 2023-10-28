#ifndef __TEST_H__
#define __TEST_H__

#include "mbed.h"
#include "serialWrapper.h"
#include <string>

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
        // Main test content
        virtual bool runTest() = 0;
        // Teardown for a given test. Does nothing by default
        virtual void teardown() {};

        // Signal to the BTF test runner to start the test
        bool signalStart() {
            // REMOVE bool started = false;
            // REMOVE int attempts = 0;
            char buff[7];

            // Send start message to BTF test runner
            printf("START TEST %s\n", testName.c_str());

            // Read response from BTF test runner
            // REMOVE Need this? memset(buff, '\0', sizeof(buff));
            size_t nread = pc.readLine(buff, sizeof(buff));

            // The response should be "READY"
            return (nread == 5) && (strcmp(buff, "READY") == 0);
            /* REMOVE
            do {

                // TODO Change to readLine with timeout

                if(pc.readable()) {
                    pc.read(buff, sizeof(buff));
                    started = strncmp(buff, "READY", 5) == 0;
                } else {
                    ThisThread::sleep_for(100);
                    attempts++;
                }
            } while(!started && (attempts < 100));

            return started;*/
        }

        // Signal to the BTF test runner to end the test
        bool signalEnd(bool result) {
            // REMOVE bool ended = false;
            // REMOVE int attempts = 0;
            char buff[6];

            // Send end message to BTF test runner
            if(result) {
                printf("PASS\n");
            } else {
                printf("FAIL\n");
            }

            // Read response from BTF test runner
            // REMOVE Need this? memset(buff, '\0', sizeof(buff));
            size_t nread = pc.readLine(buff, sizeof(buff));

            // The response should be 'DONE'. If so, preserve the test result
            return result && (nread == 4) && (strcmp(buff, "DONE") == 0);
            
            /* REMOVE
            do {

                // TODO Change to readLine with timeout

                if(pc.readable()) {
                    pc.read(buff, sizeof(buff));
                    ended = strncmp(buff, "DONE", 4) == 0;
                } else {
                    ThisThread::sleep_for(100);
                    attempts++;
                }
            } while(!ended && (attempts < 100));

            return ended;*/
        }
};

#endif // __TEST_H__