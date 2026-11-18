#include "main.h"
#include "drivetrains/theeb.hpp"

#include <cmath>
#include <optional>

template <std::size_t N>
std::optional<double> meanMotorDegrees(const std::array<std::int8_t, N>& ports,
                                       double ticksPerMotorTurn) {
    double sumTicks = 0;
    for (const auto signedPort : ports) {
        std::uint32_t timestamp{};
        const auto ticks = pros::c::motor_get_raw_position(std::abs(signedPort), &timestamp);
        if (ticks == PROS_ERR) return std::nullopt;
        sumTicks += signedPort < 0 ? -static_cast<double>(ticks) : ticks;
    }
    return sumTicks * 360.0 / (N * ticksPerMotorTurn);
}

void initialize() {
    pros::lcd::initialize();
    pros::lcd::set_text(0, "Theeb | hand-roll distance");
}

void opcontrol() {
    std::optional<double> startLeft, startRight;
    while (true) {
        const auto left = meanMotorDegrees(theeb::leftPorts, theeb::ticksPerMotorTurn);
        const auto right = meanMotorDegrees(theeb::rightPorts, theeb::ticksPerMotorTurn);
        if (left && right) {
            if (!startLeft) {
                startLeft = left;
                startRight = right;
            }
            const double leftMm = theeb::calibration.distanceMm(*left - *startLeft);
            const double rightMm = theeb::calibration.distanceMm(*right - *startRight);
            pros::lcd::print(1, "Left:   %+.1f cm", leftMm / 10.0);
            pros::lcd::print(2, "Right:  %+.1f cm", rightMm / 10.0);
            pros::lcd::print(3, "Travel: %+.1f cm", (leftMm + rightMm) / 20.0);
        } else {
            pros::lcd::set_text(1, left ? "Left OK" : "Check left motors");
            pros::lcd::set_text(2, right ? "Right OK" : "Check right motors");
            pros::lcd::set_text(3, "Restart at mark after reconnect");
            return;
        }
        pros::delay(100);
    }
}
