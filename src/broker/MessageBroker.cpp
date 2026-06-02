#include "broker/MessageBroker.h"
#include <iostream>

// Returns the single shared instance of the broker
MessageBroker& MessageBroker::getInstance() {
    static MessageBroker instance;
    return instance;
}

void MessageBroker::subscribe(const std::string& topic, ISubscriber* subscriber) {
    std::lock_guard<std::mutex> lock(mutex_);
    subscribers_[topic].push_back(subscriber);
}

void MessageBroker::publish(const Message& msg) {
    std::lock_guard<std::mutex> lock(mutex_);

    // Check if anyone is subscribed to this topic
    auto it = subscribers_.find(msg.topic);
    if (it != subscribers_.end()) {
        // Forward the message to every subscriber on this topic
        for (ISubscriber* sub : it->second) {
            sub->onMessage(msg);
        }
    }
}