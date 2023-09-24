#ifndef ETHERNET_H
#define ETHERNET_H

class EthernetServer{
    public:
        EthernetServer(const char *this_ip, const char* client_ip, void data_recv(void *data, int size), void *data_write(int *size));
        void init(const char *this_ip, const char* client_ip, void data_recv(void *data, int size), void *data_write(int *size));
        void run(void);
    private:
        void read_thread(void);
        void write_thread(void);
};

#endif /*ETHERNET_H*/