#pragma once
#include <Arduino.h>

namespace shelfbot_logging {

const int MAX_LOG_ENTRIES = 100;

class ShelfbotLogger {
public:
    ShelfbotLogger();
    void init();
    void log(const char* message);
    void logI2C(const char* message);
    void logWiFi(const char* message);
    void logSystem(const char* message);
    String getLogHTML();
    void clearLog();
    
private:
    String logEntries[MAX_LOG_ENTRIES];
    String getTimeStamp();
    int logIndex;
};

}