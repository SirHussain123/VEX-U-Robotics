#include "subsystems/drivetrain.hpp"
#include "config.hpp"
#include <algorithm>


Drivetrain::Drivetrain()
    : leftMotors({config::BOTTOM_LEFT_PORT, config::TOP_LEFT_PORT}),
      rightMotors({config::BOTTOM_RIGHT_PORT, config::TOP_RIGHT_PORT}) {
}

void Drivetrain::initialize() {
    leftMotors.set_gearing_all(pros::E_MOTOR_GEARSET_18);
    rightMotors.set_gearing_all(pros::E_MOTOR_GEARSET_18);

    leftMotors.set_encoder_units_all(pros::E_MOTOR_ENCODER_DEGREES);
    rightMotors.set_encoder_units_all(pros::E_MOTOR_ENCODER_DEGREES);
}

void Drivetrain::tank(int leftSpeed, int rightSpeed) {
    leftSpeed = std::clamp(leftSpeed, -config::MAX_DRIVE_RPM, config::MAX_DRIVE_RPM);
    rightSpeed = std::clamp(rightSpeed, -config::MAX_DRIVE_RPM, config::MAX_DRIVE_RPM);

    leftMotors.move_velocity(leftSpeed);
    rightMotors.move_velocity(rightSpeed);
}

void Drivetrain::arcade(int forward, int turn) {
    tank(forward + turn, forward - turn);
}

void Drivetrain::stop() {
    tank(0, 0);
}

void Drivetrain::resetEncoders() {
    leftMotors.tare_position_all();
    rightMotors.tare_position_all();
}

void Drivetrain::moveRelative(double degrees, int velocity) {
    leftMotors.move_relative(degrees, velocity);
    rightMotors.move_relative(degrees, velocity);
}
void Drivetrain::moveRelative(double leftDegrees, double rightDegrees, int velocity) {
    leftMotors.move_relative(leftDegrees, velocity);
    rightMotors.move_relative(rightDegrees, velocity);
}
double Drivetrain::getLeftPosition() {
    return leftMotors.get_position();
}

double Drivetrain::getRightPosition() {
    return rightMotors.get_position();
}

