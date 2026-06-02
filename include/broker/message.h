#pragma once
#include <string>
#include <chrono>

// A Message is just a container for one signal reading.
// Example: topic="engine/rpm", value=3200.0, source="EngineECU"

struct Message {
    std::string topic;       // What signal is this? e.g. "engine/rpm"
    double      value;       // The actual reading
    std::string unit;        // e.g. "RPM", "°C", "%", "km/h"
    std::string source;      // Which ECU sent this?
    long long   timestamp;   // When was it sent? (milliseconds since program start)
};