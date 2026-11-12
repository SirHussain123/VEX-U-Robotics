#include "sensors/sensors.hpp"
#include "config.hpp"
#include <algorithm>

Sensors::Sensors()
    : frontDistance(config::FRONT_DISTANCE_PORT) {
}

SensorSnapshot Sensors::read() {

    SensorSnapshot snapshot;

    int rawDistance = frontDistance.get_distance();

    if (rawDistance == 9999) {
        snapshot.frontDistance.valid = true;
        snapshot.frontDistance.detected = false;
    }
    else if (rawDistance >= 20 && rawDistance <= 2000) {

        snapshot.frontDistance.valid = true;
        snapshot.frontDistance.detected = true;

        snapshot.frontDistance.millimeters = rawDistance;

        snapshot.frontDistance.clearanceMm = std::max(
            0, rawDistance - config::FRONT_SENSOR_OFFSET_MM
        );
    }

    return snapshot;
}