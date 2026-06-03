#include "ecu/BatteryECU.h"
#include "broker/MessageBroker.h"
#include <chrono>
#include <cstdlib>

static long long nowMs() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now().time_since_epoch()
    ).count();
}

BatteryECU::BatteryECU()
    : BaseECU("BatteryECU", 2000),
      soc_(85.0), voltage_(400.0), temperature_(28.0) {}

void BatteryECU::generateAndPublish() {
    soc_ -= 0.1 + (rand() % 10) * 0.01;
    if (soc_ < 5.0) soc_ = 85.0;

    voltage_ = 350.0 + (soc_ / 100.0) * 60.0;

    temperature_ += (rand() % 3) - 1;
    if (temperature_ < 20.0) temperature_ = 20.0;
    if (temperature_ > 45.0) temperature_ = 45.0;

    MessageBroker& broker = MessageBroker::getInstance();
    broker.publish({"battery/soc",         soc_,         "%", name_, nowMs()});
    broker.publish({"battery/voltage",      voltage_,     "V", name_, nowMs()});
    broker.publish({"battery/temperature",  temperature_, "C", name_, nowMs()});
}