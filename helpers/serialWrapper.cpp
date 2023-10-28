#include "serialWrapper.h"


/**
* Reads in a line of text, excluding newline
* Writes a null-terminated string into the provided destination buffer
* 
* Parameters:
*   dest: Pointer to destination buffer
*   buffSize: Size of destination buffer
*     * NOTE: Although the newline is not included in the final buffer
*             contents, you must ensure that buffSize accounts for both
*             the newline and null-terminator at the end of the buffer
*   timeout: Time (in milliseconds) to wait for a line to be read
*     * If timeout is negative, this will not time out
*     * Default timeout is 10000ms (10s)
* 
* Returns:
*   size_t: Length of string written to buffer or -1 on error or timeout
**/
size_t SerialWrapper::readLine(char* dest, size_t buffSize, int timeout) {
    // Length of string stored in character buffer
    size_t strlen = 0;
    // Stores value returned by BufferedSerial.read()
    int err = 0;
    // Calling printf within loop is too slow
    bool overflow = false;
    // Whether or not there is a timeout
    bool canTimeout = timeout >= 0;

    while(1) {
        // Check for buffer overflow before read
        /*TODO if(strlen >= BUFFER_SIZE) {
            overflow = true;
            break;
        }*/

        // Read in from serial
        if((err = this->read(dest + strlen, buffSize)) > 0) {
            strlen += err;
            // TODO Check for buffer overflow after read
            if(strlen >= buffSize) {
                overflow = true;
                break;
            }
            // Convert to string
            dest[strlen] = '\0';

            // Check for newline
            char* newline; // Location of newline
            char n[2] {'\n', '\0'}; // Newline character string TODO Any particular reason 0xd was used instead of '\n'?
            if((newline = strstr(dest, n)) != NULL) {
                // Change \n to \0
                *newline = '\0';
                strlen = newline - dest;
                break;
            }
        } else if(err == 0) {
            // Nothing read
            if(canTimeout) {
                if(timeout <= 0) {
                    // Time out
                    return -1;
                }
                ThisThread::sleep_for(100ms);
                timeout -= 100;
            }
        } else {
            // Error reading from buffer
            return -1;
        }
    }

    if(overflow) {
        // Buffer overflow
        return -1;
    }

    return strlen;
}