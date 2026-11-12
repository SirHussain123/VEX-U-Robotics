#include "autonomous/drive_action.hpp"
#include <cmath>

DriveAction::DriveAction(Drivetrain& drivetrain)
    : drivetrain(drivetrain) {
}

void DriveAction::start(double leftDegrees, double rightDegrees,
                        int velocity, unsigned int timeoutMs) {

    drivetrain.stop();

    leftTarget = drivetrain.getLeftPosition() + leftDegrees;
    rightTarget = drivetrain.getRightPosition() + rightDegrees;

    startTime = pros::millis();
    timeoutDurationMs = timeoutMs;
    status = Status::Running;
    
    movingForward = leftDegrees > 0 || rightDegrees > 0;

    drivetrain.moveRelative(leftDegrees, rightDegrees, velocity);
}

bool DriveAction::isMovingForward() const {
    return status == Status::Running && movingForward;
}

DriveAction::Status DriveAction::update() {

    if (status != Status::Running) {
        return status;
    }

    double leftError = std::abs(leftTarget - drivetrain.getLeftPosition());
    double rightError = std::abs(rightTarget - drivetrain.getRightPosition());

    const double tolerance = 10.0;

    if (leftError <= tolerance && rightError <= tolerance) {
        drivetrain.stop();
        status = Status::Completed;
    }
    else if (pros::millis() - startTime >= timeoutDurationMs) {
        drivetrain.stop();
        status = Status::TimedOut;
    }

    return status;
}

void DriveAction::cancel() {
    if (status == Status::Running) {
        drivetrain.stop();
    }

    status = Status::Idle;
}