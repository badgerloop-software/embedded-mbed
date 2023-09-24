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

#define ENET_ERR_OK         ((int8_t)0)
#define ENET_ERR_NO_CONNECT ((int8_t)1)
#define ENET_ERR_SOCK_BROKE ((int8_t)2)
#define ENET_ERR_INVALID    ((int8_t)3)

#define NETMASK     "255.255.255.0"
#define GATEWAY     "0.0.0.0"

#define HEARTBEAT_DELAY_US 50000 // 20 times per second
#define CONNECTION_DELAY_US 2000000 // 1 time per second
#define CONNECTION_TIMEOUT_US 1000000 // 1 second

// struct enet_err_t{
//     int32_t err_code;
//     char *msg;
// };

class EthernetServer{
    public:
        EthernetServer(const char *this_ip, int port, void (*data_recv)(void *data, int size), void (*data_write)(int *size, void**));
        void init(const char *this_ip, int port, void (*data_recv)(void *data, int size), void (*data_write)(int *size, void**));
        int8_t run(void);
        char ip[48];
        void (*data_recv)(void *data, int size);
        void (*data_write)(int *size, void **data);
    private:
        void read_thread(void);
        void write_thread(void);
        int port;
        char rbuffer[2048];

        NetworkInterface *net;

        SocketAddress *server_address;
        
        TCPSocket server;
        TCPSocket *client_socket;
        SocketAddress client_address;

        volatile bool kill = 0; // Set this to 1 to kill the threads manually.

        

};

#endif /*ETHERNET_H*/