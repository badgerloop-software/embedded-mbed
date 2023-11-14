#include "INA.h"
#include <cstdint>

    INADriver::INADriver(I2C* p_i2cBus, uint8_t p_subordinate_address, uint8_t p_shunt_resistor) : I2CDevice(p_i2cBus, p_subordinate_address) {
        shunt_resistor = p_shunt_resistor;

        //Test write call
        char buffer[2];
        readI2CWrapper(0x00, buffer, 2);
        if ( buffer[0] != DEFAULT__CONFIG_STATE_LOW && buffer[1] != DEFAULT__CONFIG_STATE_HIGH) {
            error_code = I2C_BUS_ERROR;
        }
    }

    int INADriver::readRegisterINA(INARegister p_reg, char p_buffer[], int p_numBytes) {
            if(I2C_BUS_ERROR) {
                return 1;
            }
            I2CDevice::readI2CWrapper(p_reg, p_buffer, p_numBytes);
            return 0;
    }
    int INADriver::writeRegisterINA(INARegister p_reg, char p_buffer[], int p_numBytes) {
        if(I2C_BUS_ERROR) {
            return 1;
        }
        I2CDevice::writeI2CWrapper(p_reg, p_buffer, p_numBytes);
        return 0;
    }

    //Individual R/W Registers
    int INADriver::readConfigRegister(char p_data[], int p_numBytes) {
        if(I2C_BUS_ERROR) {
            return 1;
        }
        I2CDevice::readI2CWrapper(INADriver::CONFIG, p_data, p_numBytes);
        return 0;
    }
    int INADriver::writeConfigRegister(char p_data[], int p_numBytes) {
        if(I2C_BUS_ERROR) {
            return 1;
        }
        I2CDevice::writeI2CWrapper(INADriver::CONFIG, p_data, p_numBytes);
        return 0;
    }

    int INADriver::readCalibrationRegister(char p_data[], int p_numBytes) {
        if(I2C_BUS_ERROR) {
            return 1;
        }
        I2CDevice::readI2CWrapper(INADriver::CALIBRATE, p_data, p_numBytes);
        return 0;
    }
    int INADriver::writeCalibrationRegister(char p_data[], int p_numBytes) {
        if(I2C_BUS_ERROR) {
            return 1;
        }
        I2CDevice::writeI2CWrapper(INADriver::CALIBRATE, p_data, p_numBytes);
        return 0;
    }

    int INADriver::readShuntVoltageRegister(char p_data[], int p_numBytes) {
        if(I2C_BUS_ERROR) {
            return 1;
        }
        I2CDevice::readI2CWrapper(INADriver::SHUNTVOLTAGE, p_data, p_numBytes);
        return 0;
    }

    int INADriver::readBusVoltageRegister(char p_data[], int p_numBytes)  {
        if(I2C_BUS_ERROR) {
            return 1;
        }
        I2CDevice::readI2CWrapper(INADriver::BUSVOLTAGE, p_data, p_numBytes);
        return 0;
    }

    int INADriver::readPowerRegister(char p_data[], int p_numBytes)   {
        if(I2C_BUS_ERROR) {
            return 1;
        }
        I2CDevice::readI2CWrapper(INADriver::POWER, p_data, p_numBytes);
        return 0;
    }

    int INADriver::readCurrentRegister(char p_data[], int p_numBytes) {
        if(I2C_BUS_ERROR) {
            return 1;
        }
        I2CDevice::readI2CWrapper(INADriver::CURRENT, p_data, p_numBytes);
        return 0;
    }

   double INADriver::conversionMath(uint16_t raw_data) {
        //Using defualt PGA of /8
        if((raw_data & 0x8000) == 0x1) {
            //Positive Num
            return ((double)raw_data) / 10.0;
        }
        else {
            //Negative Num
            uint16_t raw_data_2s_complement = ~raw_data + 1;
            return ((double)raw_data) / 10.0;
        }
    }

    int INADriver::resetINA() {
        if(I2C_BUS_ERROR) {
            return 1;
        }
        char buffer[1] = {0xFF};
        writeI2CWrapper(0x00, buffer, 1);
        return 0;
    }