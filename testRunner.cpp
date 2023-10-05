#include "mbed.h"

BufferedSerial pc(USBTX, USBRX);


int main()
{
    printf("Hello World!\n");
    char buff[20] = "";

    if(!pc.readable()) {
        printf("Not readable\n");
    }
    while(1) {
        if(pc.readable()) {
            ThisThread::sleep_for(2000);
            pc.read(buff, sizeof(buff));
            if(strlen(buff) > 0) {
                printf("Received: %s\n", buff);
            }
            // TODO pc.write(buff, sizeof(buff));
        }
    }
}
