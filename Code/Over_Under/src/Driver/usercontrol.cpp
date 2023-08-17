#include "vex.h"
using namespace vex;

void usercontrol(void) {

  Callibrate();
  ms.should_update = false;
  Brain.Screen.clearScreen();

  double right;
  double left;

  while (true) {
    right = Controller.Axis2.position() / 1.5;
    left = Controller.Axis3.position() / 1.5;

    robot.FR.setVelocity(right, pct);
    robot.FL.setVelocity(left, pct);

    robot.BR.setVelocity(right, pct);
    robot.BL.setVelocity(left, pct);
    
    robot.BBR.setVelocity(right, pct);
    robot.BBL.setVelocity(left, pct);

    wait(20, msec);
  }
}