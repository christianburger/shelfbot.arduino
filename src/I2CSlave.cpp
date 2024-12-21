#include "I2CSlave.h"

namespace shelfbot_i2c {

I2CSlave* I2CSlave::instance = nullptr;

I2CSlave::I2CSlave() : _receivedData(0), _newDataReceived(false), _dataToSend(0) {
    instance = this;
}

void I2CSlave::begin() {
    String msg;
    logger.logI2C("Starting Slave initialization");
    
    Wire.begin(I2C_SLAVE_ADDR);
    Wire.setClock(I2C_FREQ);
    
    Wire.onReceive(onReceiveWrapper);
    Wire.onRequest(onRequestWrapper);
    
    logger.logI2C("Handlers registered");
    
    msg = "SDA Pin: " + String(SDA_PIN) + " SCL Pin: " + String(SCL_PIN);
    logger.logI2C(msg.c_str());
    
    msg = "I2C Slave Ready on address 0x" + String(I2C_SLAVE_ADDR, HEX);
    logger.logI2C(msg.c_str());
}

void I2CSlave::onReceiveWrapper(int numBytes) {
    if (instance) instance->handleReceive(numBytes);
}

void I2CSlave::onRequestWrapper(void) {
    if (instance) instance->handleRequest();
}

void I2CSlave::handleReceive(int numBytes) {
    String msg;
    logger.logI2C("=== Receive Event Start ===");
    
    msg = "Receiving " + String(numBytes) + " bytes";
    logger.logI2C(msg.c_str());
    
    int bytesAvailable = Wire.available();
    msg = "Bytes available: " + String(bytesAvailable);
    logger.logI2C(msg.c_str());
    
    while(Wire.available()) {
        _receivedData = Wire.read();
        msg = "Received byte: 0x" + String(_receivedData, HEX) + " (" + String(_receivedData) + ")";
        logger.logI2C(msg.c_str());
        _newDataReceived = true;
    }
    
    logger.logI2C("=== Receive Event End ===");
}

void I2CSlave::handleRequest() {
    String msg;
    logger.logI2C("=== Request Event Start ===");
    
    msg = "Sending response byte: 0x" + String(_dataToSend, HEX) + " (" + String(_dataToSend) + ")";
    logger.logI2C(msg.c_str());
    
    Wire.write("Hello from ESP8266!...");

    logger.logI2C("=== Request Event End ===");
}

bool I2CSlave::hasNewData() {
    if (_newDataReceived) {
        String msg = "Checking for new data: " + String(_newDataReceived ? "YES" : "NO");
        logger.logI2C(msg.c_str());
    }
    return _newDataReceived;
}

byte I2CSlave::getLastReceivedData() {
    String msg = "Retrieving last received data: 0x" + String(_receivedData, HEX) + " (" + String(_receivedData) + ")";
    logger.logI2C(msg.c_str());
    _newDataReceived = false;
    logger.logI2C("Reset new data flag");
    return _receivedData;
}

void I2CSlave::setDataToSend(byte data) {
    _dataToSend = data;
    String msg = "Data to send updated: 0x" + String(_dataToSend, HEX) + " (" + String(_dataToSend) + ")";
    logger.logI2C(msg.c_str());
}

void I2CSlave::testPins() {
    String msg;
    logger.logI2C("=== Pin Test Start ===");
    logger.logI2C("Setting pins to OUTPUT mode");
    
    pinMode(SDA_PIN, OUTPUT);
    pinMode(SCL_PIN, OUTPUT);
    
    msg = "Starting pin test - SDA=" + String(SDA_PIN) + " SCL=" + String(SCL_PIN);
    logger.logI2C(msg.c_str());
    
    for(int i = 0; i < 20; i++) {
        msg = "Cycle " + String(i+1) + "/20: HIGH";
        logger.logI2C(msg.c_str());
        digitalWrite(SDA_PIN, HIGH);
        digitalWrite(SCL_PIN, HIGH);
        delay(200);
        
        msg = "Cycle " + String(i+1) + "/20: LOW";
        logger.logI2C(msg.c_str());
        digitalWrite(SDA_PIN, LOW);
        digitalWrite(SCL_PIN, LOW);
        delay(200);
    }
    
    logger.logI2C("=== Pin Test Complete ===");
}

}