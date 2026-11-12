#pragma once

#include "main.h"
#include "sensors/sensor_snapshot.hpp"

class Sensors {
public:
    Sensors();

    SensorSnapshot read();

private:
    pros::Distance frontDistance;
};