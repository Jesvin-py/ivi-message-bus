#pragma once
#include "ecu/BaseECU.h"

class AdasECU : public BaseECU {
public:
    AdasECU();

protected:
    void generateAndPublish() override;

private:
    double speed_;
    double frontProximity_;
    int    laneClear_;
};