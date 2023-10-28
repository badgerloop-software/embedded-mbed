#include "mbed.h"
// Helper includes
#include "serialWrapper.h"
// Test includes
#include "serialTest.cpp"

// Serial interface for BTF communication
SerialWrapper pc;


int main()
{
    // Verify BTF communication before running other tests
    SerialTest serialTest = SerialTest();
    if(serialTest.execute()) {
        // Run tests
    }

    while(1) {
        wait_us(1000000);
    }
}
