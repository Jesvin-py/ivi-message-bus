#include "subscribers/DataLogger.h"
#include <iostream>

DataLogger::DataLogger(const std::string& filename) {
    file_.open(filename, std::ios::out | std::ios::trunc);
    if (file_.is_open()) {
        file_ << "timestamp_ms,topic,value,unit,source\n";
        std::cout << "[DataLogger] Logging to: " << filename << "\n";
    } else {
        std::cerr << "[DataLogger] ERROR: Could not open: " << filename << "\n";
    }
}

DataLogger::~DataLogger() {
    if (file_.is_open()) file_.close();
}

void DataLogger::onMessage(const Message& msg) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (file_.is_open()) {
        file_ << msg.timestamp << ","
              << msg.topic     << ","
              << msg.value     << ","
              << msg.unit      << ","
              << msg.source    << "\n";
        file_.flush();
    }
}