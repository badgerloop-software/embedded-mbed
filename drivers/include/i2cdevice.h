#ifndef I2CDEVICE_H
#define I2CDEVICE_H

#include <stdio.h>
#include <stdlib.h>
#include "mbed.h"
#include "stdint.h"

/*
    Wrapper for Mbed OS I2C class with nicer r/w operations
    This class is designed to be extended by device drivers and
    is linked to a single I2C IC/address
*/
class I2CDevice {
    private:
        I2C* i2cBus;
        char addr;

    public:
        /// Takes 7 Bit I2C Address
        I2CDevice(I2C* i2cBus, uint8_t addr);

        /// Prints out all devices on the i2cBus
        void i2cdetect();

        /// Reads the specified IC register into the buffer
        int readI2CWrapper(uint8_t reg, char buffer[], int numBytes);

        /// Writes the data into the register
        int writeI2CWrapper(uint8_t reg, char data[], int numBytes);
};

#endif