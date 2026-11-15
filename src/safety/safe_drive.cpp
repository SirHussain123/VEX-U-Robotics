
#include "safety/safe_drive.hpp"

SafeDrive::SafeDrive(Drivetrain& drivetrain)
    : drivetrain(drivetrain) {
}

void SafeDrive::setForwardBlocked(bool blocked) {
    forwardBlocked = blocked;

    if (forwardBlocked && relativeActive && relativeForward) {
        drivetrain.stop();

        relativeActive = false;
        relativeForward = false;
        relativeBlocked = true;
    }
}

bool SafeDrive::blocksForward() const {
    return forwardBlocked;
}

void SafeDrive::arcade(int forward, int turn) {
    if (forwardBlocked) {
        if (forward < 0) {
            drivetrain.arcade(forward, 0);
        }
        else {
            drivetrain.stop();
        }
        return;
    }

    drivetrain.arcade(forward, turn);
}

bool SafeDrive::moveRelative(double leftDegrees,
                             double rightDegrees,
                             int velocity) {

    bool movingForward = leftDegrees > 0 || rightDegrees > 0;

    if (forwardBlocked && movingForward) {
        drivetrain.stop();

        relativeActive = false;
        relativeForward = false;
        relativeBlocked = true;

        return false;
    }

    drivetrain.stop();

    relativeActive = true;
    relativeForward = movingForward;
    relativeBlocked = false;

    drivetrain.moveRelative(leftDegrees, rightDegrees, velocity);
    return true;
}

bool SafeDrive::wasRelativeMoveBlocked() const {
    return relativeBlocked;
}

double SafeDrive::getLeftPosition() {
    return drivetrain.getLeftPosition();
}

double SafeDrive::getRightPosition() {
    return drivetrain.getRightPosition();
}

void SafeDrive::stop() {
    drivetrain.stop();

    relativeActive = false;
    relativeForward = false;
    relativeBlocked = false;
}
