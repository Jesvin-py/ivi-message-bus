#include <iostream>
#include <thread>
#include <chrono>

#include "broker/MessageBroker.h"
#include "subscribers/ISubscriber.h"
#include "ecu/EngineECU.h"

// A simple inline subscriber that just prints every message it receives
class ConsolePrinter : public ISubscriber {
public:
    void onMessage(const Message& msg) override {
        std::cout << "[" << msg.timestamp << "ms] "
                  << msg.source << " | "
                  << msg.topic << " = "
                  << msg.value << " " << msg.unit
                  << "\n";
    }
};

int main() {
    std::cout << "=== IVI Message Bus Starting ===\n\n";

    // Create our simple console printer subscriber
    ConsolePrinter printer;

    // Subscribe it to engine topics
    MessageBroker::getInstance().subscribe("engine/rpm",  &printer);
    MessageBroker::getInstance().subscribe("engine/temp", &printer);

    // Start the engine ECU (runs in its own thread)
    EngineECU engine;
    engine.start();

    // Let it run for 10 seconds
    std::this_thread::sleep_for(std::chrono::seconds(10));

    // Stop cleanly
    engine.stop();

    std::cout << "\n=== IVI Message Bus Stopped ===\n";
    return 0;
}