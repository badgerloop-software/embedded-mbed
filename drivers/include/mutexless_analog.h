#include "mbed.h"

/*
    Overloaded Analog pin classes for use in interrupts
    Usage is analogous MbedOS Analog classes
*/
class AnalogInMutexless : public AnalogIn {
    public:
        AnalogInMutexless(PinName pin) : AnalogIn(pin) { }
        virtual void lock() { }
        virtual void unlock() { }
};

class AnalogOutMutexless : public AnalogOut {
    public:
        AnalogOutMutexless(PinName pin) : AnalogOut(pin) { }
        virtual void lock() { }
        virtual void unlock() { }
};