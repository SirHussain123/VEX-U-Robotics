#pragma once

#include "autonomous/drive_action.hpp"

class Autonomous {
public:
    Autonomous(Drivetrain& drivetrain);

    void startDriveTest();
    void update();
    void cancel();

    bool isRunning() const;
    bool isMovingForward() const;
private:
    enum class TestState {
        Idle,
        Forward,
        Reverse,
        Complete,
        Failed
    };

    DriveAction driveAction;
    TestState state = TestState::Idle;
};