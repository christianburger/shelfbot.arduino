#ifndef SHELFBOT_COMMS_H
#define SHELFBOT_COMMS_H

#include <Arduino.h>
#include "i2c_slave.h"

enum CommandType {
    CMD_UNKNOWN,
    CMD_GET_TEMP,
    CMD_SET_LED,
    CMD_READ_ADC,
    CMD_SET_PWM,
    CMD_GET_STATUS
};

class ShelfbotComms {
public:
    static void begin();
    static void handleComms();
    static void handleCommand(char * message);

private:
    static CommandType parseCommand(char * message);
};

#endif