#include "vex.h"
using namespace vex;

void CatapultLaunch() {
  robot.LaunchCatapult();
}

void usercontrol(void) {

  Callibrate();
  ms.should_update = false;
  Brain.Screen.clearScreen();
  Controller.ButtonL1.pressed(CatapultLaunch);

  double right;
  double left;

  while (true) {
    right = Controller.Axis2.position() * (12.0 / 100.0);
    left = Controller.Axis3.position() * (12.0 / 100.0);

    robot.FR.spin(forward, right, voltageUnits::volt);
    robot.FL.spin(forward, left, voltageUnits::volt);

    robot.BR.spin(forward, right, voltageUnits::volt);
    robot.BL.spin(forward, left, voltageUnits::volt);
    
    robot.BBR.spin(forward, right, voltageUnits::volt);
    robot.BBL.spin(forward, left, voltageUnits::volt);

    wait(20, msec);
  }
}