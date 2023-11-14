#ifndef __INA_H__
#define __INA_H__

#include "mbed.h"
#include <cstdint>
#include <stdio.h>
#include <stdlib.h>
#include "stdint.h"
#include "i2cdevice.cpp"


//A class which represents an instance of an INA Driver for the i2c commmuncation
//between the INA voltage/current sensor and the nucleo MCU
// Master (Nucleo MCU)
// Subordinate (INA Sensor)
////////////////////////////////////////////////////////////////////////////////////
// INA by default
//      without programming the INA sensor we can read the shunt voltage without any configurations
//      as well, we know the value of the shunt resistor so we are able to calculate the shunt current and power using
//      V = IR and P = IV
//
// Registers: 
//
// Config Register
// Bit 15: Reset Bit - Setting this bit to '1' generates a system reset that is the same as power-on reset. Resets all registers to default
//                     values; this bit self-clears.
// Bit 13: Bus voltage range - 0=16V, 1=32V (1 is default)
// Bit 11,12: PGA for shunt voltage - default is 11 or (+/- 320mV) PGA defaults to /8
// Bit 10-7: BADC Bus ADC Resolution/averaging (this is for the Bus ADC) 
// Bit 6-3: Shunt ADC resolution/averaging (12 bit 532 microsecond by default)
// Bit 2-0: Operating Mode (shunt and bus, continuous) by default
//
// Shunt Voltage Register
//  Stores the current voltage reading Vshunt. This is a 2's complement format. Theres a helpful table in the INA datasheet for reaeding
//  this value. Take note we are using PGA/8 by default.
//
// Bus Voltage Register
//  Stores the most recent bus voltage reading Vbus.
//  Bits 15-3 represent the voltage
//  Bit 1 - Conversion ready flag - clears when reading power register - sets when all conversions, averaging, multipilication is done
//  Bit 0 - Math Overflow flag - set when the Power or Current calculations are out of range
//
// Power Register
//  Stores the power in watts. Is calulated by multipying the current with the bus voltage. use eq 5 in the datasheet
//
// Current Register
//  Current register is determined by multiplying the value in the shunt voltage with the value in the calibration register. Eq 4
//
// Calibration Register
//  Calulated from Eq 1. calibration = trunc(0.04096/(current_LSB * Rshunt)) where current_lsb = (max expected current)/2^15
//  After programming this register the current and power registers will populate with their respected equations (4 and 5)
//////////////////////////////////////////////////////////////////////////////////////////
// Note: implementation of the INADriver class does not require that you program the calibration register to allow the power
//       and current registers but it may be recommended. Since we know the shunt resistor value and we can measure the shunt
//       voltage by default we can calculate the current and power with V=IR and P=IV. However, a good practice would be to
//       use the current and power registers and confirm that the calculated values for P and I are in good range with the 
//       measure values.
//

#define DEFAULT__CONFIG_STATE   0x399F
#define DEFAULT__CONFIG_STATE_LOW   0x9F
#define DEFAULT__CONFIG_STATE_HIGH  0x39

#define I2C_BUS_ERROR    0xFF

class INADriver : public I2CDevice{
    private:
        uint8_t shunt_resistor;
        uint8_t error_code = 0;

    public: 

    enum INARegister : uint8_t {
        CONFIG = 0x00,
        SHUNTVOLTAGE = 0x01,
        BUSVOLTAGE = 0x02,
        POWER = 0x03,
        CURRENT = 0x04,
        CALIBRATE = 0x05
    };

    //Constructor
    // Takes in the I2C bus pointer as well as the subordinate address. Shunt resistor is used for calulations
    INADriver(I2C* p_i2cBus, uint8_t p_subordinate_address, uint8_t p_shunt_resistor);

    //GENERAL R/W Registers
    int readRegisterINA(INARegister p_reg, char p_buffer[], int p_numBytes);
    int writeRegisterINA(INARegister p_reg, char p_buffer[], int p_numBytes);

    //Individual R/W Registers
    int readConfigRegister(char p_data[], int p_numBytes);
    int writeConfigRegister(char p_data[], int p_numBytes);

    int readCalibrationRegister(char p_data[], int p_numBytes);
    int writeCalibrationRegister(char p_data[], int p_numBytes);

    int readShuntVoltageRegister(char p_data[], int p_numBytes);

    int readBusVoltageRegister(char p_data[], int p_numBytes);

    int readPowerRegister(char p_data[], int p_numBytes);

    int readCurrentRegister(char p_data[], int p_numBytes);

    double conversionMath(uint16_t raw_data);

    //Set bit 15 of reg 0x00 to reset
    //We only call this if we have a valid bus (a.k.a. constructor worked)
    int resetINA();
    //Calulate Current from voltage I = V/R
    uint16_t calculateVoltage(uint16_t p_voltage) {
    // this is kinda sudo code. may need to change variable types
        return p_voltage / shunt_resistor;
    }
    
    //Calculate Power from voltage P=VI
    uint16_t calculatePower(uint16_t p_voltage) {
        return (p_voltage / shunt_resistor) * p_voltage;
    }
    //Destructor
    ~INADriver();

};


#endif