#include "services/aiVision.hpp"
#include "lemlib/pose.hpp"
#include "pros/ai_vision.h"
#include "pros/rtos.hpp"
#include "../hardware.hpp"

static pros::aivision_object_s currentTarget;
static pros::Mutex mutex;
static int TEAM_COLOR = 0; // Team blue by default

static void aiVisionTask(void*) {
  while (true) {
    pros::aivision_object_s next;
    auto objects = ai_vision.get_all_objects();
    int size = 0 ; // Small inital value
    //  Search for colsest ball
    for(auto &object: objects){
      if(int(object.id) == TEAM_COLOR){
        int new_size = object.object.element.height * object.object.element.width;
        if(new_size > size){next = object;}
      }
    }
    mutex.take();
    currentTarget = next;
    mutex.give();
    pros::delay(30); // vision update rate
  }
}

void aiVisionInit() {
	ai_vision.reset();
  ai_vision.enable_detection_types(pros::AivisionModeType::objects);
  static pros::Task task(aiVisionTask);
}

pros::aivision_object_s aiVisionGetTarget() {
  mutex.take();
  pros::aivision_object_s copy = currentTarget;
  mutex.give();
  return copy;
}

void aiVisionReset() {
  mutex.take();
  currentTarget = {};
  mutex.give();
}

lemlib::Pose getObjectOffset() {
    // Settings
    const double IMAGE_WIDTH = 320;
    const double BALL_WIDTH = 3.25;
    const double H_FOV = 61;
    // Calculations
    double fx = IMAGE_WIDTH / (2 * tan(H_FOV * M_PI / 360.0));
    double obj_center_x = currentTarget.object.element.xoffset 
            + currentTarget.object.element.width / 2.0;
    double obj_center_y = currentTarget.object.element.yoffset
          + currentTarget.object.element.height / 2.0;
    double pixelOffsetX = obj_center_x - 160; // object center offset from camera x-axis
    double pixelOffsetY = obj_center_y - 120; // object center offset from camera y-axis
    double theta_rad = atan(pixelOffsetX / fx);
    double y = (fx * BALL_WIDTH) / currentTarget.object.element.width;
    // Lateral offset
    double x = y * tan(theta_rad);
    // Angle in degrees
    double thetaDeg = theta_rad * 180.0 / M_PI;
    return lemlib::Pose(x, y-3, thetaDeg); // Minus distance of camera from the front of the roobt
}