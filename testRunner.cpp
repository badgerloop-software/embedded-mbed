#include "mbed.h"
#include "ethernet.h"
#include "dataFormat.h"


void dataFrameSender(int *size, void** data) {
    *size = TOTAL_BYTES;
    copyDataStructToWriteStruct();
    *data = &dfwrite;
}

int main(void){
    puts("===TEST CODE STARTING===");
    EthernetClient es("192.168.1.16", 4005, NULL, dataFrameSender);
    es.run();
    while(1){
        // Idle eternally and let the default data handlers do the work
        wait_us(10000000);
    }
}