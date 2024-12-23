#include "shelfbot_comms.h"

void ShelfbotComms::begin() {
    Serial.begin(115200);
    Serial.println("\nstarting I2C as slave");
    I2CSlave::begin();
}

CommandType ShelfbotComms::parseCommand(char * message) {
    if (strncmp(message, "GET_TEMP", 8) == 0) return CMD_GET_TEMP;
    if (strncmp(message, "SET_LED", 7) == 0) return CMD_SET_LED;
    if (strncmp(message, "READ_ADC", 8) == 0) return CMD_READ_ADC;
    if (strncmp(message, "SET_PWM", 7) == 0) return CMD_SET_PWM;
    if (strncmp(message, "GET_STATUS", 10) == 0) return CMD_GET_STATUS;
    return CMD_UNKNOWN;
}

void ShelfbotComms::handleCommand(char * message) {
    Serial.print("comms::Handling command: ");
    Serial.println(message);
    
    char currentResponse[32];
    switch(parseCommand(message)) {
        case CMD_GET_TEMP:
            Serial.println("comms::Processing GET_TEMP");
            snprintf(currentResponse, sizeof(currentResponse), "TEMP=%.2f", analogRead(A0) * 0.48876f);
            break;
        
        case CMD_SET_LED:
            Serial.println("comms::Processing SET_LED");
            {
                int value = atoi(message + 8);
                if(value < 0 || value > 1) {
                    strncpy(currentResponse, "ERR:VAL", sizeof(currentResponse));
                } else {
                    digitalWrite(LED_BUILTIN, value);
                    strncpy(currentResponse, "OK", sizeof(currentResponse));
                }
            }
            break;
        
        case CMD_READ_ADC:
            Serial.println("comms::Processing READ_ADC");
            snprintf(currentResponse, sizeof(currentResponse), "ADC=%d", analogRead(atoi(message + 9)));
            break;
        
        case CMD_SET_PWM:
            Serial.println("comms::Processing SET_PWM");
            {
                int value = atoi(message + 8);
                analogWrite(3, value);
                snprintf(currentResponse, sizeof(currentResponse), "PWM=%d", value);
            }
            break;
        
        case CMD_GET_STATUS:
            Serial.println("comms::Processing GET_STATUS");
            snprintf(currentResponse, sizeof(currentResponse), "UP=%lu", millis()/1000);
            break;
        
        default:
            Serial.println("comms::Unknown command received");
            strncpy(currentResponse, "ERR", sizeof(currentResponse));
            break;
    }
    
    I2CSlave::setResponse(currentResponse);
    Serial.print("comms::Response set to: ");
    Serial.println(currentResponse);
}

void ShelfbotComms::handleComms() {
    Serial.print("ShelfbotComms::handleComms");
}