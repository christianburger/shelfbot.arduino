#pragma once
#include <Arduino.h>
#include <Wire.h>
#include "ShelfbotLogger.h"

namespace shelfbot_i2c {

#define SDA_PIN 4  
#define SCL_PIN 2  
#define I2C_FREQ 10000  
#define I2C_SLAVE_ADDR 8 

class I2CSlave {
public:
    I2CSlave();
    void begin();
    bool hasNewData();
    byte getLastReceivedData();
    void setDataToSend(byte data);
    void testPins();
    void handleReceive(int numBytes);
    void handleRequest();
    
private:
    static I2CSlave* instance;
    static void onReceiveWrapper(int numBytes);
    static void onRequestWrapper(void);
    volatile byte _receivedData;
    volatile bool _newDataReceived;
    byte _dataToSend;
    shelfbot_logging::ShelfbotLogger logger;
};

}