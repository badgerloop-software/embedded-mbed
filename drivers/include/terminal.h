#ifndef TERMINAL_H
#define TERMINAL_H

#include "mbed.h"
#include <string.h>

#define BUFFER_SIZE 16

class Terminal {
    private:
        BufferedSerial *serial;

    public:
        int read(char* dest);
        Terminal();
};

#endif