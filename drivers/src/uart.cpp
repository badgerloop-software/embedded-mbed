#include "mbed.h"
#include "uart.h"
#include <cstdint>


static const int BAUD = 115200;
static const int BUFF_SIZE = 512;
static int isInit = 0;

static UnbufferedSerial uart(PG_14, PG_9, BAUD); // (tx, rx)
static CircularBuffer<char, BUFF_SIZE, unsigned int> rxBuff;

void callback() {
    if (!rxBuff.full()) {
        char buff[1];
        uart.read(buff, 1);
        rxBuff.push(buff[0]);
    }
}

int initUart() { 
    void (*fptr)() = (void (*)()) &callback;
    uart.attach(fptr);
    isInit = 1;
    return 0;
}

int readUart(void *buffer, unsigned int maxNbBytes) {
    if (!isInit) return -1;
    NVIC_DisableIRQ(USART1_IRQn);
    uint8_t *buff = (uint8_t *)buffer;
    for (int i = 0; i < maxNbBytes; i++) {
        char data;
        if (rxBuff.empty()) {
            NVIC_EnableIRQ(USART1_IRQn);
            return i;
        }
        rxBuff.pop(data);
        buff[i] = data;
    }
    NVIC_EnableIRQ(USART1_IRQn);
    return maxNbBytes;
}

int readUartTimeout(void *buffer, unsigned int maxNbBytes, unsigned int timeOut_ms, unsigned int sleepDuration_us) {
  // Timer used for timeout
  timeOut timer;
  // Initialise the timer
  timer.initTimer();
  unsigned int NbByteRead = 0;
  // While Timeout is not reached
  while (timer.elapsedTime_ms() < timeOut_ms || timeOut_ms == 0) {
    if(!rxBuff.empty()) {
        NVIC_DisableIRQ(USART1_IRQn);
        // check if there's enough bytes in the buffer to read
        if(rxBuff.size() >= maxNbBytes) {
            NbByteRead = readUart(buffer, maxNbBytes); // call the other read function to read.
            NVIC_EnableIRQ(USART1_IRQn); 
            return NbByteRead;
        }
        NVIC_EnableIRQ(USART1_IRQn);
    }
    // Suspend the loop to avoid charging the CPU
    wait_us(sleepDuration_us);
  }
  // Timeout reached, return the number of bytes read
  return NbByteRead;
}

void writeUart(void *buffer, unsigned int maxNbBytes) {
    if (!isInit) return;
    uart.write(buffer, maxNbBytes);
}

void flushBuffer() {
    rxBuff.reset();
}


// ******************************************
//  Class timeOut
// ******************************************

/*!
    \brief      Constructor of the class timeOut.
*/
// Constructor
timeOut::timeOut() {}

/*!
    \brief      Initialize the timer. It writes the current time of the day in
   the structure PreviousTime.
*/
// Initialize the timer
void timeOut::initTimer() { gettimeofday(&previousTime, NULL); }

/*!
    \brief      Returns the time elapsed since initialization.  It write the
   current time of the day in the structure CurrentTime. Then it returns the
   difference between CurrentTime and PreviousTime. \return     The number of
   microseconds elapsed since the functions InitTimer was called.
  */
// Return the elapsed time since initialization
unsigned long int timeOut::elapsedTime_ms() {
  // Current time
  struct timeval CurrentTime;
  // Number of seconds and microseconds since last call
  int sec, usec;

  // Get current time
  gettimeofday(&CurrentTime, NULL);

  // Compute the number of seconds and microseconds elapsed since last call
  sec = CurrentTime.tv_sec - previousTime.tv_sec;
  usec = CurrentTime.tv_usec - previousTime.tv_usec;

  // If the previous usec is higher than the current one
  if (usec < 0) {
    // Recompute the microseonds and substract one second
    usec = 1000000 - previousTime.tv_usec + CurrentTime.tv_usec;
    sec--;
  }

  // Return the elapsed time in milliseconds
  return sec * 1000 + usec / 1000;
}