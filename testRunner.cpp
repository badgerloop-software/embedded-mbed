#include "mbed.h"

BufferedSerial pc(USBTX, USBRX);


int main()
{
    char buff[20] = "";

    do {
        if(pc.readable()) {
            // TODO ThisThread::sleep_for(2000);
            ThisThread::sleep_for(100);
            pc.read(buff, sizeof(buff));
            // TODO pc.write(buff, sizeof(buff));
        //} else {
        //    printf("Not readable\n");
        }
    } while(strlen(buff) == 0);

    printf("Received: %s\n", buff);
    if(strncmp(buff, "Start now", strlen(buff)) == 0) {
        printf("Start tests\n");
    }
    while(1) {}
}
