#include "robot.hpp"

Robot::Robot()
    : master(pros::E_CONTROLLER_MASTER),
      safeDrive(drivetrain) {
}

void Robot::initialize() {
    drivetrain.initialize();
    sensors.initialize();
}

SensorSnapshot Robot::updateBehaviors() {

    SensorSnapshot snapshot = sensors.read();

    behaviors.update(snapshot);

    safeDrive.setForwardBlocked(behaviors.blocksForward());

    return snapshot;
}