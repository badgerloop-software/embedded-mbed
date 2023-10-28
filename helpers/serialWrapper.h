#ifndef __SERIAL_WRAPPER_H__
#define __SERIAL_WRAPPER_H__

#include "mbed.h"
#include <string.h>

// TODO REMOVE: #define BUFFER_SIZE 16

class SerialWrapper : public BufferedSerial {
    public:
        SerialWrapper(PinName tx, PinName rx, int baud=MBED_CONF_PLATFORM_DEFAULT_SERIAL_BAUD_RATE): BufferedSerial(tx, rx, baud) {};
        size_t readLine(char* dest, size_t buffSize, int timeout=10000);

    // TODO private:
    //    BufferedSerial *serial;

};

#endif // __SERIAL_WRAPPER_H__