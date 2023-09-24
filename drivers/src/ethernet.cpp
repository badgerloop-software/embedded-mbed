#include "ethernet.h"

/**
 * Initializes an EthernetServer
 * this_ip -- the IP address of the Nucleo
 * client_ip -- the IP address of the Pi
 * data_recv -- function to call when data is received
 * data_write -- function to call in order to get data to send
 */
EthernetServer::EthernetServer(const char *this_ip, const char* client_ip, 
    void (*data_recv)(void *data, int size), void (*data_write)(int *size)){
    init(this_ip, client_ip, data_recv, data_write);
}

void EthernetServer::init(const char *this_ip, const char *client_ip,
 void (*data_recv)(void *, int), void (*data_write)(int *)){
    // Copy ip
    memmove(this->ip, this_ip, strlen(this_ip)+1);
    // Copy client ip
    memmove(this->client_ip, client_ip, strlen(client_ip)+1);
    // Callback for when data is received
    this->data_recv = data_recv;
    // Callback for when data needs to be sent
    this->data_write = data_write;
}