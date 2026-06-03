#include "ecu/AdasECU.h"
#include "broker/MessageBroker.h"
#include <chrono>
#include <cstdlib>

static long long nowMs() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now().time_since_epoch()
    ).count();
}

AdasECU::AdasECU()
    : BaseECU("AdasECU", 500),
      speed_(60.0), frontProximity_(50.0), laneClear_(1) {}

void AdasECU::generateAndPublish() {
    speed_ += (rand() % 11) - 5;
    if (speed_ < 0.0)   speed_ = 0.0;
    if (speed_ > 130.0) speed_ = 130.0;

    frontProximity_ += (rand() % 21) - 10;
    if (frontProximity_ < 5.0)   frontProximity_ = 5.0;
    if (frontProximity_ > 120.0) frontProximity_ = 120.0;

    laneClear_ = (frontProximity_ > 20.0) ? 1 : 0;

    MessageBroker& broker = MessageBroker::getInstance();
    broker.publish({"adas/speed",           speed_,             "km/h", name_, nowMs()});
    broker.publish({"adas/front_proximity", frontProximity_,    "m",    name_, nowMs()});
    broker.publish({"adas/lane_clear",      (double)laneClear_, "bool", name_, nowMs()});
}