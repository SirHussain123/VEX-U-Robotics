#pragma once

struct DistanceReading {
    bool valid = false;
    bool detected = false;

    int millimeters = 0;
    int clearanceMm = 0;
};

struct SensorSnapshot {
    DistanceReading frontDistance;
};