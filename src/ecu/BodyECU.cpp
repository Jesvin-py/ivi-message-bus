#include "ecu/BodyECU.h"
#include "broker/MessageBroker.h"
#include <chrono>
#include <cstdlib>

static long long nowMs() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now().time_since_epoch()
    ).count();
}

BodyECU::BodyECU()
    : BaseECU("BodyECU", 3000),
      lightsOn_(1), doorsLocked_(1), wipersOn_(0) {}

void BodyECU::generateAndPublish() {
    if (rand() % 5  == 0) lightsOn_    = !lightsOn_;
    if (rand() % 8  == 0) wipersOn_    = !wipersOn_;
    if (rand() % 15 == 0) doorsLocked_ = !doorsLocked_;

    MessageBroker& broker = MessageBroker::getInstance();
    broker.publish({"body/lights",       (double)lightsOn_,    "bool", name_, nowMs()});
    broker.publish({"body/doors_locked", (double)doorsLocked_, "bool", name_, nowMs()});
    broker.publish({"body/wipers",       (double)wipersOn_,    "bool", name_, nowMs()});
}