#include "main.h"
#include "drivetrains/theeb.hpp"

#include <array>
#include <cstdlib>
#include <cstdint>

void initialize() {
    pros::lcd::initialize();
}

void opcontrol() {
    constexpr auto motorCount = theeb::leftPorts.size() + theeb::rightPorts.size();
    std::array<std::int32_t, motorCount> baseline{};
    std::array<bool, motorCount> initialized{};
    while (true) {
        std::size_t row = 0;
        for (const auto& side : theeb::sides) {
            for (int signedPort : side) {
                const auto i = row++;
                const int port = std::abs(signedPort);
                std::uint32_t timestamp{};
                auto ticks = pros::c::motor_get_raw_position(port, &timestamp);
                if (ticks == PROS_ERR) {
                    pros::lcd::print(i, "Port %d: read error", port);
                    continue;
                }
                if (!initialized[i]) {
                    baseline[i] = ticks;
                    initialized[i] = true;
                }
                pros::lcd::print(i, "Port %d: %+lld ticks", port,
                                 static_cast<long long>(ticks) - baseline[i]);
            }
        }
        pros::delay(100);
    }
}
