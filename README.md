# INA voltage and current sensor Driver

INA Datasheet: https://www.ti.com/lit/ds/symlink/ina219.pdf?ts=1695109047714&ref_url=https%253A%252F%252Fwww.ti.com%252Fproduct%252FINA219%252Fpart-details%252FINA219AIDCNR

embedded-mbed Repo: https://github.com/badgerloop-software/embedded-mbed 

MainIO Comp Repo: https://github.com/badgerloop-software/sc1-main-io-mbed/tree/Comp2023

# Jira ticket FW-164/FW-188

Summary

Create and validate drivers for the INA voltage/current sensor

Scope

Branch of off commit df7abbf in embedded-mbed

Copy I2C wrapper driver from MainIO competition code

Write a device class for the INA219 voltage and current sensor

Should take in reference for I2C bus and shunt resistor in constructor

Getters and setters as applicable for all registers

Test class on hardware

Resources

INA Datasheet: https://www.ti.com/lit/ds/symlink/ina219.pdf?ts=1695109047714&ref_url=https%253A%252F%252Fwww.ti.com%252Fproduct%252FINA219%252Fpart-details%252FINA219AIDCNR

embedded-mbed Repo: GitHub - badgerloop-software/embedded-mbed: Firmware repository for mbed-based driver and device code. 

MainIO Comp Repo: GitHub - badgerloop-software/sc1-main-io-mbed at Comp2023

# Hardware

NUCLEO-F303K8
Nucleo main page- https://www.st.com/en/evaluation-tools/nucleo-f303k8.html

STM32F303K8TL (MCU for the Nucelo)
https://www.st.com/en/microcontrollers-microprocessors/stm32f303k8.html

INA Datasheet: https://www.ti.com/lit/ds/symlink/ina219.pdf?ts=1695109047714&ref_url=https%253A%252F%252Fwww.ti.com%252Fproduct%252FINA219%252Fpart-details%252FINA219AIDCNR




# Things to think about

DMA? to save power? I2C works with DMA controller on STM32

# TO Do
Read and write functions for all the registers that are applicable
Add math conversion stuff to read registers (is it appilcable for all read registers)
Constructor add super for i2cdevice class

Testing
Exhaustive test for all registers read and write and make sure values make sense.


