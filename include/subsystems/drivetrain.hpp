#pragma once

#include "main.h"

class Drivetrain {
public:
    Drivetrain();

    void initialize();

    void tank(int leftSpeed, int rightSpeed);
    void arcade(int forward, int turn);
    void stop();

    void resetEncoders();
    void moveRelative(double degrees, int velocity);
    void moveRelative(double leftDegrees, double rightDegrees, int velocity);

    double getLeftPosition();
    double getRightPosition();

private:
    pros::MotorGroup leftMotors;
    pros::MotorGroup rightMotors;
};