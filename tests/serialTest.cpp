#ifndef __SERIAL_TEST__
#define __SERIAL_TEST__

#include "test.h"


class SerialTest : public Test {
    public:
        SerialTest(): Test("serial_basic") {};
    private:
        bool runTest() {
            // Simply pass, as signalStart() and signalEnd() will indicate success or failure
            return true;
        }
};

#endif // __SERIAL_TEST__
