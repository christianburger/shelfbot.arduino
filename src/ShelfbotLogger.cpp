#include "ShelfbotLogger.h"

namespace shelfbot_logging {

ShelfbotLogger::ShelfbotLogger() : logIndex(0) {
    for(int i = 0; i < MAX_LOG_ENTRIES; i++) {
        logEntries[i] = "";
    }
}

void ShelfbotLogger::init() {
}

void ShelfbotLogger::log(const char* message) {
    String entry = String(message);
    logEntries[logIndex] = entry;
    logIndex = (logIndex + 1) % MAX_LOG_ENTRIES;
    Serial.print("ShelfbotLogger::log -> message: ");
    Serial.println(message);
}
void ShelfbotLogger::logI2C(const char* message) {
    log(message);
}

void ShelfbotLogger::logWiFi(const char* message) {
    log(message);
}

void ShelfbotLogger::logSystem(const char* message) {
    log(message);
}

String ShelfbotLogger::getLogHTML() {
    String html = "<html><body><h1>Shelfbot System Log</h1><pre>\n";
    
    for (int i = 0; i < MAX_LOG_ENTRIES; i++) {
        int idx = (logIndex + i) % MAX_LOG_ENTRIES;
        if (logEntries[idx].length() > 0) {
            html += logEntries[idx] + "\n";
        }
    }
    
    html += "</pre></body></html>";
    return html;
}

void ShelfbotLogger::clearLog() {
    for(int i = 0; i < MAX_LOG_ENTRIES; i++) {
        logEntries[i] = "";
    }
    logIndex = 0;
    log("Log cleared");
}

}