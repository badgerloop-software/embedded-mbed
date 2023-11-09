#include "canmanager.h"

/*
    Queues read to be handled in queue
*/
void CANManager::queueRead() {
    this->messageQueue.call(callback(this, &CANManager::readMessage));
}


/*
    Reads message from bus and schedules handling in queue
*/
void CANManager::readMessage() {
    // Read in message
    CANMessage message;
    this->canBus.read(message);

    // Copy data to shared pointer for later access
    SharedPtr<unsigned char> data = new unsigned char[message.len];
    std::memcpy(data.get(), message.data, message.len);

    // Schedule handling of message in queue
    this->messageQueue.call(callback(this, &CANManager::readHandler), message.id, data, message.len);
}


/*
    Queues handling of error
*/
void CANManager::queueError() {
    this->messageQueue.call(callback(this, &CANManager::errorHandler));
}


/*
    Logs error and resets bus in case of fault
*/
void CANManager::errorHandler() {
    int numberTdErrors = this->canBus.tderror();
    int numberRdErrors = this->canBus.rderror();

    if (numberRdErrors || numberTdErrors) {
        printf("Cleared %i transmit errors and %i read errors\n", numberTdErrors, numberRdErrors);
        canBus.reset();
        // Need to reattach all CAN bus interrupt handlers after reset
        this->canBus.attach(callback(this, &CANManager::queueRead), CAN::RxIrq);
    }
}


/*
    Constructor initializes all constructs 
    - Create bus
    - Create EventQueue
    - Attach all interrupt handlers
*/
CANManager::CANManager(PinName rd, PinName td, int frequency, std::chrono::milliseconds errorCheckPeriod) : canBus(rd, td, frequency), messageQueue() {
    // Set up function to read on interrupt
    this->canBus.attach(callback(this, &CANManager::queueRead), CAN::RxIrq);
    // Set up function to periodically clear errors
    this->errorClearer.attach(callback(this, &CANManager::queueError), errorCheckPeriod);
}


/*
    Destructor stops all operation and frees resources
    - Unattach interrupt handlers
    - Dispatch remaining events in queue
    - Destroy EventQueue
    - Destroy bus
*/
CANManager::~CANManager() {    
    // Clear interrupt handlers
    this->canBus.attach(0, CAN::RxIrq);
    this->messageQueue.dispatch_once();
}


/*
    Sends a message with the specified data in the CAN bus
    Will retry until specified timeout
*/
int CANManager::sendMessage(int messageID, void* data, int length, std::chrono::milliseconds timeout) {
    Timer t;
    t.start();
    int returnCode = 0;
    // Retry while failing to send and less than timeout
    while (!(returnCode = this->canBus.write(CANMessage(messageID, (char*)data, length))) && t.elapsed_time() < timeout) {
        this->runQueue(1ms);
    }
    t.stop();
    return returnCode;
}


/*
    Runs queued events for the specified time duration
*/
void CANManager::runQueue(std::chrono::milliseconds duration) {
    this->messageQueue.dispatch_for(duration);
}


/*
    Wrapper for Mbed CAN filter
*/
void CANManager::filter(unsigned int id, unsigned int mask, CANFormat format, int handle) {
    this->canBus.filter(id, mask, format, handle);
}


/*
    Wrapper for Mbed CAN frequency
*/
void CANManager::frequency(int hz) {
    this->canBus.frequency(hz);
}