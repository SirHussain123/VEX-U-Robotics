#pragma once
#include "aiVision.hpp"
#include "pros/ai_vision.h"
struct VisionObject {
  bool valid;
  pros::aivision_object_s objRef;
};
/*
  Initializes the vision service task.
  Must be called once in initialize().
*/
void aiVisionInit();

/*
  Returns the most relevant object (or invalid).
  Fast, non-blocking, thread-safe.
*/
pros::aivision_object_s aiVisionGetTarget();

/*
  Optional: clears internal tracking state.
*/
void aiVisionReset();

int getObjectOffset();