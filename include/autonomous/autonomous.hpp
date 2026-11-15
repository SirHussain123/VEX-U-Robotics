
#pragma once

#include "autonomous/drive_action.hpp"

class Autonomous {
public:
    explicit Autonomous(SafeDrive& safeDrive);

    void startDriveTest();
    void update();
    void cancel();

    bool isRunning() const;

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
