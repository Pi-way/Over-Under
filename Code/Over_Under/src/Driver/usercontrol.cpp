#include "vex.h"
using namespace vex;

void CatapultLaunch() {
  robot.LaunchCatapult();
}

void usercontrol(void) {

  Calibrate();
  ms.should_update = false;
  Brain.Screen.clearScreen();

  Controller.ButtonL1.pressed(CatapultLaunch);
  Controller.ButtonL2.pressed(ToggleBothWings);
  Controller.ButtonB.pressed([](){ToggleRightWing();});
  Controller.ButtonDown.pressed([](){ToggleLeftWing();});

  double right;
  double left;

  robot.IntakeCylinder.set(true);

  while (true) {

    //Drivetrain Control
    right = Controller.Axis2.position() * (12.0 / 100.0);
    left = Controller.Axis3.position() * (12.0 / 100.0);
    robot.FR.spin(forward, right, voltageUnits::volt);
    robot.FL.spin(forward, left, voltageUnits::volt);
    robot.BR.spin(forward, right, voltageUnits::volt);
    robot.BL.spin(forward, left, voltageUnits::volt);
    robot.BBR.spin(forward, right, voltageUnits::volt);
    robot.BBL.spin(forward, left, voltageUnits::volt);

    //Intake Control
    if (Controller.ButtonR1.pressing()) {
      robot.RightIntake.setVelocity(100, pct);
      robot.LeftIntake.setVelocity(100, pct);
    } else if (Controller.ButtonR2.pressing()) {
      robot.RightIntake.setVelocity(-100, pct);
      robot.LeftIntake.setVelocity(-100, pct);
    } else {
      robot.RightIntake.setVelocity(0, pct);
      robot.LeftIntake.setVelocity(0, pct);
    }



    wait(20, msec);
  }
}