#include "INA219.h"
#include <cstdint>

INADriver::INADriver(I2C* p_i2cBus, uint8_t p_subordinate_address, uint8_t p_shunt_resistor) : I2CDevice(p_i2cBus, p_subordinate_address) {
    shunt_resistor = p_shunt_resistor;
}


int INADriver::testConnection() {
    //Test write call
    char buffer[2];

    /// Will need to read the config register into a variable
    /// uint16_t and readI2CWrapper will probably be helpful here

    if ( buffer[0] != DEFAULT__CONFIG_STATE_LOW || buffer[1] != DEFAULT__CONFIG_STATE_HIGH) {
        return 1;
    } else {
        return 0;
    }
}

//Individual R/W Registers
int INADriver::readConfigRegister(char p_data[], int p_numBytes) {
    readI2CWrapper((uint8_t)INARegister::CONFIG, p_data, p_numBytes);
    return 0;
}
int INADriver::writeConfigRegister(char p_data[], int p_numBytes) {
    writeI2CWrapper((uint8_t)INARegister::CONFIG, p_data, p_numBytes);
    return 0;
}

int INADriver::readCalibrationRegister(char p_data[], int p_numBytes) {
    readI2CWrapper((uint8_t)INARegister::CALIBRATE, p_data, p_numBytes);
    return 0;
}
int INADriver::writeCalibrationRegister(char p_data[], int p_numBytes) {
    writeI2CWrapper((uint8_t)INARegister::CALIBRATE, p_data, p_numBytes);
    return 0;
}

int INADriver::readShuntVoltageRegister(char p_data[], int p_numBytes) {
    /// It would be better if the conversion was done here as well
    /// We will never be interested in the raw register, want the voltage it maps to
    readI2CWrapper((uint8_t)INARegister::SHUNTVOLTAGE, p_data, p_numBytes);
    return 0;
}

int INADriver::readBusVoltageRegister(char p_data[], int p_numBytes)  {
    /// It would be better if the conversion was done here as well
    /// We will never be interested in the raw register, want the voltage it maps to
    readI2CWrapper((uint8_t)INARegister::BUSVOLTAGE, p_data, p_numBytes);
    return 0;
}

int INADriver::readPowerRegister(char p_data[], int p_numBytes)   {
    readI2CWrapper((uint8_t)INARegister::POWER, p_data, p_numBytes);
    return 0;
}

int INADriver::readCurrentRegister(char p_data[], int p_numBytes) {
    /// It would be better if the conversion was done here as well
    /// We will never be interested in the raw register, want the current it maps to
    readI2CWrapper((uint8_t)INARegister::CURRENT, p_data, p_numBytes);
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
    char buffer[1] = {0xFF};
    writeI2CWrapper((uint8_t)INARegister::CONFIG, buffer, 1);
    return 0;
}