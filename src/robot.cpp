#include "robot.hpp"
#include "config.hpp"

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

    safeDrive.setForwardBlocked(
    config::ENABLE_OBSTACLE_STOP && behaviors.blocksForward()
    );

    return snapshot;
}