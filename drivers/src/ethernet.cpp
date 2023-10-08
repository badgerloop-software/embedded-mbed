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
    void (*data_recv)(void *data, int size), void (*data_write)(int *size, void**)){
    init(this_ip, port, data_recv, data_write);
}

void EthernetServer::init(const char *this_ip, int port,
 void (*data_recv)(void *, int), void (*data_write)(int *, void**)){
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
    // Thread read;
    // read.start(callback(this, &EthernetServer::read_thread));
    Thread write;
    write.start(callback(this, &EthernetServer::write_thread));
    return 0;
}

void EthernetServer::write_thread(void){
    puts("WT: Starting ethernet communication driver");
    puts("WT: Initializing Network Interface");
    this->net = EthernetInterface::get_default_instance();
    puts("WT: Generating server address");
    this->server_address = new SocketAddress(this->ip, this->port);
    puts("WT: Opening socket on Ethernet Interface...");
    this->server.open(net);
    puts("WT: Binding socket to generated address...");
    this->server.bind(*(this->server_address));
    puts("WT: Configure as non blocking...");
    this->server.set_blocking(false);
    puts("WT: Listening on socket...");
    this->server.listen();
    
    int size;
    void *data;
    TCPSocket *new_connection;
    nsapi_error_t a;
    Thread read;

    // Wait for initial connection
    puts("WT: Write thread initialized.");
    puts("WT: Waiting for initial connection");
    while(1){
        new_connection = this->server.accept(&a);
        if(a != NSAPI_ERROR_WOULD_BLOCK){
            // Connection incoming!
            puts("WT: Connection incoming (initial)");
            this->client_socket = new_connection;
            this->client_socket->set_timeout(CONNECTION_TIMEOUT_US);
            read.start(callback(this, &EthernetServer::read_thread));
            puts("WT: Started read thread");
            break;
        }
        // Otherwise wait for the heartbeat delay
        wait_us(CONNECTION_DELAY_US);
    }

    
    while(1){
        puts("WT: Write cycle started");

        while(1){
            // Write thread portion
            // First, delay by the heartbeat delay
            wait_us(HEARTBEAT_DELAY_US);
            // Write data
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
            //TODO write data
            a = this->client_socket->send(data, size);
            if(a == NSAPI_ERROR_TIMEOUT){
                puts("WT: Connection timed out, sending kill signal");
                this->kill = 1;
            }

            // Check for any incoming connections
            new_connection = this->server.accept(&a);
            if(a != NSAPI_ERROR_WOULD_BLOCK){
                // Incoming connection
                puts("WT: Incoming connection");
                puts("WT: Killing read thread");
                this->kill = 1;
                read.join();
                puts("WT: Read thread exited, resetting kill flag");
                this->kill = 0;
                puts("WT: Destroying old socket");
                this->client_socket->close();
                puts("WT: Setting up connection");
                this->client_socket = new_connection;
                this->client_socket->set_timeout(CONNECTION_TIMEOUT_US);
                puts("WT: Starting new read thread");
                read.start(callback(this, &EthernetServer::read_thread));
            }

            if(this->kill){
                puts("WT: Seen kill signal read thread!");
                read.join();
                // Clean up old socket
                this->client_socket->close();
                puts("WT: Connection destroyed");
                while(1){
                    new_connection = this->server.accept(&a);
                    if(a != NSAPI_ERROR_WOULD_BLOCK){
                        // Connection incoming!
                        puts("WT: Connection incoming (reconnect?)");
                        this->client_socket = new_connection;
                        this->client_socket->set_timeout(CONNECTION_TIMEOUT_US);
                        read.start(callback(this, &EthernetServer::read_thread));
                        puts("WT: Restarted read thread");
                        break;
                    }
                    // Otherwise wait for the heartbeat delay
                    // TODO change this?
                    wait_us(CONNECTION_DELAY_US);
                }
            }

            // Done


        }
    }
    

}

void EthernetServer::read_thread(void){
    puts("RT: Read thread starting");
    int recv_size;
    while(1){
        puts("RT: Reading data");
        nsapi_size_or_error_t ret = this->client_socket->recv(this->rbuffer, 2048);
        if(ret == NSAPI_ERROR_TIMEOUT){
            puts("RT: Connection timed out");
            this->kill = 1;
            return;
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
}