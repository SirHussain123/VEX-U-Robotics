
#pragma once

#include "main.h"
#include "subsystems/drivetrain.hpp"
#include "sensors/sensors.hpp"
#include "behaviors/behavior_manager.hpp"
#include "safety/safe_drive.hpp"

class Robot {
private:
    Drivetrain drivetrain;

public:
    Robot();

    void initialize();
    SensorSnapshot updateBehaviors();

    pros::Controller master;
    Sensors sensors;
    BehaviorManager behaviors;
    SafeDrive safeDrive;
};
