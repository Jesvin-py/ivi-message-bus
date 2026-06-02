#pragma once
#include "ecu/BaseECU.h"

class EngineECU : public BaseECU {
public:
    EngineECU();

protected:
    void generateAndPublish() override;

private:
    double rpm_;
    double temperature_;
};