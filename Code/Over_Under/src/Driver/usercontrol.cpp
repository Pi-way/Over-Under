#include "vex.h"
using namespace vex;

void CatapultLaunch() {
  robot.LaunchCatapult();
}

// How long do the wing release cylinders need to be activated for to close the wings?
double wing_release_delay = 0.25;
// How long do the wing release cylinders need to be activated for to open the wings?
double wing_open_delay = 0.25;
double main_wing_open_delay = 0.75;

bool right_wing_open = false;
bool left_wing_open = false;

void ToggleRightWing(bool no_toggle, bool force_toggle_open) {
  // If nothing needs to be done
  if(no_toggle && (right_wing_open == force_toggle_open)){
    return;
  }

  if (right_wing_open || (no_toggle == true && !force_toggle_open)) {

    // If the wing is open and should be closed
    robot.WingReleaseCylinder.set(true);
    wait(wing_release_delay, seconds);
    robot.WingReleaseCylinder.set(false);

    right_wing_open = false;

  } else {
    // If the wing is closed and should be opened

    robot.RightWingCylinder.set(true);

    robot.WingReleaseCylinder.set(true);
    wait(wing_open_delay, seconds);
    robot.WingReleaseCylinder.set(false);

    wait(main_wing_open_delay, seconds);
    robot.RightWingCylinder.set(false);

    right_wing_open = true;

  }
}

void ToggleLeftWing(bool no_toggle, bool force_toggle_open) {
  // If nothing needs to be done
  if(no_toggle && (left_wing_open == force_toggle_open)){
    return;
  }

  if (left_wing_open || (no_toggle == true && !force_toggle_open)) {

    // If the wing is open and should be closed
    robot.WingReleaseCylinder.set(true);
    wait(wing_release_delay, seconds);
    robot.WingReleaseCylinder.set(false);

    left_wing_open = false;

  } else {
    // If the wing is closed and should be opened

    robot.LeftWingCylinder.set(true);

    robot.WingReleaseCylinder.set(true);
    wait(wing_open_delay, seconds);
    robot.WingReleaseCylinder.set(false);

    wait(main_wing_open_delay, seconds);
    robot.LeftWingCylinder.set(false);

    left_wing_open = true;
  }
}

void ToggleBothWings() {
  if (right_wing_open || left_wing_open) {
    //Force both wings shut
    vex::task PistonTask = vex::task([]()->int{
      ToggleRightWing(true, false);
      return 0;
    });
    ToggleLeftWing(true, false);
  } else {
    //Force both wings open
    vex::task PistonTask = vex::task([]()->int{
      ToggleRightWing(true, true);
      return 0;
    });
    ToggleLeftWing(true, true);
  }
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
      robot.RightIntake.setVelocity(60, pct);
      robot.LeftIntake.setVelocity(60, pct);
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