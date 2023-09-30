#include "ethernet.h"
#include "EthernetInterface.h"
#include "NetworkInterface.h"
#include "Socket.h"
#include "TCPSocket.h"
#include "nsapi_types.h"

/**
 * Initializes an EthernetServer
 * this_ip -- the IP address of the Nucleo
 * client_ip -- the IP address of the Pi
 * data_recv -- function to call when data is received
 * data_write -- function to call in order to get data to send
 */
EthernetServer::EthernetServer(const char *this_ip, int port,
    void (*data_recv)(void *data, int size), void (*data_write)(int *size)){
    init(this_ip, port, data_recv, data_write);
}

void EthernetServer::init(const char *this_ip, int port,
 void (*data_recv)(void *, int), void (*data_write)(int *)){
    // Copy ip
    memmove(this->ip, this_ip, strlen(this_ip)+1);
    // Callback for when data is received
    this->data_recv = data_recv;
    // Callback for when data needs to be sent
    this->data_write = data_write;
    // The TCP port to use for communication
    this->port = port;
}

int8_t EthernetServer::run(void){
    puts("Starting ethernet communication driver");
    puts("Initializing Network Interface");
    this->net = EthernetInterface::get_default_instance();
    puts("Generating server address");
    this->server_address = new SocketAddress(this->ip, this->port);
    puts("Opening socket on Ethernet Interface...");
    this->server.open(net);
    puts("Binding socket to generated address...");
    this->server.bind(*(this->server_address));
    puts("Configure as non blocking...");
    this->server.set_blocking(false);
    puts("Listening on socket...");
    this->server.listen();
    
    while(1){
        
    }

    return 0;
}