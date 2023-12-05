#include "mbed.h"
#include "ethernet.h"


#include "mbed.h"
#include "NetworkInterface.h"
 
/*****#define BUFFSIZE 50
#define SERVERIP         "192.168.1.16"   //Here place your static IP of Mbed Server
 
//#define ROUTER // commented = Static IP uncomented = address assigned by router
#ifndef ROUTER
    #define IP          "192.168.1.15"   //Here place your static IP of Mbed
    #define GATEWAY     "0.0.0.0"
    #define MASK        "255.255.255.0"
#endif
#define PORT            4005
 
DigitalOut led(LED1);

NetworkInterface *net = NetworkInterface::get_default_instance();
TCPSocket client;
SocketAddress clientAddress;
SocketAddress serverAddress(SERVERIP, PORT);

int main (void){
    printf("TCP Client starting...\n");
    int net_stat;
// #ifndef ROUTER
//     net->disconnect();
//     net_stat = net->set_network((SocketAddress)IP,(SocketAddress)MASK,(SocketAddress)GATEWAY);
//     printf("set IP status: %i\n",net_stat);
// #endif
    net_stat = net->connect();
    printf("connect status: %i\n",net_stat);
 
    SocketAddress ip; 
    net->get_ip_address(&ip);
    const char *p_ip = ip.get_ip_address();
    printf("IP address: %s and Port: %d\n", p_ip ? p_ip : "None" , PORT );
    SocketAddress mask;
    net->get_netmask(&mask);
    const char *p_mask = mask.get_ip_address();
    printf("Netmask: %s\n", p_mask ? p_mask : "None");
    SocketAddress gateway;
    net->get_gateway(&gateway);
    const char *p_gateway = gateway.get_ip_address();
    printf("Gateway: %s\n", p_gateway ? p_gateway : "None");
    
    int scount = 0;
    int rcount = 0;
    int dummy = 0;
    char sbuffer[100];
    char rbuffer[100];
    
    while (1) {
        if(client.open(net)== NSAPI_ERROR_OK ){
            if (client.connect(serverAddress) < 0) {
                printf("Failed to connect with server\n\r");
    
            }else{
                printf("Connected to server\n");
                int n = sprintf(sbuffer,"Test String with a dummy number - %d", dummy++);
                scount = client.send(sbuffer, n);
                printf("sent [%s] - %d bytes\n", sbuffer, scount);
                rcount = client.recv(rbuffer, sizeof rbuffer);
                printf("recv [%s] - %d bytes\n", rbuffer, rcount);
                client.close();
            }
        }else{
            printf("No Server\n");
            net->disconnect();
            printf("Program end\n");
            break;
        }
        led = !led;
        thread_sleep_for(5000);
    }
}******/

int main(void){
    puts("===TEST CODE STARTING===");
    EthernetClient es("192.168.1.16", 4005, NULL, NULL);
    es.run();
    while(1){
        // Idle eternally and let the default data handlers do the work
        wait_us(10000000);
    }
}