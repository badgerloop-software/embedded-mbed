#ifndef ETHERNET_H
#define ETHERNET_H

/**
Server implementation on Nucleo for communication with the pi
*/

#include "mbed.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include "limits.h"
#include "stdbool.h"

class EthernetServer{
    public:
        EthernetServer(const char *this_ip, const char* client_ip, void (*data_recv)(void *data, int size), void (*data_write)(int *size));
        void init(const char *this_ip, const char* client_ip, void (*data_recv)(void *data, int size), void (*data_write)(int *size));
        void run(void);
        char ip[32];
        char client_ip[32];
        void (*data_recv)(void *data, int size);
        void (*data_write)(int *size);
    private:
        void read_thread(void);
        void write_thread(void);
};

#endif /*ETHERNET_H*/