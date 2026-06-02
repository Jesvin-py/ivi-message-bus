#include "ecu/BaseECU.h"
#include <chrono>
#include <iostream>

BaseECU::BaseECU(const std::string& name, int intervalMs)
    : name_(name), intervalMs_(intervalMs), running_(false) {}

BaseECU::~BaseECU() {
    stop();
}

void BaseECU::start() {
    running_ = true;
    thread_ = std::thread(&BaseECU::run, this);
    std::cout << "[" << name_ << "] started\n";
}

void BaseECU::stop() {
    running_ = false;
    if (thread_.joinable()) {
        thread_.join();   // Wait for the thread to finish cleanly
    }
    std::cout << "[" << name_ << "] stopped\n";
}

void BaseECU::run() {
    while (running_) {
        generateAndPublish();
        std::this_thread::sleep_for(std::chrono::milliseconds(intervalMs_));
    }
}