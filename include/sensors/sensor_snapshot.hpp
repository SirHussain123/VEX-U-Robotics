#pragma once
#include <vector>
// Distance sensor reading
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
// Inertial sensor reading
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
// AI Vision sensor reading
enum class VisionDetectionType {
    Unknown,
    Color,
    ColorCode,
    AIObject,
    AprilTag
};

struct VisionObjectReading {
    VisionDetectionType type = VisionDetectionType::Unknown;

    int id = -1;
    int centerX = 0;
    int centerY = 0;
    int width = 0;
    int height = 0;
    int confidence = 0;
};

struct AIVisionReading {
    bool valid = false;
    std::vector<VisionObjectReading> objects;
};


struct SensorSnapshot {
    DistanceReading frontDistance;
    InertialReading inertial;
    AIVisionReading aiVision;

};