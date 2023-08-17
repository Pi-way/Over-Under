#pragma once

#include <cmath>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <functional>
#include <vector>
#include <sstream>


#include "v5.h"
#include "v5_vcs.h"

#include "util.h"
#include "Robot/tracking_wheel.h"
//#include "Robot/odometry.h"
#include "Auton/callibrate.h"
#include "Auton/match_selector.h"
#include "Robot/robot.h"

#include "Auton/PID.h"


#define waitUntil(condition)                                                   \
  do {                                                                         \
    wait(5, msec);                                                             \
  } while (!(condition))

#define repeat(iterations)                                                     \
  for (int iterator = 0; iterator < iterations; iterator++)