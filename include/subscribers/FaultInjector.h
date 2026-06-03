#pragma once
#include "broker/MessageBroker.h"
#include "broker/Message.h"
#include <string>
#include <sstream>
#include <fstream>
#include <thread>
#include <atomic>
#include <chrono>
#include <iostream>

class FaultInjector {
public:
    FaultInjector() : running_(false) {}

    void start() {
        running_ = true;
        thread_ = std::thread(&FaultInjector::run, this);
    }

    void stop() {
        running_ = false;
        if (thread_.joinable()) thread_.detach();
    }

private:
    void run() {
        while (running_) {
            std::ifstream file("inject_cmd.txt");
            if (file.is_open()) {
                std::string line;
                if (std::getline(file, line) && !line.empty()) {
                    file.close();
                    processCommand(line);
                    // Clear the file after reading so same command
                    // can be re-injected next time
                    std::ofstream clear("inject_cmd.txt", std::ios::trunc);
                    clear.close();
                }
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    }

    void processCommand(const std::string& line) {
        std::istringstream ss(line);
        std::string command, topic;
        double value = 0.0;

        ss >> command >> topic >> value;

        if (command == "inject" && !topic.empty()) {
            Message msg;
            msg.topic     = topic;
            msg.value     = value;
            msg.unit      = "INJECTED";
            msg.source    = "FaultInjector";
            msg.timestamp = 0;

            MessageBroker::getInstance().publish(msg);
        }
    }

    std::thread       thread_;
    std::atomic<bool> running_;
};