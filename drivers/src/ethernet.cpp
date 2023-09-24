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
EthernetServer::EthernetServer(const char *this_ip, int port,
    void (*data_recv)(void *data, int size), void (*data_write)(int *size, void**)){
    init(this_ip, port, data_recv, data_write);
}

void EthernetServer::init(const char *this_ip, int port,
 void (*data_recv)(void *, int), void (*data_write)(int *, void**)){
    puts("init");
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
    puts("Run");
    // Thread read;
    // read.start(callback(this, &EthernetServer::read_thread));
    Thread *write = new Thread();
    write->start(callback(this, &EthernetServer::write_thread));
    //puts("Done");
    return 0;
}

void EthernetServer::write_thread(void){
    puts("WT: Starting ethernet communication driver");
    fflush(stdout);
    puts("WT: Initializing Network Interface");
    this->net = EthernetInterface::get_default_instance();
    puts("Disconnecting open network");
    net->disconnect();
    puts("Setting network parameters...");
    net->set_network((SocketAddress)this->ip, (SocketAddress)"255.255.255.0", (SocketAddress)"0.0.0.0");
    puts("Connecting to network");
    net->connect();
    puts("WT: Generating server address");
    this->server_address = new SocketAddress(this->ip, this->port);
    puts("WT: Opening socket on Ethernet Interface...");
    if(NSAPI_ERROR_OK != this->server.open(net)){
        puts("BAD OPEN");
    }
    puts("WT: Binding socket to generated address...");
    this->server.bind(*(this->server_address));
    puts("WT: Configure as blocking...");
    this->server.set_blocking(false);
    puts("WT: Listening on socket...");
    if(this->server.listen()==NSAPI_ERROR_NO_SOCKET){
        puts("BAD SOCKET CONFIG");
    }
    //this->server.set_timeout(INT_MAX);
    
    int size;
    void *data;
    TCPSocket *new_connection;
    nsapi_error_t a;
    Thread *read;

    // Wait for initial connection
    puts("WT: Write thread initialized.");
    puts("WT: Waiting for initial connection");
    while(1){
        new_connection = this->server.accept(&a);
        if(a == NSAPI_ERROR_OK){
            // Connection incoming!
            puts("WT: Connection incoming (initial)");
            this->client_socket = new_connection;
            this->client_socket->set_timeout(CONNECTION_TIMEOUT_US);
            read->start(callback(this, &EthernetServer::read_thread));
            puts("WT: Started read thread");
            break;
        }else{
            if(a == NSAPI_ERROR_NO_SOCKET){
                puts("no socket");
            }else{
                puts("Trying to close connection");
                //new_connection->close();
                //puts("Done");
                delete new_connection;
            }
            printf("%d\n", a);
        }
        // Otherwise wait for the heartbeat delay
        wait_us(CONNECTION_DELAY_US);
        putc('.', stdout);
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
            if(a == NSAPI_ERROR_OK){
                // Incoming connection
                puts("WT: Incoming connection");
                puts("WT: Killing read thread");
                this->kill = 1;
                read->join();
                puts("WT: Read thread exited, resetting kill flag");
                this->kill = 0;
                puts("WT: Destroying old socket");
                this->client_socket->close();
                puts("WT: Setting up connection");
                this->client_socket = new_connection;
                this->client_socket->set_timeout(CONNECTION_TIMEOUT_US);
                puts("WT: Starting new read thread");
                delete read;
                read = new Thread();
                read->start(callback(this, &EthernetServer::read_thread));
            }

            if(this->kill){
                puts("WT: Seen kill signal read thread!");
                read->join();
                // Clean up old socket
                this->client_socket->close();
                puts("WT: Connection destroyed");
                while(1){
                    new_connection = this->server.accept(&a);
                    if(a == NSAPI_ERROR_OK){
                        // Connection incoming!
                        puts("WT: Connection incoming (reconnect?)");
                        this->client_socket = new_connection;
                        this->client_socket->set_timeout(CONNECTION_TIMEOUT_US);
                        delete read;
                        read = new Thread();
                        read->start(callback(this, &EthernetServer::read_thread));
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