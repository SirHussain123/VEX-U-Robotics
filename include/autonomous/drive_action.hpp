#pragma once

#include "subsystems/drivetrain.hpp"

class DriveAction {
public:

    bool isMovingForward() const;

    enum class Status {
        Idle,
        Running,
        Completed,
        TimedOut
    };

    DriveAction(Drivetrain& drivetrain);

void start(double leftDegrees, double rightDegrees,
           int velocity, unsigned int timeoutMs);
    Status update();
    void cancel();

private:
    bool movingForward = false;

    Drivetrain& drivetrain;
    
    Status status = Status::Idle;

    double leftTarget = 0;
    double rightTarget = 0;

    unsigned int startTime = 0;
    unsigned int timeoutDurationMs = 0;
};