#include "subscribers/AlertManager.h"
#include <iostream>

AlertManager::AlertManager() {
    rules_ = {
        {"engine/temp",          110.0, "above", "P0217", "Engine Overtemperature"},
        {"battery/soc",           15.0, "below", "P0A80", "Battery Charge Critical"},
        {"battery/temperature",   40.0, "above", "P0A93", "Battery Pack Overheating"},
        {"adas/front_proximity",  10.0, "below", "P1A00", "Collision Risk: Object Too Close"},
        {"adas/speed",           120.0, "above", "P0A1F", "Vehicle Overspeed Warning"},
        {"engine/rpm",          5500.0, "above", "P0219", "Engine Overspeed - Redline"},
        {"battery/voltage",      360.0, "below", "P0A02", "Battery Voltage Too Low"},
    };
}

void AlertManager::onMessage(const Message& msg) {
    for (const auto& rule : rules_) {
        if (msg.topic != rule.topic) continue;

        bool triggered = false;
        if (rule.condition == "above" && msg.value > rule.threshold) triggered = true;
        if (rule.condition == "below" && msg.value < rule.threshold) triggered = true;

        if (triggered) {
            std::lock_guard<std::mutex> lock(mutex_);
            // Add to alert history, keep last 5 only
            std::string alert = "  [" + rule.dtcCode + "] " + rule.description
                              + "  |  " + msg.topic
                              + " = " + std::to_string((int)msg.value)
                              + " " + msg.unit
                              + "  (threshold: " + std::to_string((int)rule.threshold) + ")";
            recentAlerts_.push_back(alert);
            if (recentAlerts_.size() > 5) {
                recentAlerts_.erase(recentAlerts_.begin());
            }
        }
    }
}

const std::vector<std::string>& AlertManager::getRecentAlerts() const {
    return recentAlerts_;
}