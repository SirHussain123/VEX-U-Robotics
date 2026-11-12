#pragma once

#include "behaviors/obstacle_stop.hpp"

class BehaviorManager {
public:
    void update(const SensorSnapshot& snapshot);

    bool blocksForward() const;

private:
    ObstacleStopBehavior obstacleStop;
};