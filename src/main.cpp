#include "shelfbot.h"

uint8_t buffer[shelfbot::BUFFER_SIZE];

void setup() { Serial.begin(115200); }

void loop() {
  if (Serial.available()) {
    shelfbot::receiveCommand();
  }
  shelfbot::sendStatus();
  delay(100);
}

namespace shelfbot {

  void debugPrint(const char *className, const char *message) {
    unsigned long timestamp = millis();
    Serial.print("debug ");
    Serial.print(timestamp);
    Serial.print(": ");
    Serial.print(className);
    Serial.print(": ");
    Serial.println(message);
  }

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
