#pragma once
#include "broker/Message.h"

// Any class that wants to receive messages must inherit from this
// and implement the onMessage function.

class ISubscriber {
public:
    virtual void onMessage(const Message& msg) = 0;
    virtual ~ISubscriber() = default;
};