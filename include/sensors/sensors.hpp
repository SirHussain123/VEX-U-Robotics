#pragma once

#include "main.h"
#include "sensors/sensor_snapshot.hpp"

class Sensors {
public:
    Sensors();
    void initialize();
    SensorSnapshot read();

private:
    pros::Distance frontDistance;
    pros::Imu inertial;
};