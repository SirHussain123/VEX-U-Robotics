#include "sensors/sensors.hpp"
#include "config.hpp"
#include <algorithm>
#include <cmath>

Sensors::Sensors()
    : frontDistance(config::FRONT_DISTANCE_PORT),
      inertial(config::INERTIAL_PORT) {
}

void Sensors::initialize() {
    inertial.reset(false);
}

SensorSnapshot Sensors::read() {

    SensorSnapshot snapshot;

    // Distance sensor
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

        // Inertial sensor
    int status = static_cast<int>(inertial.get_status());

    constexpr int CALIBRATING_BIT = 0x01;
    constexpr int ERROR_STATUS = 0xFF;

    if (status != ERROR_STATUS && status != -1) {

        snapshot.inertial.calibrating =
            (status & CALIBRATING_BIT) != 0;

        if (!snapshot.inertial.calibrating) {

            double heading = inertial.get_heading();
            double rotation = inertial.get_rotation();
            double pitch = inertial.get_pitch();
            double roll = inertial.get_roll();

            pros::imu_gyro_s_t gyro = inertial.get_gyro_rate();
            pros::imu_accel_s_t accel = inertial.get_accel();

            if (std::isfinite(heading) &&
                heading >= 0 && heading < 360 &&
                std::isfinite(rotation) &&
                std::isfinite(pitch) &&
                std::isfinite(roll) &&
                std::isfinite(gyro.x) &&
                std::isfinite(gyro.y) &&
                std::isfinite(gyro.z) &&
                std::isfinite(accel.x) &&
                std::isfinite(accel.y) &&
                std::isfinite(accel.z)) {

                snapshot.inertial.valid = true;

                snapshot.inertial.headingDegrees = heading;
                snapshot.inertial.rotationDegrees = rotation;
                snapshot.inertial.pitchDegrees = pitch;
                snapshot.inertial.rollDegrees = roll;

                snapshot.inertial.angularVelocity = {
                    gyro.x, gyro.y, gyro.z
                };

                snapshot.inertial.acceleration = {
                    accel.x, accel.y, accel.z
                };
            }
        }
    }

    return snapshot;
}