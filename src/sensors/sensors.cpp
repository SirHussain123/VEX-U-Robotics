#include "sensors/sensors.hpp"
#include "config.hpp"
#include <algorithm>
#include <cmath>

Sensors::Sensors()
    : frontDistance(config::FRONT_DISTANCE_PORT),
      inertial(config::INERTIAL_PORT),
      aiVision(config::FRONT_AI_VISION_PORT)  {
}

void Sensors::initialize() {
    inertial.reset(false);

    aiVision.enable_detection_types(
        pros::AivisionModeType::all
    );
}

SensorSnapshot Sensors::read() {

    SensorSnapshot snapshot;
    
    // Distance sensor
    int rawDistance = frontDistance.get_distance();

    if (rawDistance > 2000 && rawDistance <= 9999) {
        // No object within usable detection range
        snapshot.frontDistance.valid = true;
        snapshot.frontDistance.detected = false;
    }
    else if (rawDistance >= 20 && rawDistance <= 2000) {
        // Object detected
        snapshot.frontDistance.valid = true;
        snapshot.frontDistance.detected = true;

        snapshot.frontDistance.millimeters = rawDistance;

        snapshot.frontDistance.clearanceMm = std::max(
            0, rawDistance - config::FRONT_SENSOR_OFFSET_MM
        );
    }
    // Other readings remain invalid (fail-safe)


    // AI Vision sensor
    int visionCount = aiVision.get_object_count();

    if (visionCount >= 0) {
        snapshot.aiVision.valid = true;

        for (int i = 0; i < visionCount; i++) {
            auto object = aiVision.get_object(i);

            VisionObjectReading reading;
            reading.id = object.id;

            if (pros::AIVision::is_type(
                    object, pros::AivisionDetectType::object)) {

                reading.type = VisionDetectionType::AIObject;

                const auto& box = object.object.element;

                reading.centerX = box.xoffset + box.width / 2;
                reading.centerY = box.yoffset + box.height / 2;
                reading.width = box.width;
                reading.height = box.height;
                reading.confidence = box.score;
            }
            else if (pros::AIVision::is_type(
                    object, pros::AivisionDetectType::color) ||
                     pros::AIVision::is_type(
                    object, pros::AivisionDetectType::code)) {

                reading.type = pros::AIVision::is_type(
                    object, pros::AivisionDetectType::color)
                    ? VisionDetectionType::Color
                    : VisionDetectionType::ColorCode;

                const auto& box = object.object.color;

                reading.centerX = box.xoffset + box.width / 2;
                reading.centerY = box.yoffset + box.height / 2;
                reading.width = box.width;
                reading.height = box.height;
            }
            else if (pros::AIVision::is_type(
                    object, pros::AivisionDetectType::tag)) {

                reading.type = VisionDetectionType::AprilTag;

                const auto& tag = object.object.tag;

                reading.centerX =
                    (tag.x0 + tag.x1 + tag.x2 + tag.x3) / 4;
                reading.centerY =
                    (tag.y0 + tag.y1 + tag.y2 + tag.y3) / 4;

                reading.width =
                    std::max({tag.x0, tag.x1, tag.x2, tag.x3}) -
                    std::min({tag.x0, tag.x1, tag.x2, tag.x3});

                reading.height =
                    std::max({tag.y0, tag.y1, tag.y2, tag.y3}) -
                    std::min({tag.y0, tag.y1, tag.y2, tag.y3});
            }
            else {
                continue;
            }

            snapshot.aiVision.objects.push_back(reading);
        }
    }

        // Inertial sensor
    int status = static_cast<int>(inertial.get_status());

    constexpr int CALIBRATING_BIT = 0x01;
    constexpr int ERROR_STATUS = 0xFF;

    if (status != ERROR_STATUS && status != -1) {

        snapshot.inertial.calibrating =
            (status & CALIBRATING_BIT) != 0;

        if (!snapshot.inertial.calibrating) {

            double heading = inertial.get_heading();
            double rotation = inertial.get_rotation();
            double pitch = inertial.get_pitch();
            double roll = inertial.get_roll();

            pros::imu_gyro_s_t gyro = inertial.get_gyro_rate();
            pros::imu_accel_s_t accel = inertial.get_accel();

            if (std::isfinite(heading) &&
                heading >= 0 && heading < 360 &&
                std::isfinite(rotation) &&
                std::isfinite(pitch) &&
                std::isfinite(roll) &&
                std::isfinite(gyro.x) &&
                std::isfinite(gyro.y) &&
                std::isfinite(gyro.z) &&
                std::isfinite(accel.x) &&
                std::isfinite(accel.y) &&
                std::isfinite(accel.z)) {

                snapshot.inertial.valid = true;

                snapshot.inertial.headingDegrees = heading;
                snapshot.inertial.rotationDegrees = rotation;
                snapshot.inertial.pitchDegrees = pitch;
                snapshot.inertial.rollDegrees = roll;

                snapshot.inertial.angularVelocity = {
                    gyro.x, gyro.y, gyro.z
                };

                snapshot.inertial.acceleration = {
                    accel.x, accel.y, accel.z
                };
            }
        }
    }

    return snapshot;
}