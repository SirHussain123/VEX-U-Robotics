#pragma once

#include "main.h"
#include "subsystems/drivetrain.hpp"
#include "sensors/sensors.hpp"
#include "behaviors/behavior_manager.hpp"

class Robot {
public:
    Robot();

    void initialize();

    SensorSnapshot updateBehaviors();

    Drivetrain drivetrain;
    pros::Controller master;
    Sensors sensors;
    BehaviorManager behaviors;
};