#pragma once

struct DistanceReading {
    bool valid = false;
    bool detected = false;

    int millimeters = 0;
    int clearanceMm = 0;
};

struct Vector3 {
    double x = 0;
    double y = 0;
    double z = 0;
};

struct InertialReading {
    bool valid = false;
    bool calibrating = false;

    double headingDegrees = 0;
    double rotationDegrees = 0;
    double pitchDegrees = 0;
    double rollDegrees = 0;
    
    Vector3 angularVelocity;
    Vector3 acceleration;
};

struct SensorSnapshot {
    DistanceReading frontDistance;
    InertialReading inertial;
};