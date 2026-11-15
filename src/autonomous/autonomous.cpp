
#include "autonomous/autonomous.hpp"

Autonomous::Autonomous(SafeDrive& safeDrive)
    : driveAction(safeDrive) {
}

void Autonomous::startDriveTest() {

    if (isRunning()) {
        return;
    }

    if (!driveAction.start(90, 90, 60, 2500)) {
        state = TestState::Failed;
        pros::lcd::set_text(3, "Test: Front Blocked");
        return;
    }

    state = TestState::Forward;
    pros::lcd::set_text(3, "Test: Forward");
}

void Autonomous::update() {

    if (!isRunning()) {
        return;
    }

    DriveAction::Status result = driveAction.update();

    if (result == DriveAction::Status::Running) {
        return;
    }

    if (result == DriveAction::Status::Blocked) {
        state = TestState::Failed;
        pros::lcd::set_text(3, "Test: Obstacle");
        return;
    }

    if (result == DriveAction::Status::TimedOut) {
        state = TestState::Failed;
        pros::lcd::set_text(3, "Test: TIMEOUT");
        return;
    }

    if (result != DriveAction::Status::Completed) {
        return;
    }

    if (state == TestState::Forward) {

        if (!driveAction.start(-90, -90, 60, 2500)) {
            state = TestState::Failed;
            pros::lcd::set_text(3, "Test: Blocked");
            return;
        }

        state = TestState::Reverse;
        pros::lcd::set_text(3, "Test: Reverse");
    }
    else if (state == TestState::Reverse) {

        state = TestState::Complete;
        pros::lcd::set_text(3, "Test: PASSED");
    }
}

void Autonomous::cancel() {

    if (isRunning()) {
        driveAction.cancel();
        pros::lcd::set_text(3, "Test: Cancelled");
    }

    state = TestState::Idle;
}

bool Autonomous::isRunning() const {
    return state == TestState::Forward ||
           state == TestState::Reverse;
}
