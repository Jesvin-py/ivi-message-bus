#pragma once
#include "ecu/BaseECU.h"

class BodyECU : public BaseECU {
public:
    BodyECU();

protected:
    void generateAndPublish() override;

private:
    int lightsOn_;
    int doorsLocked_;
    int wipersOn_;
};