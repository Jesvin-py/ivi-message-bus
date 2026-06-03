#pragma once
#include "subscribers/ISubscriber.h"
#include <vector>
#include <string>
#include <mutex>

struct AlertRule {
    std::string topic;
    double      threshold;
    std::string condition;
    std::string dtcCode;
    std::string description;
};

class AlertManager : public ISubscriber {
public:
    AlertManager();
    void onMessage(const Message& msg) override;
    const std::vector<std::string>& getRecentAlerts() const;

private:
    std::vector<AlertRule>   rules_;
    std::vector<std::string> recentAlerts_;
    mutable std::mutex       mutex_;
};