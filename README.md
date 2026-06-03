# IVI Message Bus Simulator

A multithreaded C++ simulation of an in-vehicle publish-subscribe communication
system, modelling how Electronic Control Units (ECUs) exchange signals in a
modern software-defined vehicle.

Built as a portfolio project to demonstrate automotive software engineering
concepts including concurrent ECU simulation, thread-safe message routing,
real-time fault detection, and interactive diagnostic injection.

---

## Architecture

Each ECU runs on an independent thread and publishes signals to a central
MessageBroker. Subscribers receive and process those signals in real time.

EngineECU  (10ms cycle) ──┐
BatteryECU (2s  cycle)  ──┤──► MessageBroker ──► Dashboard     (live terminal UI)
AdasECU    (500ms cycle)──┤    (pub-sub hub)  ──► AlertManager  (DTC fault codes)
BodyECU    (3s  cycle)  ──┘                   ──► DataLogger    (CSV output)
──► FaultInjector (runtime commands)

---

## Features

- **4 simulated ECUs** running as concurrent threads with realistic cycle times
- **Thread-safe MessageBroker** using std::mutex and the Observer pattern
- **Live terminal dashboard** that refreshes in real time with all signal values
- **AlertManager** with 7 DTC fault rules mapped to real automotive codes
- **DataLogger** writing every signal to a timestamped CSV file
- **FaultInjector** — inject any signal value at runtime via a command file,
  triggering alerts instantly without stopping the simulation
- Clean shutdown — all threads finish their current cycle before stopping

---

## DTC Fault Rules

| DTC Code | Signal | Condition | Description |
|----------|--------|-----------|-------------|
| P0217 | engine/temp | > 110°C | Engine Overtemperature |
| P0219 | engine/rpm | > 5500 RPM | Engine Overspeed - Redline |
| P0A80 | battery/soc | < 15% | Battery Charge Critical |
| P0A93 | battery/temperature | > 40°C | Battery Pack Overheating |
| P0A02 | battery/voltage | < 360V | Battery Voltage Too Low |
| P0A1F | adas/speed | > 120 km/h | Vehicle Overspeed Warning |
| P1A00 | adas/front_proximity | < 10m | Collision Risk: Object Too Close |

---

## Build Instructions

**Requirements:** CMake 3.15+, g++ with C++17 support, MSYS2 UCRT64 (Windows)
or any Linux/macOS with GCC.

```bash
git clone https://github.com/Jesvin-py/ivi-message-bus.git
cd ivi-message-bus
mkdir build && cd build
cmake .. -G "Ninja"
cmake --build .
./ivi_bus.exe
```

---

## Fault Injection Demo

While the simulation is running, open a second terminal and inject any signal:

```bash
# Trigger engine overtemperature (P0217)
echo "inject engine/temp 130" > inject_cmd.txt

# Trigger battery critical (P0A80)
echo "inject battery/soc 10" > inject_cmd.txt

# Trigger collision risk (P1A00)
echo "inject adas/front_proximity 5" > inject_cmd.txt

# Trigger overspeed (P0A1F)
echo "inject adas/speed 125" > inject_cmd.txt
```

Alerts appear instantly inside the dashboard and persist in the ACTIVE ALERTS
section so they are always visible.

---

## Project Structure

ivi-message-bus/
├── include/
│   ├── broker/
│   │   ├── Message.h           # Signal envelope struct
│   │   └── MessageBroker.h     # Pub-sub engine (Singleton)
│   ├── ecu/
│   │   ├── BaseECU.h           # Abstract ECU with thread lifecycle
│   │   ├── EngineECU.h
│   │   ├── BatteryECU.h
│   │   ├── AdasECU.h
│   │   └── BodyECU.h
│   └── subscribers/
│       ├── ISubscriber.h       # Pure virtual subscriber interface
│       ├── Dashboard.h
│       ├── AlertManager.h
│       ├── DataLogger.h
│       └── FaultInjector.h
├── src/                        # Implementations
├── CMakeLists.txt
└── README.md

---

## Concepts Demonstrated

- **Multithreading** — std::thread per ECU, std::mutex for shared state
- **Observer pattern** — decoupled pub-sub message routing
- **OOP** — abstract base class, polymorphism, interface contracts
- **RAII** — thread cleanup in destructors, file handle management
- **Automotive domain** — ECU architecture, DTC fault codes, cycle times,
  signal topics modelled after real vehicle networks (CAN/SOME-IP concepts)
- **Software-defined vehicle** — runtime fault injection simulates OTA
  diagnostic commands sent to a live vehicle system

---

## Automotive Context

This project models concepts found in real vehicle software stacks:

- The **MessageBroker** mirrors middleware like AUTOSAR COM or DDS, where
  signals are routed by topic without publishers knowing their subscribers
- **ECU cycle times** reflect real values — ADAS at 500ms, body control at 3s
- **DTC codes** follow SAE J2012 naming conventions used in OBD-II diagnostics
- **Fault injection** mirrors UDS (ISO 14229) diagnostic commands sent to ECUs
  over a separate channel during vehicle testing

---

*Built with C++17 — no external dependencies*