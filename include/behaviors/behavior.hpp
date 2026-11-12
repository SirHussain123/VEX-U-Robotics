#pragma once

#include "sensors/sensor_snapshot.hpp"

class Behavior {
public:
    virtual ~Behavior() = default;

    virtual void update(const SensorSnapshot& snapshot) = 0;
    virtual bool blocksForward() const = 0;
};