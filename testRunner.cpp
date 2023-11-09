#include "mbed.h"
#include "canmanager.h"
#include <array>

// 0 for stress, 1 for mock main
#define MODE 1


#define BIG_NUC 1
#if BIG_NUC
#define CAN_RX_PIN PD_0
#define CAN_TX_PIN PD_1
#else
#define CAN_RX_PIN PA_11
#define CAN_TX_PIN PA_12
#endif

void stressTest() {
    BufferedSerial terminal(USBTX, USBRX, 115200);

    class CANBounce : public CANManager {
        public:
            CANBounce(PinName rd, PinName td, int frequency): CANManager(rd, td, frequency) {};
            
            void readHandler(int messageID, SharedPtr<unsigned char> data, int length) {
                // Variable to track number of received messages
                static unsigned long messageCount[4];
                static Timer t;
                if (messageID >= 0 && messageID <= 3) 
                    messageCount[messageID]++;
                if (t.elapsed_time() > 1000ms) {
                    printf("\033[H\033[J");
                    for (int i = 0; i < 4; i++) {
                        printf("%lu\n", messageCount[i]);
                        messageCount[i] = 0;
                    }
                    
                    
                    t.reset();
                } else {
                    t.start();
                }
            }
    };

    CANBounce bus(CAN_RX_PIN, CAN_TX_PIN, 250000);
    char data[8] = {0,0,0,0,0,0,0,0};

    while(1) {
        bus.sendMessage(0, &data, 8);
        bus.sendMessage(1, &data, 8);
        bus.sendMessage(2, &data, 8);
        bus.sendMessage(3, &data, 8);
        //bus.runQueue(10ms);
    }
}

void motorTest() {
    BufferedSerial terminal(USBTX, USBRX, 115200);
    terminal.set_blocking(false);

    class CANBounce : public CANManager {
        public:
            CANBounce(PinName rd, PinName td, int frequency): CANManager(rd, td, frequency) {};
            
            void readHandler(int messageID, SharedPtr<unsigned char> data, int length) {
                // Variable to track number of received messages
                static std::array<unsigned int, 9> messageCount;
                static Timer t;
                static bool start_timer = true;
                if (start_timer) {
                    t.start();
                    start_timer = false;
                }
                if (t.elapsed_time() > 1000ms) {
                    printf("\033[H\033[J");
                    for (auto const& entry : messageCount) {
                        printf("%i\n", entry);
                    }
                    t.reset();
                    t.start();
                }
                
                messageCount[messageID - 0x200]++;
            }
    };

    CANBounce bus(CAN_RX_PIN, CAN_TX_PIN, 250000);

    unsigned char parkBrake = 1;

    char buf[1];
    while(1) {
        if (terminal.read(buf, 1) > 0 && buf[0] == 'p') {
            parkBrake = 1 - parkBrake;
            printf("Sending %i\n", parkBrake);
            bus.sendMessage(0x100, &parkBrake, 1);
        }

        bus.runQueue(10ms);
    }
}

int main()
{
    #if MODE
    motorTest();
    #else
    stressTest();
    #endif
}