#ifndef SHELFBOT_H
#define SHELFBOT_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <MsgPack.h>

namespace shelfbot {

const int BUFFER_SIZE = 64;

void receiveCommand();
void executeCommand(float leftWheelCmd, float rightWheelCmd);
void sendStatus();
float getLeftWheelPosition();
float getRightWheelPosition();
float getBatteryVoltage();
void debugPrint(const char* className, const char* message);

}

#endif
