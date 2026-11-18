#include "../hardware.hpp"
#include "pros/rtos.hpp"
#include "services/optical.hpp"

static pros::Mutex mutex;
static double currentColor;

void opticalTask(void*){
    mutex.take();
    currentColor = optical.get_hue();
    mutex.give();
    pros::delay(30);
}

void initOptical() {
    optical.set_led_pwm(100);
    static pros::Task task(opticalTask);
}

double opticalGetColor() {
    mutex.take();
    double color = currentColor;
    mutex.give();
    return color;
}