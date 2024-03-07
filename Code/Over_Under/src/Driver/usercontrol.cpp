#include "vex.h"
using namespace vex;

int TimersRunning = 0;
bool DISABLED = false;
void StartTimer(){
  auto timer_task = task([]()->int{
    double startTime = Brain.Timer.value();
    int time = 0;
    while(time < 60){//TimersRunning < 2 && time < 60){
      time = Brain.Timer.value() - startTime;
      clearConsole();
      std::cout << std::ceil(60.0 - (Brain.Timer.value() - startTime)) << std::endl;
      std::cout << " " << std::endl;
      wait(150, msec);
    }
    if (time >= 60){
      DISABLED = true;
    }
    Controller.rumble(".-.-.-");
    return 0;
  });
}

double CatapultSpeed = 100;
bool RunCata = false;
void ToggleCatapult(){
  RunCata = !RunCata;
  robot.Cata.spin(fwd);
  if (RunCata) {
    robot.Cata.setVelocity(CatapultSpeed, pct);
    if (ms.GetAlliance() == AllianceEnum::Skills){
      StartTimer();
    } 
  } else {
    robot.Cata.setVelocity(0, pct);
  }
}

bool shouldHold = false;

void usercontrol(void) {

  Calibrate(false);
  ms.should_update = false;
  Brain.Screen.clearScreen();

  if (ms.GetAlliance() == AllianceEnum::Skills){
      StartTimer();
      clearConsole();
			odom.Calibrate(-50, -56.15, 50);
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
			DriveWithAnglesAndSpeed(true, {{11, {75, 85}}}, 100, true, false, 1);
			robot.RightWing.set(true);
			TurnAtPoint({48, -6}, true, 100, false, false, 3);
			wait(0.75, sec);
			robot.LaunchCatapultUntilButtonPressed();
      robot.Intake.setVelocity(-100, pct);

			vex::task ball_set_up2 = vex::task([]()->int{
				robot.Cata.setBrake(coast);
				robot.RightWing.set(false);
				wait(0.5, sec);
				robot.Cata.setBrake(coast);
				return 0;
			});

  }

  robot.Cata.setBrake(coast); 

  Controller.ButtonL1.pressed([](){ToggleCatapult();}); 
  Controller.ButtonL2.pressed([](){ToggleBothWings();});
  Controller.ButtonB.pressed([](){ToggleRightWing();});
  Controller.ButtonDown.pressed([](){ToggleLeftWing();});
  Controller.ButtonUp.pressed([](){robot.MiniWing.set(!robot.MiniWing.value());});

  double right;
  double left;

  double startTime = Brain.Timer.value();
  while (true && !DISABLED) {

    //Drivetrain Control
    right = Controller.Axis2.position();
    left = Controller.Axis3.position();
    RightDrive(spin(forward, (right/100.0) * 12.0, volt);)
    LeftDrive(spin(forward, (left/100.0) * 12.0, volt);)

    //Intake Control
    if (Controller.ButtonR1.pressing()) {
      robot.Intake.setVelocity(100, pct);
    } else if (Controller.ButtonR2.pressing()) {
      robot.Intake.setVelocity(-100, pct);
    } else {
      robot.Intake.setVelocity(0, pct);
    }

    if (Controller.ButtonA.pressing() && Controller.ButtonRight.pressing()){
      robot.SideElevation.set(true);
      shouldHold = true;
    }

    clearConsole();

    if (std::abs(robot.Inertial.pitch(deg)) > 3 || shouldHold){
      RightDrive(setBrake(brake);)
      LeftDrive(setBrake(brake);)
    } else {
      RightDrive(setBrake(coast);)
      LeftDrive(setBrake(coast);)
    }
    wait(20, msec);
  }
  RightDrive(spin(forward, 0, pct);)
  LeftDrive(spin(forward, 0, pct);)
  robot.Intake.setVelocity(0, pct);
}