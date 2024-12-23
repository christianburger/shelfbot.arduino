#ifndef I2C_SLAVE_H
#define I2C_SLAVE_H

#include <Arduino.h>
#include <Wire.h>

class I2CSlave {
public:
    static const uint8_t I2C_ADDRESS = 12;
    static const uint32_t I2C_CLOCK = 10000;
    static const uint8_t MAX_MESSAGE_LENGTH = 32;

    static void begin();
    static void setResponse(const char* response);
    
private:
    static void requestCallback();
    static void receiveCallback(int numBytes);

    static void printStatus();
    
    static uint32_t requestCount;
    static uint32_t receiveCount;
    static uint32_t lastStatus;

    static char lastMessage[MAX_MESSAGE_LENGTH];
    static char response[MAX_MESSAGE_LENGTH];
};

#endif