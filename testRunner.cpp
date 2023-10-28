#include "mbed.h"
#include "serialWrapper.h"
#include "serialTest.cpp"

// Serial interface for BTF communication
SerialWrapper pc(USBTX, USBRX);


int main()
{
    // Test BTF communication before running other tests
    SerialTest serialTest = SerialTest();
    if(serialTest.execute()) {
        // Run tests
    }

    while(1) {
        wait_us(1000000);
    }
}
