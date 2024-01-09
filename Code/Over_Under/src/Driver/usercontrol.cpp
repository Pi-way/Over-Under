#include "vex.h"
using namespace vex;

void CatapultLaunch() {
  robot.LaunchCatapult();
}

void LaunchC() {
  robot.LaunchCatapult();
}

bool cataCoast = false;

void usercontrol(void) {

  Calibrate();
  ms.should_update = false;
  Brain.Screen.clearScreen();

  if (ms.GetAlliance() == AllianceEnum::Skills){
    	robot.Inertial.setHeading(41.5, deg);

			vex::task ball_set_up = vex::task([]()->int{
				robot.Intake.setVelocity(100, pct);
				robot.Intake.spin(fwd);
				wait(0.25, sec);
				robot.Intake.setVelocity(0, pct);
				return 0;
			});

			DriveWithAnglesAndSpeed({{-5, {40, 95}}, {-25, {90, 80}}}, 100, true, false, 2);
			
			DriveWithAnglesAndSpeed({{20, {7, 90}}});
			TurnAt(-20);
			robot.RightWing.set(true);
			robot.LaunchCatapultFor(47);
			robot.RightWing.set(false);
  }

  robot.Cata.setBrake(hold); 

  Controller.ButtonL1.pressed([](){cataCoast = false;CatapultLaunch();});
  Controller.ButtonL2.pressed([](){ToggleBothWings();});
  Controller.ButtonX.pressed([](){ToggleLift();});
  Controller.ButtonB.pressed([](){ToggleRightWing();});
  Controller.ButtonDown.pressed([](){ToggleLeftWing();});
  Controller.ButtonUp.pressed([](){cataCoast = !cataCoast;});

  double right;
  double left;

  while (true) {

    //Drivetrain Control
    right = Controller.Axis2.position();
    left = Controller.Axis3.position();
    robot.FR.setVelocity(right, pct);
    robot.FL.setVelocity(left, pct);
    robot.BR.setVelocity(right, pct);
    robot.BL.setVelocity(left, pct);
    robot.BBR.setVelocity(right, pct);
    robot.BBL.setVelocity(left, pct);

    //Intake Control
    if (Controller.ButtonR1.pressing()) {
      robot.Intake.setVelocity(100, pct);
    } else if (Controller.ButtonR2.pressing()) {
      robot.Intake.setVelocity(-100, pct);
    } else {
      robot.Intake.setVelocity(0, pct);
    }

    if (cataCoast) {
      robot.Cata.setBrake(coast);
    } else {
      robot.Cata.setBrake(hold);
    }

    wait(20, msec);
  }
}