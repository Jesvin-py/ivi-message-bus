#pragma once
#include "subscribers/ISubscriber.h"
#include "subscribers/AlertManager.h"
#include <unordered_map>
#include <vector>
#include <mutex>
#include <string>

class Dashboard : public ISubscriber {
public:
    Dashboard() : alertManager_(nullptr) {}

    void onMessage(const Message& msg) override;
    void setAlertManager(AlertManager* am);

private:
    void render();

    std::unordered_map<std::string, Message> latest_;
    std::mutex   mutex_;
    AlertManager* alertManager_;

    static const std::vector<std::string> TOPIC_ORDER;
};