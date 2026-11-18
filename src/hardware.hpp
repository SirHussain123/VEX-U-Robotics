#pragma once
#include "pros/ai_vision.hpp"
#include "pros/imu.hpp"
#include "pros/motor_group.hpp"
#include "pros/motors.hpp"
#include "pros/optical.hpp"
#include "pros/rotation.hpp"

// Motors
inline pros::MotorGroup LEFT_MOTORS({-11,-12,-13,-14}, pros::MotorGearset::green);
inline pros::MotorGroup RIGHT_MOTORS({17,18,19,20}, pros::MotorGearset::green);
inline pros::MotorGroup ALL_MOTORS({-11,-12,-13,-14,17,18,19,20});
// Sensors 
inline pros::AIVision ai_vision = pros::AIVision(18); //(pros::AIVision::get_all_devices())[0];
inline pros::Imu imu = pros::Imu(2); //(pros::Imu::get_all_devices())[0];
inline pros::Optical optical = pros::Optical(13);