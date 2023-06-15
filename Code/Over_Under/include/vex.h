#pragma once

#include <sstream>
#include <cmath>

#include "v5.h"
#include "v5_vcs.h"

#include "util.h"
#include "Robot/tracking_wheel.h"
#include "Robot/odometry.h"
#include "Robot/robot.h"

#define waitUntil(condition)                                                   \
  do {                                                                         \
    wait(5, msec);                                                             \
  } while (!(condition))

#define repeat(iterations)                                                     \
  for (int iterator = 0; iterator < iterations; iterator++)