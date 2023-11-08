#include "mbed.h"
// Helper includes
#include "serialWrapper.h"
// Test includes
#include "serialTest.cpp"

// Serial interface for BTF communication
SerialWrapper pc;


int main()
{
    // Test result
    result res;

    // Verify BTF communication before running other tests
    SerialTest serialTest = SerialTest();
    if((res = serialTest.execute()).pass) {
        // Run tests
    }

    while(1) {
        wait_us(1000000);
    }
}
