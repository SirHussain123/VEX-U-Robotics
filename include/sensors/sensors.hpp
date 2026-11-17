#pragma once

#include "main.h"
#include "sensors/sensor_snapshot.hpp"
#include "pros/ai_vision.hpp"

class Sensors {
public:
    Sensors();
    void initialize();
    SensorSnapshot read();

private:
    pros::Distance frontDistance;
    pros::Imu inertial;
    pros::AIVision aiVision;
};