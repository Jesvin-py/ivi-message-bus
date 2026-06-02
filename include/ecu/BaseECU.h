#pragma once
#include <thread>
#include <atomic>
#include <string>

// BaseECU is the template for all ECUs.
// Each ECU inherits from this and implements generateAndPublish().

class BaseECU {
public:
    BaseECU(const std::string& name, int intervalMs);
    virtual ~BaseECU();

    void start();   // Starts the ECU's background thread
    void stop();    // Stops it cleanly

protected:
    // Subclasses put their signal-generation logic here
    virtual void generateAndPublish() = 0;

    std::string name_;
    int intervalMs_;   // How often this ECU sends data (milliseconds)

private:
    void run();   // The thread loop

    std::thread       thread_;
    std::atomic<bool> running_;
};