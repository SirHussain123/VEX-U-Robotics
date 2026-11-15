
#pragma once

#include "safety/safe_drive.hpp"

class DriveAction {
public:
    enum class Status {
        Idle,
        Running,
        Completed,
        TimedOut,
        Blocked
    };

    explicit DriveAction(SafeDrive& safeDrive);

    bool start(double leftDegrees, double rightDegrees,
               int velocity, unsigned int timeoutMs);

    Status update();
    void cancel();

private:
    SafeDrive& safeDrive;

    Status status = Status::Idle;

    double leftTarget = 0;
    double rightTarget = 0;

    unsigned int startTime = 0;
    unsigned int timeoutDurationMs = 0;
};
