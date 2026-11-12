#include "autonomous/autonomous.hpp"

Autonomous::Autonomous(Drivetrain& drivetrain)
    : driveAction(drivetrain) {
}

void Autonomous::startDriveTest() {

    if (isRunning()) {
        return;
    }

    driveAction.start(90, 90, 60, 2500);
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

    if (result == DriveAction::Status::TimedOut) {
        state = TestState::Failed;
        pros::lcd::set_text(3, "Test: TIMEOUT");
        return;
    }

    if (state == TestState::Forward &&
        result == DriveAction::Status::Completed) {

        driveAction.start(-90, -90, 60, 2500);
        state = TestState::Reverse;

        pros::lcd::set_text(3, "Test: Reverse");
    }
    else if (state == TestState::Reverse &&
             result == DriveAction::Status::Completed) {

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

bool Autonomous::isMovingForward() const {
    return isRunning() && driveAction.isMovingForward();
}