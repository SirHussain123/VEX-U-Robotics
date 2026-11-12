#pragma once

#include "behaviors/behavior.hpp"

class ObstacleStopBehavior : public Behavior {
public:
    void update(const SensorSnapshot& snapshot) override;
    bool blocksForward() const override;

private:
    bool blocked = false;
};