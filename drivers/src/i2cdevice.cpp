#include "i2cdevice.h"

I2CDevice::I2CDevice(I2C* i2cBus, uint8_t addr) {
    this->i2cBus = i2cBus;
    this->addr = addr << 1;
}

/* Reads bytes from the specified register at the specified address
 * It does this by first writing the register offset to the address of the device, then
 * reading the number of bytes from that address. 
 * 
 * return 0 if no errors, 1 if there were errors. 
 */
int I2CDevice::readI2CWrapper(uint8_t reg, char buffer[], int numBytes) {
    char writeBuffer[1] = {reg};
    // check if the I2C write and read throw errors. 
    if (this->i2cBus->write(this->addr, writeBuffer, 1) != 0) {
        return 1; 
    }
    // the read() function sets bit 0 of addr to 1 for a read 
    if (this->i2cBus->read(this->addr, buffer, numBytes) != 0) {
        return 1; 
    }
    return 0; // no errors
}

/* Writes bytes to the specified register at the specified address
 * Does this by prepending the register offset to the buffer, then writing that 
 * to the address of the device. 
 * 
 * returns 0 if no errors, 1 if there were errors
 */
int I2CDevice::writeI2CWrapper(uint8_t reg, char data[], int numBytes) {
    // create a new buffer which is just the input buffer with the register offset prepended.
    char newBuffer[numBytes + 1];
    newBuffer[0] = reg;
    memcpy(newBuffer+1, data, numBytes);
    // check if the I2C write throws an error 
    if(this->i2cBus->write(this->addr, newBuffer, numBytes + 1)) {
        return 1;
    }
    return 0; // no errors 
}

/* Prints the addresses of the I2C devices to console
 * This code was taken from 
 * https://os.mbed.com/users/MACRUM/code/i2cdetect//file/0ca27c666888/main.cpp/
 */
void I2CDevice::i2cdetect() {
    char buf[10];
    buf[0] = 0;
    printf("\n     0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f\n");
    for (int adrs = 0; adrs < 0x80; adrs += 16) {
        printf("%02x: ", adrs);
        for (int i = 0; i < 16; i++) {
            this->i2cBus->write(((adrs + i) << 1), buf, 1);
            int ret = this->i2cBus->read(((adrs + i) << 1), buf, 2);
            if ((adrs+i) == 0 || (adrs+i) == 1 || (adrs+i) == 2) {
                printf("   ");
                continue;
            }
            if ((adrs+i) >= 0x78) {
                printf("\n");
                break;
            }
            if (ret == 1) {
                printf("-- ");
            }
            if (ret == 0) {
                printf("%02x ", (adrs + i));
            }
        }
        printf("\n");
    }
}