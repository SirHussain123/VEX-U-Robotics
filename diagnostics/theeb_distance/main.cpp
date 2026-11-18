#include "main.h"
#include "drivetrains/theeb.hpp"

#include <array>
#include <cmath>

namespace {
using namespace pros::c;
#ifndef THEEB_TEST_CM
#define THEEB_TEST_CM 50
#endif
constexpr int targetCm = THEEB_TEST_CM;
static_assert(targetCm == 50 || targetCm == 100);
constexpr double targetMm = 10.0 * targetCm;
constexpr double targetDegrees = theeb::calibration.motorDegrees(targetMm);
static_assert(targetDegrees > 0);
constexpr int speedRpm = 30;
// Preserve the tested six-second allowance per 50 cm at this speed.
constexpr unsigned driveTimeoutMs = 6000 * targetCm / 50;
std::array<double, 22> startPosition{};

void brakeAll() {
    for (const auto& side : theeb::sides) {
        for (int port : side) {
            motor_brake(std::abs(port));
        }
    }
}

bool prepare() {
    for (const auto& side : theeb::sides) {
        for (int signedPort : side) {
            const int port = std::abs(signedPort);
            if (motor_set_gearing(port, pros::E_MOTOR_GEARSET_18) != 1 ||
                motor_set_encoder_units(port, pros::E_MOTOR_ENCODER_DEGREES) != 1 ||
                motor_set_brake_mode(port, pros::E_MOTOR_BRAKE_BRAKE) != 1) return false;
            startPosition[port] = motor_get_position(port);
            if (!std::isfinite(startPosition[port])) return false;
        }
    }
    return true;
}

bool readTravel(double& meanMm, bool& atTarget) {
    double total = 0;
    unsigned count = 0;
    atTarget = true;
    for (const auto& side : theeb::sides) {
        for (int signedPort : side) {
            const int port = std::abs(signedPort);
            const double delta = motor_get_position(port) - startPosition[port];
            if (!std::isfinite(delta)) return false;
            const double forward = signedPort < 0 ? -delta : delta;
            total += forward;
            ++count;
            if (std::abs(targetDegrees - forward) > 3.0) atTarget = false;
        }
    }
    meanMm = theeb::calibration.distanceMm(total / count);
    return true;
}

constexpr int startLeft = 65;
constexpr int startRight = 415;
constexpr int startTop = 70;
constexpr int startBottom = 185;
std::int32_t startPressCount{};
bool controllerStarted = false;

void showStatus(const char* message) {
    screen_set_eraser(0x000000);
    screen_erase();
    screen_set_pen(0xffffff);
    screen_print(pros::E_TEXT_MEDIUM_CENTER, 0, "Theeb | %d cm test", targetCm);
    screen_print(pros::E_TEXT_MEDIUM_CENTER, 2, "%s", message);
}

bool waitForStart() {
    showStatus("Tap START or press controller Y");
    screen_set_pen(0x176a47);
    screen_fill_rect(startLeft, startTop, startRight, startBottom);
    screen_set_pen(0xffffff);
    screen_print_at(pros::E_TEXT_LARGE, 140, 125, "START %d CM", targetCm);

    controllerStarted = false;
    bool yReleased = false;
    int lastConnection = -1;
    bool ready = false;
    bool pressedInside = false;
    while (true) {
        const auto touch = screen_touch_status();
        if (touch.touch_status == pros::E_TOUCH_ERROR) return false;
        const bool connected = controller_is_connected(pros::E_CONTROLLER_MASTER) == 1;
        if (connected != lastConnection) {
            screen_print(pros::E_TEXT_MEDIUM_CENTER, 10, "%s",
                         connected ? "Controller linked: Y=start X=stop" : "Controller offline: use touchscreen");
            lastConnection = connected;
        }
        const bool canUseY = connected && touch.touch_status == pros::E_TOUCH_RELEASED &&
            controller_get_digital(pros::E_CONTROLLER_MASTER, pros::E_CONTROLLER_DIGITAL_X) == 0;
        const int y = canUseY ? controller_get_digital(pros::E_CONTROLLER_MASTER, pros::E_CONTROLLER_DIGITAL_Y) : -1;
        const bool startWithY = y == 1 && yReleased;
        yReleased = y == 0;
        if (touch.touch_status == pros::E_TOUCH_RELEASED) {
            if (pressedInside || startWithY) {
                controllerStarted = startWithY;
                startPressCount = touch.press_count;
                return true;
            }
            ready = true;
        } else if (ready) {
            const bool inside = touch.x >= startLeft && touch.x <= startRight &&
                                touch.y >= startTop && touch.y <= startBottom;
            if (!inside) ready = false;
            if (inside && !pressedInside)
                screen_print(pros::E_TEXT_MEDIUM_CENTER, 9, "Touch received - release to start");
            pressedInside = inside;
        }
        pros::delay(10);
    }
}

bool stopRequested() {
    const auto touch = screen_touch_status();
    const bool connected = controller_is_connected(pros::E_CONTROLLER_MASTER) == 1;
    return (controllerStarted && !connected) || touch.touch_status != pros::E_TOUCH_RELEASED ||
           touch.press_count != startPressCount ||
           (connected && controller_get_digital(pros::E_CONTROLLER_MASTER, pros::E_CONTROLLER_DIGITAL_X) != 0);
}

const char* driveOnce() {
    for (const auto& side : theeb::sides) {
        for (int signedPort : side) {
            const int port = std::abs(signedPort);
            const double delta = signedPort < 0 ? -targetDegrees : targetDegrees;
            if (motor_move_absolute(port, startPosition[port] + delta, speedRpm) != 1)
                return "Motor command error";
        }
    }
    const auto start = pros::millis();
    while (pros::millis() - start < driveTimeoutMs) {
        if (stopRequested()) return "Stopped";
        double mm;
        bool atTarget;
        if (!readTravel(mm, atTarget)) return "Encoder error";
        if (atTarget) return "Target reached";
        pros::delay(20);
    }
    return "Timed out";
}
}

void initialize() {
    showStatus("Waiting for driver mode");
}

void disabled() { brakeAll(); }
void autonomous() { brakeAll(); }

void opcontrol() {
    if (!waitForStart()) {
        showStatus("Touch read error - restart test");
        return;
    }
    showStatus("Starting in 2s | X or touch cancels");
    const auto countdownStart = pros::millis();
    while (pros::millis() - countdownStart < 2000) {
        if (stopRequested()) {
            showStatus("Cancelled. Restart to repeat");
            return;
        }
        pros::delay(20);
    }
    if (!prepare() || stopRequested()) {
        brakeAll();
        showStatus("Start cancelled. Check motors");
        return;
    }
    showStatus("Driving | X or touch to stop");
    const char* result = driveOnce();
    brakeAll();
    showStatus(result);
    pros::delay(300);
    double mm;
    bool atTarget;
    if (readTravel(mm, atTarget)) screen_print(pros::E_TEXT_MEDIUM_CENTER, 4, "Encoders: %.1f cm", mm / 10.0);
    screen_print(pros::E_TEXT_MEDIUM_CENTER, 6, "Restart program to repeat");
}
