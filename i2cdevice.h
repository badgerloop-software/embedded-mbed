#ifndef I2CDEVICE_H
#define I2CDEVICE_H

#include <stdio.h>
#include <stdlib.h>
#include "mbed.h"
#include "stdint.h"

class I2CDevice {
    private:
        I2C* i2cBus;
        char addr;

    public:
        /// Takes 7 Bit I2C Address
        I2CDevice(I2C* i2cBus, uint8_t addr);
        void i2cdetect();
        int readI2CWrapper(uint8_t reg, char buffer[], int numBytes);
        int writeI2CWrapper(uint8_t reg, char data[], int numBytes);
};

#endif