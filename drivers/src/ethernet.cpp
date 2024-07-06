#include "ethernet.h"
#include "EthernetInterface.h"
#include "NetworkInterface.h"
//#include "Socket.h"
#include "SocketAddress.h"
#include "TCPSocket.h"

#include "nsapi_types.h"
#include <cstdio>

/**
 * Debug print for ethernet functions
 * This lets you selectively enable debugging output
 */
void dbg(char* s) {
#ifdef EDBG
  puts(s);
#endif
}

/**
 * Initializes an EthernetServer
 * this_ip -- the IP address of the Nucleo
 * port -- the port to use
 * data_recv -- function to call when data is received
 * data_write -- function to call in order to get data to send
 */
EthernetClient::EthernetClient(const char *this_ip, int port,
                               void (*data_recv)(void *data, int size),
                               void (*data_write)(int *size, void **)) {
  init(this_ip, port, data_recv, data_write);
}

/**
 * Attempt a connection
 */
int EthernetClient::connect(void) {
  dbg("Connecting...");
  int status;
  dbg("Getting network interface");
  NetworkInterface *net = NetworkInterface::get_default_instance();
  net->disconnect();
  dbg("Disconnected from any existing connections");
  dbg("(not) Setting static IP");
  net->set_network((SocketAddress)IP,(SocketAddress)MASK,(SocketAddress)GATEWAY);
  dbg("Closing socket if already open");
  this->sock.close();
  this->connected = 0;
  status = net->connect();
#ifdef EDBG
  printf("Status: %d\n", status);
#endif
  SocketAddress mask;
  net->get_netmask(&mask);
  SocketAddress gateway;
  net->get_gateway(&gateway);
  // net->set_network(((SocketAddress)"192.168.1.15"), mask, gateway);
  SocketAddress tmp_ip;
  net->get_ip_address(&tmp_ip);
  const char *p_ip = tmp_ip.get_ip_address();
#ifdef EDBG
  printf("IP address: %s\n", p_ip ? p_ip : "None");
  puts("Opening socket...");
#endif
  if (this->sock.open(net) != NSAPI_ERROR_OK) {
    this->connected = 0;
    return -1;
  }
  dbg("Attempt: Connecting...");
  // this->sock.set_blocking(true);
  if (this->sock.connect(*(this->server_address)) < 0) {
    this->connected = 0;
    return -2;
  }
  this->sock.set_timeout(5000); // all operations can take up to 5 seconds. This is NOT a wait_ms()
  // Connected
  this->connected = 1;
  return 0;
}

void EthernetClient::init(const char *server_ip, int port,
                          void (*data_recv)(void *, int),
                          void (*data_write)(int *, void **)) {
  dbg("init");
  // Copy ip
  memmove(this->ip, server_ip, strlen(server_ip) + 1);
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

int8_t EthernetClient::run(void) {
  dbg("Starting Ethernet Client");
  Thread *read = new Thread();
  read->start(callback(this, &EthernetClient::read_thread));
  Thread *write = new Thread();
  write->start(callback(this, &EthernetClient::write_thread));
  dbg("Finished run function");
  return 0;
}

void EthernetClient::write_thread(void) {
restart:
  // puts("[WT]: starting");
  int status;
  // puts("[WT]: entering mainloop");
  int size;
  void *data;
  while (1) {
    //  puts("[WT]: Begin cycle");
    if (!this->connected) {
      status = this->connect();
      if (status < 0) {
        // Not connected
        wait_us(CONNECTION_DELAY_US);
        #ifdef EDBG
        printf("Socket connection failed! Status: %d\n", status);
        #endif
        continue;
      }
      // Connected
      // Fall through to rest of routine
    }
    // puts("[WT]: getting data");
    size = 0;
    data = NULL;
    if (this->data_write) {
      // We have a callback function
      // Get a pointer to the data, and the size to use
      this->data_write(&size, &data);
    } else {
      // NULL data_write function, send fallback keepalive signal
      data = (void *)"keepalive";
      size = strlen((char *)data);
    }
    status = this->sock.send(data, size);
    if (status < 0) {
      #ifdef EDBG
      printf("[WT]: timeout, disconnect");
      printf("Debug information:");
      printf("NSAPI error code: %d\n", status);
      #endif
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

void EthernetClient::read_thread(void) {
#ifdef EDBG
  dbg("[RT]: Read thread starting");
#endif
  int recv_size;
  int status;
  while (1) {
    // puts("[RT]: Begin cycle");
    if (!connected) {
      // puts("no connection, stalling");
      wait_us(1000000); // Use 1 second to reduce latency
      continue;
    }
    status = this->sock.recv(this->rbuffer, 2048);
    // printf("Read returned status %d\n", status);
    if (status <= NSAPI_ERROR_OK) {
      // No data to find, or error
      // puts("Error");
      continue;
    }
    recv_size = status;
    if (this->data_recv) {
      // puts("RT: Calling data_recv");
      this->data_recv((void *)this->rbuffer, recv_size);
    } else {
      dbg("RT: Undefined data_recv. Default behaviour treat as string");
      this->rbuffer[recv_size - 1] = 0;
      printf("%s\n", this->rbuffer);
    }
  }
}