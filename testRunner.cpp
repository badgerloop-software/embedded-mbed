#include "mbed.h"

int main()
{
    char address = 0x22;
    I2C i2c(I2C_SDA, I2C_SCL);
    char out = 'a';
    char * data = &out;

    while (true) {
        i2c.write(address, data, 2, false);
    }
}