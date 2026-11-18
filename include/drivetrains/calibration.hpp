#pragma once

namespace drivetrain {

struct Gearing {
    int motorTeeth;
    int wheelTeeth;

    constexpr double wheelTurns(double motorDegrees) const {
        return motorDegrees / 360.0 * motorTeeth / wheelTeeth;
    }
};

struct Calibration {
    Gearing gearing;
    double mmPerWheelTurn;

    constexpr double distanceMm(double motorDegrees) const {
        return gearing.wheelTurns(motorDegrees) * mmPerWheelTurn;
    }

    constexpr double motorDegrees(double millimeters) const {
        return millimeters / distanceMm(1.0);
    }
};

} // namespace drivetrain
