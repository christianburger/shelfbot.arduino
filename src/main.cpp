#define I2C_SLAVE_DEVICE

#include "shelfbot.h"
#include "i2c_slave.h"
#include "shelfbot_comms.h"
#include "shelfbot_motor.h"
#include <Wire.h>

I2CSlave i2c_slave;
uint8_t buffer[shelfbot::BUFFER_SIZE];

void stepAllMotors(int steps) {
    #ifdef I2C_SLAVE_DEVICE
    ShelfbotMotor::moveAllMotors(steps);
    #endif
}

bool isMotorRunning(int motorNumber) {
    #ifdef I2C_SLAVE_DEVICE
    return ShelfbotMotor::isMotorRunning(motorNumber);
    #else
    return false;
    #endif
}

void printMotorSpeeds() {
    #ifdef I2C_SLAVE_DEVICE
    Serial.println("\nprintMotorSpeeds starting...");
    Serial.println("Motor Maximum Speeds:");
    ShelfbotMotor::printMotorSpeeds();
    Serial.println("printMotorSpeeds ending...\n\n");
    #endif
}

void testMotorSpeeds() {
    #ifdef I2C_SLAVE_DEVICE
    Serial.println("\ntestMotorSpeeds starting...");
    Serial.println("\n--- Motor Speed Test ---");
    // Test different speeds
    long testSpeeds[] = {1000, 2000, 5000, 10000};
    
    for (int speed : testSpeeds) {
        Serial.print("\nTesting speed: ");
        Serial.println(speed);
        
        ShelfbotMotor::setAllMotorSpeeds(speed);
        
        // Move all motors
        ShelfbotMotor::moveAllMotors(1000);
        delay(1000);
        ShelfbotMotor::moveAllMotors(0);
        delay(1000);
    }
    Serial.println("\ntestMotorSpeeds ending...\n\n");
    #endif
}

void testMotors() {
    #ifdef I2C_SLAVE_DEVICE
    unsigned long time = millis();

    Serial.print("\n\nTIME: ");
    Serial.println(time, DEC);

    Serial.println("TMC2208: Forward movement - all motors");
    stepAllMotors(2000);
    
    Serial.println("TMC2208: Backward movement - all motors");
    stepAllMotors(-2000);
    
    Serial.println("TMC2208: Full rotation forward - all motors");
    stepAllMotors(800);
    
    Serial.println("TMC2208: Full rotation backward - all motors");
    stepAllMotors(-800);

    Serial.println("TMC2208: Forward movement - all motors");
    ShelfbotMotor::moveAllMotors(2000);
 
    Serial.println("TMC2208: Backward movement - all motors");
    ShelfbotMotor::moveAllMotors(0);
    
    Serial.println("TMC2208: Full rotation forward - all motors");
    ShelfbotMotor::moveAllMotors(800);
    
    Serial.println("TMC2208: Full rotation backward - all motors");
    ShelfbotMotor::moveAllMotors(-800);
    #endif
}

void setup() {
    Serial.begin(115200);

    Serial.println("main: setup starting!");
    Serial.println("main: initializing device as i2c slave");
    i2c_slave.begin();

    #ifdef I2C_SLAVE_DEVICE
    ShelfbotMotor::begin();
    printMotorSpeeds();
    testMotorSpeeds();
    //testMotors();
    #endif
}   

void loop() {
    Serial.println("main: loop()!");
    delay(2000);
}

namespace shelfbot {
    void receiveCommand() {
        debugPrint("receiveCommand", "Starting");
        size_t bytesRead = Serial.readBytes(buffer, BUFFER_SIZE);
        JsonDocument doc;
        DeserializationError error = deserializeMsgPack(doc, buffer, bytesRead);

        if (!error) {
            float leftWheelCmd = doc["left_wheel"];
            float rightWheelCmd = doc["right_wheel"];
            executeCommand(leftWheelCmd, rightWheelCmd);
        }
        debugPrint("receiveCommand", "Finished");
    }

    void executeCommand(float leftWheelCmd, float rightWheelCmd) {
        debugPrint("executeCommand", "Starting");
        debugPrint("executeCommand", "Finished");
    }

    void sendStatus() {
        debugPrint("sendStatus", "Starting");
        JsonDocument doc;
        doc["left_wheel_pos"] = getLeftWheelPosition();
        doc["right_wheel_pos"] = getRightWheelPosition();
        doc["battery_voltage"] = getBatteryVoltage();

        size_t bytesWritten = serializeMsgPack(doc, buffer, BUFFER_SIZE);
        Serial.write(buffer, bytesWritten);
        debugPrint("sendStatus", "Finished");
    }

    float getLeftWheelPosition() {
        debugPrint("getLeftWheelPosition", "Called");
        return 0.0;
    }

    float getRightWheelPosition() {
        debugPrint("getRightWheelPosition", "Called");
        return 0.0;
    }

    float getBatteryVoltage() {
        debugPrint("getBatteryVoltage", "Called");
        return 0.0;
    }
}