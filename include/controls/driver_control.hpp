#pragma once

#include "robot.hpp"
#include "autonomous/autonomous.hpp"

class DriverControl {
public:
    DriverControl(Robot& robot, Autonomous& autonomous);

    void run();

private:
    Robot& robot;
    Autonomous& autonomous;

    bool stopped = false;
};