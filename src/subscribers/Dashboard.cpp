#include "subscribers/Dashboard.h"
#include <iostream>
#include <iomanip>

const std::vector<std::string> Dashboard::TOPIC_ORDER = {
    "engine/rpm",
    "engine/temp",
    "battery/soc",
    "battery/voltage",
    "battery/temperature",
    "adas/speed",
    "adas/front_proximity",
    "adas/lane_clear",
    "body/lights",
    "body/doors_locked",
    "body/wipers"
};

void Dashboard::onMessage(const Message& msg) {
    {
        std::lock_guard<std::mutex> lock(mutex_);
        latest_[msg.topic] = msg;
    }
    render();
}

void Dashboard::setAlertManager(AlertManager* am) {
    alertManager_ = am;
}

void Dashboard::render() {
    std::cout << "\033[2J\033[H";

    std::cout << "+----------------------------------------------------------+\n";
    std::cout << "|         IVI MESSAGE BUS  -  LIVE DASHBOARD               |\n";
    std::cout << "+----------------------------------------------------------+\n";
    std::cout << "|  " << std::left
              << std::setw(24) << "SIGNAL"
              << std::setw(10) << "VALUE"
              << std::setw(8)  << "UNIT"
              << std::setw(14) << "SOURCE"
              << "|\n";
    std::cout << "+----------------------------------------------------------+\n";

    {
        std::lock_guard<std::mutex> lock(mutex_);
        for (const auto& topic : TOPIC_ORDER) {
            auto it = latest_.find(topic);
            if (it != latest_.end()) {
                const Message& m = it->second;
                std::cout << "|  " << std::left
                          << std::setw(24) << m.topic
                          << std::setw(10) << std::fixed
                          << std::setprecision(1) << m.value
                          << std::setw(8)  << m.unit
                          << std::setw(14) << m.source
                          << "|\n";
            }
        }
    }

    std::cout << "+----------------------------------------------------------+\n";
    std::cout << "|  INJECT: echo \"inject <topic> <value>\" > inject_cmd.txt  |\n";
    std::cout << "+----------------------------------------------------------+\n";

    // Show alert history — stays visible inside the dashboard
    std::cout << "|  ACTIVE ALERTS:                                          |\n";
    std::cout << "+----------------------------------------------------------+\n";

    if (alertManager_ != nullptr) {
        const auto& alerts = alertManager_->getRecentAlerts();
        if (alerts.empty()) {
            std::cout << "|  No alerts.                                              |\n";
        } else {
            for (const auto& alert : alerts) {
                // Truncate to fit the box width (58 chars content)
                std::string display = alert;
                if (display.size() > 56) display = display.substr(0, 53) + "...";
                std::cout << "| \033[31m" << std::left << std::setw(57) << display
                          << "\033[0m|\n";
            }
        }
    }

    std::cout << "+----------------------------------------------------------+\n";
    std::cout.flush();
}