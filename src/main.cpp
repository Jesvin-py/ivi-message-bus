#include <iostream>
#include <thread>
#include <chrono>

#include "broker/MessageBroker.h"
#include "ecu/EngineECU.h"
#include "ecu/BatteryECU.h"
#include "ecu/AdasECU.h"
#include "ecu/BodyECU.h"
#include "subscribers/Dashboard.h"
#include "subscribers/AlertManager.h"
#include "subscribers/DataLogger.h"
#include "subscribers/FaultInjector.h"

int main() {
    std::cout << "=== IVI Message Bus Starting ===\n\n";

    Dashboard    dashboard;
    AlertManager alertManager;
    DataLogger   logger("vehicle_log.csv");

    // Connect alert history to dashboard so alerts stay visible
    dashboard.setAlertManager(&alertManager);

    const std::vector<std::string> allTopics = {
        "engine/rpm", "engine/temp",
        "battery/soc", "battery/voltage", "battery/temperature",
        "adas/speed", "adas/front_proximity", "adas/lane_clear",
        "body/lights", "body/doors_locked", "body/wipers"
    };

    auto& broker = MessageBroker::getInstance();
    for (const auto& topic : allTopics) {
        broker.subscribe(topic, &dashboard);
        broker.subscribe(topic, &alertManager);
        broker.subscribe(topic, &logger);
    }

    FaultInjector injector;
    injector.start();

    EngineECU  engine;
    BatteryECU battery;
    AdasECU    adas;
    BodyECU    body;

    engine.start();
    battery.start();
    adas.start();
    body.start();

    std::this_thread::sleep_for(std::chrono::seconds(60));

    engine.stop();
    battery.stop();
    adas.stop();
    body.stop();
    injector.stop();

    std::cout << "\n=== IVI Message Bus Stopped ===\n";
    std::cout << "Log saved to vehicle_log.csv\n";
    return 0;
}