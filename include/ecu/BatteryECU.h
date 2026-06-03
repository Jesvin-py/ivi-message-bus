#pragma once
#include "ecu/BaseECU.h"

class BatteryECU : public BaseECU {
public:
    BatteryECU();

protected:
    void generateAndPublish() override;

private:
    double soc_;
    double voltage_;
    double temperature_;
};