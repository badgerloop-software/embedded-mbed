#include "mbed.h"
#include "./drivers/include/i2cdevice.h"
#include "./devices/include/mockDevice.h"
//#include "uart.h"

// macros for the mockDevice test
#define MOCK_DEVICE_ADDR 0x11
#define MOCK_DEVICE_READ_REG_VAL 0xff 
#define MOCK_DEVICE_WRITE_VAL 0x59
#define MOCK_DEVICE_WRITE_VAL2 0x4e



int main()
{
    I2C i2cBus(PF_0, PF_1); // (sda, scl)
    printf("Start mockDevice test\n");
    // create new mockDevice
    MOCKDEVICE mockDevice(&i2cBus, MOCK_DEVICE_ADDR);
    mockDevice.i2cdetect();
    
    // do some reads to 0xff to clear the i2c data buffer (THIS IS NECESSARY)
    char data[1];
    mockDevice.readI2CWrapper(0xff, data, 1);
    printf("flush buffer, data: %d\n", data[0]);
    mockDevice.readI2CWrapper(0xff, data, 1);
    printf("flush buffer, data: %d\n", data[0]);

    // initialize mockDevice and check for errors
    if (mockDevice.begin()) {
        printf("Error initializing mockDevice\n");
    }
    // read test
    uint8_t read_val = mockDevice.read_value();
    if (read_val != MOCK_DEVICE_READ_REG_VAL) {
        printf("Read Error:\n Expected value: %d\n Value read from mockDevice: %d\n", MOCK_DEVICE_READ_REG_VAL, read_val);
    } else {
        printf("READ Successful!!!! :)\n");
    }
    // write test
    if (mockDevice.write_value(MOCK_DEVICE_WRITE_VAL)) {
        printf("MockDevice test write error: input was %d\n", MOCK_DEVICE_WRITE_VAL);
    } else {
        printf("1. WRITE Successful!!!! :)\n");
    }
    // write test
    if (mockDevice.write_value(MOCK_DEVICE_WRITE_VAL2)) {
        printf("MockDevice test write error: input was %d\n", MOCK_DEVICE_WRITE_VAL2);
    } else {
        printf("2. WRITE Successful!!!! :)\n");
    }

    printf("End of mockDevice test\n\n");

    printf("stop\n");

    //while(1) {}
}
