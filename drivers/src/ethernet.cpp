#include "ethernet.h"
#include "EthernetInterface.h"
#include "NetworkInterface.h"
#include "Socket.h"
#include "SocketAddress.h"
#include "TCPSocket.h"
#include "nsapi_types.h"
#include <cstdio>

/**
 * Initializes an EthernetServer
 * this_ip -- the IP address of the Nucleo
 * client_ip -- the IP address of the Pi
 * data_recv -- function to call when data is received
 * data_write -- function to call in order to get data to send
 */
EthernetClient::EthernetClient(const char *this_ip, int port,
    void (*data_recv)(void *data, int size), void (*data_write)(int *size, void**)){
    init(this_ip, port, data_recv, data_write);
}

/**
 * Attempt a connection
 */
int EthernetClient::connect(void){
    puts("Connecting...");
    int status;
    puts("Getting network interface");
    NetworkInterface *net = NetworkInterface::get_default_instance();
    net->disconnect();
    puts("Disconnected");
    this->connected = 0;
    status = net->connect();
    printf("Status: %d\n", status);
    SocketAddress mask;
    net->get_netmask(&mask);
    SocketAddress gateway;
    net->get_gateway(&gateway);
    if(this->sock.open(net) != NSAPI_ERROR_OK){
        this->connected = 0;
        return -1;
    }
    //this->sock.set_blocking(true);
    if(this->sock.connect(*(this->server_address)) < 0){
        this->connected = 0;
        return -2;
    }
    this->sock.set_timeout(HEARTBEAT_DELAY_US); // TODO change this?
    // Connected
    this->connected = 1;
    return 0;
}

void EthernetClient::init(const char *server_ip, int port,
 void (*data_recv)(void *, int), void (*data_write)(int *, void**)){
    puts("init");
    // Copy ip
    memmove(this->ip, server_ip, strlen(server_ip)+1);
    // Callback for when data is received
    this->data_recv = data_recv;
    // Callback for when data needs to be sent
    this->data_write = data_write;
    // The TCP port to use for communication
    this->port = port;
    // Connections start off as disconnected
    this->connected = false;
    // Heap allocate this - no free needed; allocate onece and forget
    this->server_address = new SocketAddress(server_ip, port);
}

int8_t EthernetClient::run(void){
    puts("Starting Ethernet Client");
    Thread read;
    read.start(callback(this, &EthernetClient::read_thread));
    Thread *write = new Thread();
    write->start(callback(this, &EthernetClient::write_thread));
    puts("Finished run function");
    return 0;
}

void EthernetClient::write_thread(void){
restart:
    puts("[WT]: starting");
    int status;
    puts("[WT]: entering mainloop");
    int size;
    void *data;
    while(1){
        puts("[WT]: Begin cycle");
        if(!this->connected){
            status = this->connect();
            if(status < 0){
                // Not connected
                //wait_us(CONNECTION_DELAY_US);
                continue;
            }
            // Connected
            // Fall through to rest of routine
        }
        puts("[WT]: getting data");
        size = 0;
        data = NULL;
        if(this->data_write){
            // We have a callback function
            // Get a pointer to the data, and the size to use
            this->data_write(&size, &data);
        }else{
            // NULL data_write function, send fallback keepalive signal
            data = (void*)"keepalive";
            size = strlen((char*)data);
        }
        status = this->sock.send(data, size);
        if(status != NSAPI_ERROR_OK){
            puts("[WT]: timeout, disconnect");
            this->connected = 0;
        }
        wait_us(HEARTBEAT_DELAY_US);
        // Let it fall through and reconnect on next cycle


    }
    // Should never exit the loop
    puts("[WT][WARNING]: EXECUTION SHOULD NEVER REACH HERE");
    puts("Emergency restart in 10s");
    wait_us(10000);
    goto restart;

}

void EthernetClient::read_thread(void){
    puts("[RT]: Read thread starting");
    int recv_size;
    int status;
    while(1){
        puts("[RT]: Begin cycle");
        if(!connected){
            wait_us(CONNECTION_DELAY_US);
            continue;
        }
        status = this->sock.recv(this->rbuffer, 2048);
        if(status != NSAPI_ERROR_OK){
            // No data to find, or error
            continue;
        }
        if(this->data_recv){
            puts("RT: Calling data_recv");
            this->data_recv((void*)this->rbuffer, recv_size);
        }else{
            puts("RT: Undefined data_recv. Default behaviour treat as string");
            this->rbuffer[recv_size-1]=0;
            printf("%s\n",this->rbuffer);
        }
    }
    // puts("RT: Read thread starting");
    // int recv_size;
    // while(1){
    //     puts("RT: Reading data");
    //     nsapi_size_or_error_t ret = this->client_socket->recv(this->rbuffer, 2048);
    //     if(ret == NSAPI_ERROR_TIMEOUT){
    //         puts("RT: Connection timed out");
    //         this->kill = 1;
    //         return;
    //     }
    //     if(this->data_recv){
    //         puts("RT: Calling data_recv");
    //         this->data_recv((void*)this->rbuffer, recv_size);
    //     }else{
    //         puts("RT: Undefined data_recv. Default behaviour treat as string");
    //         this->rbuffer[recv_size-1]=0;
    //         printf("%s\n",this->rbuffer);
    //     }
    // }
}