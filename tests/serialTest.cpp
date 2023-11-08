#ifndef __SERIAL_TEST__
#define __SERIAL_TEST__

#include "test.h"


class SerialTest : public Test {
    public:
        SerialTest(): Test("serial_basic") {};
    private:
        bool runTest() {
            char buff[25];
            // Print multiple messages to the BTF and have the BTF repeat a specific one
            // Tests readAll() in BTF
            printf("Testing serial communication\n");
            printf("Repeat this message\n");
            printf("Don't repeat this message\n");
            // Wait for BTF timeout to expire
            ThisThread::sleep_for(1000ms);
            // Read and verify response from BTF test runner
            size_t nread = pc.readLine(buff, sizeof(buff));
            bool result = strcmp(buff, "Repeat this message") == 0;

            // Print a message with a delimiter (STOP)
            // Tests readUntil() in BTF
            printf("Should only read thisSTOPNot this\n");
            // Read and verify response from BTF test runner
            nread = pc.readLine(buff, sizeof(buff));

            return result && strcmp(buff, "Should only read this") == 0;
        }
};

#endif // __SERIAL_TEST__
