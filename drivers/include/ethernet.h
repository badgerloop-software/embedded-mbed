#ifndef ETHERNET_H
#define ETHERNET_H

/**
Server implementation on Nucleo for communication with the pi
*/

#include "SocketAddress.h"
#include "mbed.h"
#include "NetworkInterface.h"

#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include "limits.h"
#include "stdbool.h"

#define EDBG

#define IP          "192.168.1.15"   //Here place your static IP of Mbed
#define GATEWAY     "192.168.1.200"
#define MASK        "255.255.0.0"

#define NETMASK     "255.255.0.0"
#define GATEWAY     "192.168.1.200"

#define HEARTBEAT_DELAY_US 100000 // 10 times per second
#define CONNECTION_DELAY_US 1000000 // 1 time per second
#define CONNECTION_TIMEOUT_US_WRITE 1000000 // 1 second

struct EthernetClient{
    public:
        EthernetClient(const char *server_ip, int port, void (*data_recv)(void *data, int size), void (*data_write)(int *size, void**));
        void init(const char *server_ip, int port, void (*data_recv)(void *data, int size), void (*data_write)(int *size, void**));
        int8_t run(void);
        char ip[48];
        void (*data_recv)(void *data, int size);
        void (*data_write)(int *size, void **data);
    private:
        int connect(void);
        void read_thread(void);
        void write_thread(void);
        int port;
        char rbuffer[2048];

        NetworkInterface *net;

        SocketAddress *server_address;
        
        TCPSocket sock;

        volatile bool connected;
};

#endif /*ETHERNET_H*/