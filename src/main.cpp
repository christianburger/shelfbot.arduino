#include "shelfbot.h"
#include "i2c_slave.h"

#include <AccelStepper.h>
#include <Wire.h>

I2CSlave slave;

uint8_t buffer[shelfbot::BUFFER_SIZE];

// Number of steps per output rotation
const int stepsPerRevolution = 800;

// Define stepper motor connections
const int NUM_MOTORS = 6;

// Motor pin definitions
const int motorPins[NUM_MOTORS][2] = {
  {4, 3},   // {STEP_PIN, DIR_PIN} for Motor 1
  {6, 5},   // Motor 2
  {8, 7},   // Motor 3
  {10, 9},  // Motor 4
  {12, 11}, // Motor 5
  {14, 13}  // Motor 6
};

// Create array of stepper motor objects
AccelStepper steppers[NUM_MOTORS] = {
  AccelStepper(AccelStepper::DRIVER, motorPins[0][0], motorPins[0][1]),
  AccelStepper(AccelStepper::DRIVER, motorPins[1][0], motorPins[1][1]),
  AccelStepper(AccelStepper::DRIVER, motorPins[2][0], motorPins[2][1]),
  AccelStepper(AccelStepper::DRIVER, motorPins[3][0], motorPins[3][1]),
  AccelStepper(AccelStepper::DRIVER, motorPins[4][0], motorPins[4][1]),
  AccelStepper(AccelStepper::DRIVER, motorPins[5][0], motorPins[5][1])
};


void setupMotors() {
    for (int i = 0; i < NUM_MOTORS; i++) {
        // Basic settings for TMC2208
        steppers[i].setMaxSpeed(1000);
        steppers[i].setAcceleration(500);
        steppers[i].setSpeed(1000);
        
        // Essential TMC2208 driver settings
        steppers[i].setMinPulseWidth(1);
        steppers[i].setPinsInverted(false, false, false);
        steppers[i].setEnablePin(0xff); // Disable the enable pin handling
    }
}

void moveAllMotors(long position) {
    // Set target position
    for (int i = 0; i < NUM_MOTORS; i++) {
        steppers[i].moveTo(position);
        steppers[i].setSpeed(steppers[i].maxSpeed());
    }
    
    // Run motors
    while (true) {
        bool allDone = true;
        for (int i = 0; i < NUM_MOTORS; i++) {
            if (steppers[i].distanceToGo() != 0) {
                steppers[i].runSpeedToPosition();
                allDone = false;
            }
        }
        if (allDone) break;
    }
}

/* void setupMotors() {
  for (int i = 0; i < NUM_MOTORS; i++) {
      // Lower speeds for initial testing
      steppers[i].setMaxSpeed(4000);  // Start with 1000 steps/sec
      steppers[i].setAcceleration(5000);  // Gentler acceleration
      steppers[i].setSpeed(4000);
      
      // Critical for TMC2208
      steppers[i].setMinPulseWidth(2);  // 2 microseconds pulse width
      steppers[i].setPinsInverted(false, false, false);
  }
}

void moveAllMotors(long position) {
    // Set target position for all motors
    for (int i = 0; i < NUM_MOTORS; i++) {
        steppers[i].moveTo(position);
    }
    
    // Run all motors with acceleration until they reach their targets
    bool running;
    do {
        running = false;
        for (int i = 0; i < NUM_MOTORS; i++) {
            if (steppers[i].distanceToGo() != 0) {
                steppers[i].run();  // Changed from runSpeed() to run()
                running = true;
            }
        }
    } while (running);
} */

void stepAllMotors(int steps) {
    // Set direction for all motors
  int direction = steps > 0 ? 1 : -1;
  for (int i = 0; i < abs(steps); i++) {
    for (int motor = 0; motor < NUM_MOTORS; motor++) {
      steppers[motor].setCurrentPosition(steppers[motor].currentPosition() + direction);
    }
  }
}

bool isMotorRunning(int motorNumber) {
    if (motorNumber >= 0 && motorNumber < NUM_MOTORS) {
        return steppers[motorNumber].isRunning();
    }
    return false;
}

void printMotorSpeeds() {
  Serial.println("\nprintMotorSpeeds starting...");
  Serial.println("Motor Maximum Speeds:");
  for (int i = 0; i < NUM_MOTORS; i++) {
    Serial.print("Motor ");
    Serial.print(i + 1);
    Serial.print(" max speed: ");
    Serial.print(steppers[i].maxSpeed());
    Serial.print(" steps/sec, current speed: ");
    Serial.print(steppers[i].speed());
    Serial.println(" steps/sec");
  }
  Serial.println("printMotorSpeeds ending...\n\n");
}

void testMotorSpeeds() {
  Serial.println("\ntestMotorSpeeds starting...");
  Serial.println("\n--- Motor Speed Test ---");
  // Test different speeds
  long testSpeeds[] = {1000, 2000, 5000, 10000};
    
  for (int speed : testSpeeds) {
    Serial.print("\nTesting speed: ");
    Serial.println(speed);
        
    for (int i = 0; i < NUM_MOTORS; i++) {
      steppers[i].setMaxSpeed(speed);
      steppers[i].setSpeed(speed);
      steppers[i].setAcceleration(speed/2);
      
      Serial.print("Motor ");
      Serial.print(i);
      Serial.print(" - Max: ");
      Serial.print(steppers[i].maxSpeed());
      Serial.print(", Current: ");
      Serial.println(steppers[i].speed());
    }
        
    // Move all motors
    moveAllMotors(1000);
    delay(1000);
    moveAllMotors(0);
    delay(1000);
  }
  Serial.println("\ntestMotorSpeeds ending...\n\n");
}

void requestEvent() {
    Serial.println("Req");
}

void receiveEvent(int numBytes) {
    Serial.print("\nreceived: ");
    for (int i = 0; Wire.available() && i < numBytes; i++) {
        char c = Wire.read();
        Serial.print(c);
    }
    Serial.println("\t ... END");
}

void setup() {
  Serial.begin(115200);

  Serial.println("main: setup starting!");
  Serial.println("main: initializing device as i2c slave");
  slave.begin();

  setupMotors();
  printMotorSpeeds();
  testMotorSpeeds();
}   

void loop() {
  unsigned long time = millis();
  Serial.print("\n\nTIME: ");
  Serial.println(time, DEC);

  Serial.println("TMC2208: Forward movement - all motors");
  stepAllMotors(2000);
    
  Serial.println("TMC2208: Backward movement - all motors");
  stepAllMotors(-2000);
    
  Serial.println("TMC2208: Full rotation forward - all motors");
  stepAllMotors(stepsPerRevolution);
    
  Serial.println("TMC2208: Full rotation backward - all motors");
  stepAllMotors(-stepsPerRevolution);

  Serial.println("TMC2208: Forward movement - all motors");
  moveAllMotors(2000);
 
  Serial.println("TMC2208: Backward movement - all motors");
  moveAllMotors(0);
    
  Serial.println("TMC2208: Full rotation forward - all motors");
  for (int i = 0; i < NUM_MOTORS; i++) {
      steppers[i].moveTo(steppers[i].currentPosition() + stepsPerRevolution);
  }
  moveAllMotors(steppers[0].targetPosition());
    
  Serial.println("TMC2208: Full rotation backward - all motors");
  for (int i = 0; i < NUM_MOTORS; i++) {
      steppers[i].moveTo(steppers[i].currentPosition() - stepsPerRevolution);
  }
  moveAllMotors(steppers[0].targetPosition());
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
