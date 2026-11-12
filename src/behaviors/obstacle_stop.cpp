#include "behaviors/obstacle_stop.hpp"
#include "config.hpp"

void ObstacleStopBehavior::update(const SensorSnapshot& snapshot) {

    const DistanceReading& reading = snapshot.frontDistance;

    // Sensor error: block forward movement
    if (!reading.valid) {
        blocked = true;
        return;
    }

    // No object detected
    if (!reading.detected) {
        blocked = false;
        return;
    }

    // Hysteresis to prevent rapid switching
    if (blocked) {
        if (reading.clearanceMm >= config::FRONT_RELEASE_CLEARANCE_MM) {
            blocked = false;
        }
    }
    else {
        if (reading.clearanceMm <= config::FRONT_STOP_CLEARANCE_MM) {
            blocked = true;
        }
    }
}

bool ObstacleStopBehavior::blocksForward() const {
    return blocked;
}