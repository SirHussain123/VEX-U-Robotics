
#pragma once

#include "subsystems/drivetrain.hpp"

class SafeDrive {
public:
    explicit SafeDrive(Drivetrain& drivetrain);

    void setForwardBlocked(bool blocked);
    bool blocksForward() const;

    void arcade(int forward, int turn);
    bool moveRelative(double leftDegrees, double rightDegrees,
                      int velocity);

    bool wasRelativeMoveBlocked() const;

    double getLeftPosition();
    double getRightPosition();

    void stop();

private:
    Drivetrain& drivetrain;

    bool forwardBlocked = true;

    bool relativeActive = false;
    bool relativeForward = false;
    bool relativeBlocked = false;
};
