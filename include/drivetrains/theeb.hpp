#pragma once

#include "drivetrains/calibration.hpp"

#include <array>
#include <cstdint>

namespace theeb {

inline constexpr std::array<std::int8_t, 4> leftPorts{-11, -12, -13, -14};
inline constexpr std::array<std::int8_t, 4> rightPorts{17, 18, 19, 20};
inline constexpr std::array sides{leftPorts, rightPorts};
inline constexpr double ticksPerMotorTurn = 900.0; // Green 200 RPM cartridge.

inline constexpr drivetrain::Gearing gearing{60, 36};
// A hand roll covered 790 mm in 657 motor degrees. A powered 500 mm run averaged 491.5 mm.
inline constexpr double measuredMmPerWheelTurn = 790.0 / gearing.wheelTurns(657.0);
inline constexpr drivetrain::Calibration calibration{gearing, measuredMmPerWheelTurn * (491.5 / 500.0)};

static_assert(gearing.wheelTurns(216.0) == 1.0);

} // namespace theeb
