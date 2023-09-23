#ifndef __MOCKDEVICE_H__
#define __MOCKDEVICE_H__

#include "mbed.h"
#include "../../drivers/include/i2cdevice.h"

class MOCKDEVICE : public I2CDevice {
    private:
        uint8_t read_from_reg(uint8_t reg);
        void write_to_reg(uint8_t reg, uint8_t val);
    public: 
        MOCKDEVICE(I2C* bus, uint8_t addr);
        int begin();
        uint8_t read_value();
        int write_value(uint8_t val);
}

#endif
