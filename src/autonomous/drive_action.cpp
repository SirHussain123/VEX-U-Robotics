
#include "autonomous/drive_action.hpp"
#include <cmath>

DriveAction::DriveAction(SafeDrive& safeDrive)
    : safeDrive(safeDrive) {
}

bool DriveAction::start(double leftDegrees, double rightDegrees,
                        int velocity, unsigned int timeoutMs) {

    safeDrive.stop();

    leftTarget = safeDrive.getLeftPosition() + leftDegrees;
    rightTarget = safeDrive.getRightPosition() + rightDegrees;

    startTime = pros::millis();
    timeoutDurationMs = timeoutMs;

    if (!safeDrive.moveRelative(leftDegrees, rightDegrees, velocity)) {
        status = Status::Blocked;
        return false;
    }

    status = Status::Running;
    return true;
}

DriveAction::Status DriveAction::update() {

    if (status != Status::Running) {
        return status;
    }

    if (safeDrive.wasRelativeMoveBlocked()) {
        status = Status::Blocked;
        return status;
    }

    double leftError =
        std::abs(leftTarget - safeDrive.getLeftPosition());

    double rightError =
        std::abs(rightTarget - safeDrive.getRightPosition());

    const double tolerance = 10.0;

    if (leftError <= tolerance && rightError <= tolerance) {
        safeDrive.stop();
        status = Status::Completed;
    }
    else if (pros::millis() - startTime >= timeoutDurationMs) {
        safeDrive.stop();
        status = Status::TimedOut;
    }

    return status;
}

void DriveAction::cancel() {
    if (status == Status::Running) {
        safeDrive.stop();
    }

    status = Status::Idle;
}
