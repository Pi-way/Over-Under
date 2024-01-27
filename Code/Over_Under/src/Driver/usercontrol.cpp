#include "vex.h"
using namespace vex;

void CatapultLaunch() {
  robot.LaunchCatapult();
}

void LaunchC() {
  robot.LaunchCatapult();
}

void usercontrol(void) {

  Calibrate(false);
  ms.should_update = false;
  Brain.Screen.clearScreen();

  if (ms.GetAlliance() == AllianceEnum::Skills){
			odom.Calibrate(-50, -56.15, 46.5);
			auto ODOM = new vex::task(updateOdometry);
			robot.Inertial.setHeading(46.5, deg);

			vex::task ball_set_up = vex::task([]()->int{
			robot.Intake.setVelocity(100, pct);
			robot.Intake.spin(fwd);
			wait(0.25, sec);
			robot.Intake.setVelocity(0, pct);
			return 0;
			});

			DriveWithAnglesAndSpeed({{-7, {42, 85}},{-8, {90, 25}},{-30, {90, 75}}}, 100, true, false, 1.5);
			DriveWithAnglesAndSpeed({{13, {60, 85}}}, 100, true, false, 1);
			TurnAtPoint({48, -12}, true, 100, false, false, 3);
			wait(0.75, sec);
			robot.RightWing.set(true);
			robot.LaunchCatapultFor(46);

			vex::task ball_set_up2 = vex::task([]()->int{
				robot.Cata.setBrake(coast);
				robot.RightWing.set(false);
				wait(0.5, sec);
				robot.Cata.setBrake(coast);
				return 0;
			});

  }

  robot.Cata.setBrake(coast); 

  Controller.ButtonL1.pressed([](){letrun = false;CatapultLaunch();});
  Controller.ButtonL2.pressed([](){ToggleBothWings();});
  Controller.ButtonX.pressed([](){ToggleLift();});
  Controller.ButtonB.pressed([](){ToggleRightWing();});
  Controller.ButtonDown.pressed([](){ToggleLeftWing();});
  Controller.ButtonUp.pressed([](){letrun = false;robot.LaunchCatapultButNot();});

  double right;
  double left;

  double startTime = Brain.Timer.value();
  while (true) {

    if(Brain.Timer.value() - startTime > 95){
      RightDrive(setBrake(hold);)
      LeftDrive(setBrake(hold);)
    }

    robot.Cata.setBrake(coast);

    //Drivetrain Control
    right = Controller.Axis2.position();
    left = Controller.Axis3.position();
    RightDrive(spin(forward, (right/100.0) * 12.0, volt);)
    LeftDrive(spin(forward, (left/100.0) * 12.0, volt);)

    //Intake Control
    if (Controller.ButtonR1.pressing()) {
      if (robot.RightLift.value()){
        robot.Intake.setVelocity(-100, pct);
      } else {
        robot.Intake.setVelocity(100, pct);
      }
    } else if (Controller.ButtonR2.pressing()) {
      robot.Intake.setVelocity(-100, pct);
    } else {
      robot.Intake.setVelocity(0, pct);
    }

    if (Controller.ButtonA.pressing() && Controller.ButtonRight.pressing()){
      robot.SideElevation.set(true);
    }

    wait(20, msec);
  }
}