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
    Logs error and resets bus in case of fault
*/
void CANManager::errorHandler() {
    int numberTdErrors = this->canBus.tderror();
    int numberRdErrors = this->canBus.rderror();

    printf("Cleared %i transmit errors and %i read errors\n", numberTdErrors, numberRdErrors);
    canBus.reset();
}


/*
    Constructor initializes all constructs 
    - Create bus
    - Create EventQueue
    - Attach all interrupt handlers
*/
CANManager::CANManager(PinName rd, PinName td, int frequency) : canBus(rd, td, frequency), messageQueue() {
    // Set up function to read on interrupt
    using IrqType = mbed::interface::can::IrqType;
    this->canBus.attach(callback(this, &CANManager::queueRead), IrqType::RxIrq);

    // Set up error clearing on interrupt
    this->canBus.attach(callback(this, &CANManager::errorHandler), IrqType::BeIrq);
    this->canBus.attach(callback(this, &CANManager::errorHandler), IrqType::EpIrq);
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
    using IrqType = mbed::interface::can::IrqType;
    this->canBus.attach(0, IrqType::RxIrq);
    this->canBus.attach(0, IrqType::BeIrq);
    this->canBus.attach(0, IrqType::EpIrq);

    this->messageQueue.dispatch_once();
}


/*
    Sends a message with the specified data in the CAN bus
*/
void CANManager::sendMessage(int messageID, SharedPtr<unsigned char> data, int length) {
    this->canBus.write(CANMessage(messageID, data.get(), length));
}


/*
    Runs queued events for the specified time duration
*/
void CANManager::doRead(std::chrono::milliseconds duration) {
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