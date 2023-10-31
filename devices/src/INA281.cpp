#include "INA281.h"

/*
    Initialize pin and parameters
*/
INA281Driver::INA281Driver(PinName pinName, float resistance, float scaleFactor) : iNAPin(pinName)
{
    this->resistance = resistance;
    this->scaleFactor = scaleFactor;
}

/*
    Retrieve new current reading
*/
float INA281Driver::readCurrent()
{
    float measuredVoltage = iNAPin.read() * 3.3;

    // voltage = measuredVoltage / scaleFactor
    // current = voltage / resistance
    float current = (measuredVoltage / scaleFactor) / resistance;

    return current;
}

/*
    Retrieve new voltage reading
*/
float INA281Driver::readVoltage()
{
    float measuredVoltage = iNAPin.read() * 3.3;

    float voltage = measuredVoltage / scaleFactor;

    return voltage;
}