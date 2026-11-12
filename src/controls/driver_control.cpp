#include "controls/driver_control.hpp"
#include "config.hpp"

DriverControl::DriverControl(Robot& robot, Autonomous& autonomous)
    : robot(robot), autonomous(autonomous) {
}

void DriverControl::run() {

    while (true) {

        // Update sensor readings and behaviors
        SensorSnapshot readings = robot.updateBehaviors();
        bool blocked = robot.behaviors.blocksForward();

        // Display distance information
        if (!readings.frontDistance.valid) {
            pros::lcd::set_text(4, "Front: Sensor Error");
        }
        else if (!readings.frontDistance.detected) {
            pros::lcd::set_text(4, "Front: No Target");
        }
        else {
            pros::lcd::print(4, "Front: %d mm    ",
                             readings.frontDistance.clearanceMm);
        }

        pros::lcd::set_text(5, blocked ? "Forward: BLOCKED"
                                       : "Forward: Allowed");

        // Software stop toggle
        if (robot.master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_X)) {
            stopped = !stopped;

            if (stopped) {
                autonomous.cancel();
            }
        }

        if (stopped) {
            robot.drivetrain.stop();
            pros::lcd::set_text(1, "Stopped");
            pros::delay(config::LOOP_DELAY_MS);
            continue;
        }

        // Temporary autonomous test
        if (robot.master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_Y)
            && !autonomous.isRunning()) {

            if (!blocked) {
                robot.drivetrain.stop();
                autonomous.startDriveTest();
            }
            else {
                pros::lcd::set_text(3, "Test: Front Blocked");
            }
        }

        // Autonomous has drivetrain control
        if (autonomous.isRunning()) {

            if (blocked && autonomous.isMovingForward()) {
                autonomous.cancel();
                robot.drivetrain.stop();
                pros::lcd::set_text(3, "Test: Obstacle");
            }
            else {
                autonomous.update();
            }

            pros::delay(config::LOOP_DELAY_MS);
            continue;
        }

        // Manual movement
        int forward = 0;

        if (robot.master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
            forward = 200;
        }
        else if (robot.master.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) {
            forward = -200;
        }
        else if (robot.master.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
            forward = 100;
        }
        else if (robot.master.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) {
            forward = -100;
        }

        int turn = robot.master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_X);

        if (turn > -config::JOYSTICK_DEADZONE &&
            turn < config::JOYSTICK_DEADZONE) {
            turn = 0;
        }

        // Apply obstacle safety
        if (blocked) {

            if (forward < 0) {
                robot.drivetrain.arcade(forward, 0);
            }
            else {
                robot.drivetrain.stop();
            }
        }
        else {
            robot.drivetrain.arcade(forward, turn);
        }

        pros::lcd::set_text(1, "Driving");
        pros::delay(config::LOOP_DELAY_MS);
    }
}