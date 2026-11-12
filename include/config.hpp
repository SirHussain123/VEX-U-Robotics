#pragma once

namespace config {

    constexpr int BOTTOM_LEFT_PORT = 1;
    constexpr int BOTTOM_RIGHT_PORT = -2;
    constexpr int TOP_LEFT_PORT = 3;
    constexpr int TOP_RIGHT_PORT = -4;

    constexpr int MAX_DRIVE_RPM = 200;
    constexpr int JOYSTICK_DEADZONE = 10;
    constexpr int LOOP_DELAY_MS = 20;

    // Front distance sensor
    constexpr int FRONT_DISTANCE_PORT = 17;
    constexpr int FRONT_SENSOR_OFFSET_MM = 80;

    // Obstacle behavior
    constexpr int FRONT_STOP_CLEARANCE_MM = 200;
    constexpr int FRONT_RELEASE_CLEARANCE_MM = 250; 
}