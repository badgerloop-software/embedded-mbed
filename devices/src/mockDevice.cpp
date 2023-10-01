#include "../include/mockDevice.h"

// register offsets
#define R_REG 0x0
#define W_REG 0x1

#define READ_REG_DEFAULT 0xff

/* Reads 1 byte from a register
 */
uint8_t MOCKDEVICE::read_from_reg(uint8_t reg) {
    char data;
    readI2CWrapper(reg, &data, 1);
    return data;
}

/* Write 2 bytes to a register.
 */
void MOCKDEVICE::write_to_reg(uint8_t reg, uint8_t val) {
    static char data[1];
    data[0] = val;
    //printf("mockDevice write_to_reg values:\nreg: %d\nval: %d\n", reg, val);
    if (writeI2CWrapper(reg, data, 1)) {
        printf("writeI2CWrapper error\n");
    }
}

MOCKDEVICE::MOCKDEVICE(I2C *bus, uint8_t addr) : I2CDevice(bus, addr) {

}

/* Checks the value of the read register.
 * Returns 0 on success, 1 if an error occurred
 */
int MOCKDEVICE::begin() {
    // check value of read register to make sure this is the right device.
    uint8_t read_val = read_from_reg(R_REG);
    if (read_val != READ_REG_DEFAULT) {
        printf("MockDevice::begin()| read %d| Expected: %d.\n", read_val, READ_REG_DEFAULT);
        return 1;
    }
    return 0; // successful begin
}

/* Returns the value of the "read" register of the device
 */
uint8_t MOCKDEVICE::read_value() {
    return read_from_reg(R_REG);
}

/* Writes a value to the "write" register of the device and
 * checks if the write was successful. 
 */
int MOCKDEVICE::write_value(uint8_t val) {
    write_to_reg(W_REG, val);
    printf("(wait to read after write)\n");
    uint8_t read_val = read_from_reg(W_REG);
    if (read_val != val) {
        printf("MockDevice::write_value()| read %d| Expected: %d.\n", read_val, val);
        return 1; // error: value read doesn't match value written
    }
    return 0; // successful write
}
