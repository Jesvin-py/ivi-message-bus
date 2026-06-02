#include "ecu/EngineECU.h"
#include "broker/MessageBroker.h"
#include <chrono>
#include <cstdlib>  // for rand()

EngineECU::EngineECU()
    : BaseECU("EngineECU", 1000),   // sends data every 1000ms = 1 second
      rpm_(800.0),
      temperature_(70.0) {}

// Helper: get current time in milliseconds
static long long nowMs() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now().time_since_epoch()
    ).count();
}

void EngineECU::generateAndPublish() {
    // Simulate RPM changing slightly each second
    rpm_ += (rand() % 201) - 100;   // random change between -100 and +100
    if (rpm_ < 800)  rpm_ = 800;
    if (rpm_ > 6000) rpm_ = 6000;

    // Simulate temperature slowly rising then stabilising
    temperature_ += (rand() % 5) - 2;   // random change between -2 and +2
    if (temperature_ < 70)  temperature_ = 70;
    if (temperature_ > 115) temperature_ = 115;

    MessageBroker& broker = MessageBroker::getInstance();

    // Publish RPM
    broker.publish({"engine/rpm", rpm_, "RPM", name_, nowMs()});

    // Publish temperature
    broker.publish({"engine/temp", temperature_, "C", name_, nowMs()});
}