#ifndef __CAN_MANAGER_H__
#define __CAN_MANAGER_H__

#include "mbed.h"
#include "platform/SharedPtr.h"
#include <chrono>

#define DEFAULT_CAN_FREQ 250000     // Match Elcon charger
#define ERR_CHECK_INTERVAL 5000ms   // How quickly to check bus errors in ms


/*
    Class managing a Nucleo CAN Bus

    To use
    1. Extend class to implement the readHandler function
    - Example: Class that simply prints out number of messages received on read

    class CANBounce : public CANManager {
        public:
            CANBounce(PinName rd, PinName td, int frequency): CANManager(rd, td, frequency) {};
            
            void readHandler(int messageID, SharedPtr<unsigned char> data, int length) {
                // Variable to track number of received messages
                static unsigned long messageCount = 0;
                messageCount++;

                // Print out how many messages receieved in intervals of 10
                if (messageCount % 10 == 0) {
                    printf("\033[H\033[J%lu", messageCount);
                }
            }
    };

    2. Initialize on desired CAN pins from pinout using constructor
    - Can update frequency or filter using methods
    - Frequency by default is equal to DEFAULT_CAN_FREQ macro
    - Example
    CANBounce bus(PD_0, PD_1, 100000);

    3. Use sendMessage to write out to the bus at any time.
    - Example: 
    // Send CAN message with ID 0, data value 1, and length 1
    char counter[] = {0};
    bus.sendMessage(0x00, &counter, 1);

    4. Reads are automatically queued to be handled by the virtual readHandler function on interrupt
       Call the doRead function whenever the CPU is free to actually process the queued messages
    - Example
    // Process reads for up to 1ms time
    bus.runQueue(1ms);
*/
class CANManager {
    private:
        EventQueue messageQueue;    // Schedule reading messages
        CAN canBus;                 // Bus the class manages
        Ticker errorClearer;        // Periodically clears error and ensures bus is working

        // Interrupt handler to queue a readMessage (cannot do actual read in IRQ context)
        void queueRead();
        // Takes CAN message out of buffer and queues handling of messages
        void readMessage();

        // Interrupt handler to queue an errorHandler 
        void queueError();
        // Prints out number of errors and resets bus on error interrupt
        void errorHandler();

    public: 
        // Constructor initializing bus and all manager functions
        // rd: CAN rd pin of the Nucleo to use
        // td: CAN td pin of the Nucleo to use
        // frequency: Baud rate of can bus
        CANManager(PinName rd, PinName td, int frequency = DEFAULT_CAN_FREQ, std::chrono::milliseconds errorCheckPeriod = ERR_CHECK_INTERVAL);

        // Destructor stopping manager and freeing resources
        ~CANManager();

        // Reads input message and does any logic handling needed
        // Intended to be implemented by class extension per board
        virtual void readHandler(int messageID, SharedPtr<unsigned char> data, int length) = 0;

        // Send a message over CAN
        // messageID: CAN ID to use to identify the signal
        // data: Payload
        // length: Size of data in bytes
        int sendMessage(int messageID, void* data, int length, std::chrono::milliseconds timeout = 10ms);

        // Wrapper for Mbed CAN filter
        void filter(unsigned int id, unsigned int mask, CANFormat format=CANAny, int handle=0);

        // Wrapper for Mbed CAN frequency
        void frequency(int hz);

        // Processes queued reads to be handled for up to specified time
        void runQueue(std::chrono::milliseconds duration);
};


#endif // __CAN_MANAGER_H__