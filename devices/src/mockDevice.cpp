#include "mockDevice.h"

// register offsets
#define READ_REG 0x0
#define WRITE_REG 0x1

#define READ_REG_DEFAULT 0xFF

/* Reads 1 byte from a register
 */
uint8_t MOCKDEVICE::read_from_reg(uint8_t reg) {
    char data;
    readI2CWrapper(reg, data, 1);
    return data;
}

/* Write 2 bytes to a register.
 */
void MOCKDEVICE::write_to_reg(uint8_t reg, uint8_t val) {
    char data = val;
    writeI2CWrapper(reg, data, 1);
}

MOCKDEVICE::MOCKDEVICE(I2C *bus, uint8_t addr) : I2CDevice(bus, addr) {

}

/* Checks the value of the read register.
 * Returns 0 on success, 1 if an error occurred
 */
int MOCKDEVICE::begin() {
    // check value of read register to make sure this is the right device. 
    if (read_from_reg(READ_REG) != READ_REG_DEFAULT) {
        printf("Error reading from read register.\n");
        return 1;
    }
    return 0; // successful begin
}

/* Returns the value of the "read" register of the device
 */
uint8_t MOCKDEVICE::read_value() {
    return read_from_reg(READ_REG);
}

/* Writes a value to the "write" register of the device and
 * checks if the write was successful. 
 */
int MOCKDEVICE::write_value(uint8_t val) {
    write_to_reg(WRITE_REG, val);
    if (read_from_reg(WRITE_REG) != val) {
        return 1; // error: value read doesn't match value written
    }
    return 0; // successful write
}