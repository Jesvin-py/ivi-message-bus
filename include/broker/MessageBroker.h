#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <mutex>
#include "broker/Message.h"
#include "subscribers/ISubscriber.h"

// The MessageBroker is the central hub.
// ECUs call publish() to send a message.
// Subscribers call subscribe() to say "I want messages on this topic".

class MessageBroker {
public:
    // Singleton: only one broker exists in the whole program
    static MessageBroker& getInstance();

    // ECU calls this to send a message
    void publish(const Message& msg);

    // Subscriber calls this to register interest in a topic
    void subscribe(const std::string& topic, ISubscriber* subscriber);

private:
    MessageBroker() = default;

    // Map of topic -> list of subscribers interested in that topic
    std::unordered_map<std::string, std::vector<ISubscriber*>> subscribers_;

    // Mutex prevents two threads from touching subscribers_ at the same time
    std::mutex mutex_;
};