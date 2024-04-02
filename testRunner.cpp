#include "mbed.h"

#include "thermistor.h"

int main(void){
    Thermistor testTherm(NXRT15XH103FA1B040_Constants, PA_11);

    while(1) {
        printf("%f degrees C\n", testTherm.get_temperature());
        wait_us(1000000);

    }
}