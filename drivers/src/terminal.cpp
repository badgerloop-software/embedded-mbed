#include "terminal.h"

/**
* Creates a terminal object that interfaces wit the NUCLEO using BufferedSerial
* Setting the TX & RX pins to USB creates a connection to the terminal
**/
Terminal::Terminal() {
    serial = new BufferedSerial(USBTX, USBRX);
}

/**
* Reads in a line from the terminal, excluding newline
* Writes a null-terminated string into the provided destination buffer
* 
* Parameters:
*   dest: pointer to destination buffer
* 
* Returns:
*   int: length of string written to buffer or -1 on error
**/
int Terminal::read(char* dest) {
    int strlen = 0; // length of string stored in character buffer
    int err = 0; // store value returned by BufferedSerial.read()
    bool overflow = false; // calling printf within loop is too slow

    while (1) {
        // check for buffer overflow before read
        if (strlen >= BUFFER_SIZE) { overflow = true; break; }

        // read in from serial
        if ((err = serial->read(dest + strlen, BUFFER_SIZE)) > 0) {
            strlen += err;
            dest[strlen] = '\0'; // convert to string

            // check for newline
            char* newline; // location of newline
            char n[2] {0xd, '\0'}; // newline character string
            if ((newline = strstr(dest, n)) != NULL) {
                *newline = '\0'; // change \n to \0
                strlen = newline - dest;
                break;
            }
        } else if (err < 0) {
            if (strlen + err > BUFFER_SIZE) overflow = true;
            else printf("Error reading from buffer\n");
            return -1;
        }
    }

    if (overflow) { printf("Buffer overflow\n"); return -1; }
    return strlen;
}