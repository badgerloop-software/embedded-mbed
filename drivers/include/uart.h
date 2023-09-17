#ifndef __BADGERLOOP_UART_H__
#define __BADGERLOOP_UART_H__

#include "mbed.h"

/* initUart
 * Initializes UART connection by attaching an RX callback to the serial channel
 */
int initUart();


/* readUart
 * @param void *buffer - Places read data here
 * @param unsigned int maxNbBytes - Size of the buffer
 * @return int - The number of bytes we actually read
 * 
 * Reads data from the nucleo's UART buffer. If there is nothing in the buffer, the read doesn't occur.
 */
int readUart(void *buffer, unsigned int maxNbBytes);

/* readUartTimeout
 * @param void *buffer - Places read data here
 * @param unsigned int maxNbBytes - Size of the buffer
 * @param unsigned int timeOut_ms - time to wait before giving up the read
 * @param unsigned int sleepDuration_us - time in between each read attempt
 * 
 * Attempts to read data from the nucleo's UART buffer during the timeout's duration. Returns when it has read enough bytes. 
 * If there is nothing in the buffer, the read doesn't occur.
 *  
 */
int readUartTimeout(void *buffer, unsigned int maxNbBytes, unsigned int timeOut_ms, unsigned int sleepDuration_us);

/* writeUart
 * @param void *buffer - data packet to send
 * @param unsigned int maxNbBytes - number of bytes to write
 *
 * Writes a data packet to the UART buffer
 */
void writeUart(void *buffer, unsigned int maxNbBytes);


/* flushBuffer
 * 
 * flushes (clears) the UART buffer
 */
void flushBuffer();


/*!  \class     timeOut
     \brief     This class can manage a timer which is used as a timeout.
   */
// Class timeOut
class timeOut {
 public:
  // Constructor
  timeOut();

  // Init the timer
  void initTimer();

  // Return the elapsed time since initialization
  unsigned long int elapsedTime_ms();

 private:
  // Used to store the previous time (for computing timeout)
  struct timeval previousTime;
};

#endif