#ifndef SHELFBOT_COMMS_H
#define SHELFBOT_COMMS_H

#include <Arduino.h>
#include <Wire.h>

#ifdef I2C_SLAVE_DEVICE
#include "i2c_slave.h"
#include "shelfbot_motor.h"
#else
#include "i2c_master.h"
#endif

#define CMD_START_MARKER 0x3C3C  // "<<"
#define CMD_END_MARKER 0x3E3E    // ">>"
#define CMD_VALUE_START 0x7B7B   // "{{"
#define CMD_VALUE_END 0x7D7D     // "}}"
#define CMD_CHECKSUM_START 0x2323 // "##"
#define CMD_CHECKSUM_END 0x2424   // "$"

enum CommandResponse : uint16_t {
    RESP_OK = 0x4F4B,       
    RESP_READY = 0x5244,    
    RESP_COMPLETE = 0x434D,  
    RESP_BUSY = 0x4255,     
    RESP_MOVING = 0x4D56,   
    RESP_STOPPED = 0x5354,  
    RESP_ERROR = 0x4552,    
    RESP_INVALID = 0x494E,  
    RESP_TIMEOUT = 0x544F,  
    RESP_ERR_RANGE = 0x4552, 
    RESP_ERR_VALUE = 0x4556, 
    RESP_ERR_PIN = 0x4550,   
    RESP_ERR_MOTOR = 0x454D, 
    RESP_ERR_PARAM = 0x4548, 
    RESP_ERR_STATE = 0x4553, 
    RESP_ERR_COMM = 0x4543,  
    RESP_ERR_CRC = 0x4352    
};

enum CommandType : uint16_t {
    CMD_UNKNOWN = 0x3F3F,     
    CMD_GET_TEMP = 0x4754,    
    CMD_SET_LED = 0x534C,     
    CMD_READ_ADC = 0x5241,    
    CMD_SET_PWM = 0x5350,     
    CMD_GET_STATUS = 0x4753,  
    CMD_SET_MOTOR_1 = 0x4D31, 
    CMD_SET_MOTOR_2 = 0x4D32, 
    CMD_SET_MOTOR_3 = 0x4D33, 
    CMD_SET_MOTOR_4 = 0x4D34, 
    CMD_SET_MOTOR_5 = 0x4D35, 
    CMD_SET_MOTOR_6 = 0x4D36, 
    CMD_GET_MOTOR_1_POS = 0x5031, 
    CMD_GET_MOTOR_2_POS = 0x5032, 
    CMD_GET_MOTOR_3_POS = 0x5033, 
    CMD_GET_MOTOR_4_POS = 0x5034, 
    CMD_GET_MOTOR_5_POS = 0x5035, 
    CMD_GET_MOTOR_6_POS = 0x5036, 
    CMD_GET_MOTOR_1_VEL = 0x5631, 
    CMD_GET_MOTOR_2_VEL = 0x5632, 
    CMD_GET_MOTOR_3_VEL = 0x5633, 
    CMD_GET_MOTOR_4_VEL = 0x5634, 
    CMD_GET_MOTOR_5_VEL = 0x5635, 
    CMD_GET_MOTOR_6_VEL = 0x5636, 
    CMD_STOP_MOTOR_1 = 0x5331, 
    CMD_STOP_MOTOR_2 = 0x5332, 
    CMD_STOP_MOTOR_3 = 0x5333, 
    CMD_STOP_MOTOR_4 = 0x5334, 
    CMD_STOP_MOTOR_5 = 0x5335, 
    CMD_STOP_MOTOR_6 = 0x5336, 
    CMD_STOP_ALL = 0x5341,     
    CMD_GET_BATTERY = 0x4742,  
    CMD_GET_SYSTEM = 0x5359    
};

class ShelfbotComms {
public:
    static void begin();
    static void handleComms();
    static void handleCommand(char* message);
    static void sendCommand(CommandType cmd, uint16_t value = 0);
    static String formatResponse(CommandResponse resp, const String& value);
    static String formatCommand(CommandType cmd, const String& value);
    static String parseValue(const String& message);
    static CommandType parseCommand(const String& message);
    static bool verifyChecksum(const String& message);
    static void moveAllMotors(long position);
    static String setMotor(uint8_t index, const String& value);
    static String getMotorPosition(uint8_t index);
    static String getMotorVelocity(uint8_t index);
    static String stopMotor(uint8_t index);
    static String stopAllMotors();

private:
    static String getTemperature();
    static String setLed(const String& value);
    static String readAdc(const String& value);
    static String setPwm(const String& value);
    static String getStatus();
    static String getBatteryLevel();
    static String getSystemStatus();
};

#endif