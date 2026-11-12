#include "robot.hpp"

Robot::Robot()
    : master(pros::E_CONTROLLER_MASTER) {
}

void Robot::initialize() {
    drivetrain.initialize();
}

SensorSnapshot Robot::updateBehaviors() {

    SensorSnapshot snapshot = sensors.read();

    behaviors.update(snapshot);

    return snapshot;
}