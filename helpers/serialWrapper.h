#ifndef __SERIAL_WRAPPER_H__
#define __SERIAL_WRAPPER_H__

#include "mbed.h"


class SerialWrapper : public BufferedSerial {
    public:
        // By default, connect to USB serial
        SerialWrapper(PinName tx=USBTX, PinName rx=USBRX, int baud=MBED_CONF_PLATFORM_DEFAULT_SERIAL_BAUD_RATE): BufferedSerial(tx, rx, baud) {};
        ssize_t readLine(char* dest, size_t buffSize, int timeout=10000);
};

#endif // __SERIAL_WRAPPER_H__