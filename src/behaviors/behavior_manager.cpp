#include "behaviors/behavior_manager.hpp"

void BehaviorManager::update(const SensorSnapshot& snapshot) {
    obstacleStop.update(snapshot);
}

bool BehaviorManager::blocksForward() const {
    return obstacleStop.blocksForward();
}