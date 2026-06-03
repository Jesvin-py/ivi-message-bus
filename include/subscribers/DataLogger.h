#pragma once
#include "subscribers/ISubscriber.h"
#include <fstream>
#include <mutex>
#include <string>

class DataLogger : public ISubscriber {
public:
    explicit DataLogger(const std::string& filename);
    ~DataLogger();
    void onMessage(const Message& msg) override;

private:
    std::ofstream file_;
    std::mutex    mutex_;
};