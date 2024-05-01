#include "vex.h"
using namespace vex;

double CatapultSpeed = 50;

void LiftTask(){
  task Task = task([]()->int{
    while (true)
    {
      wait(20, msec);
      robot.lift.updateFromDriverCommands();
    }  
  });
}

bool AdjustPuncherPosition = true;

void usercontrol(void) {

  Calibrate(false);
  ms.should_update = false;
  Brain.Screen.clearScreen();

  // Auto Driver Control Code
  if (ms.GetAlliance() == AllianceEnum::Skills){
    robot.Inertial.setHeading(45, deg);

    robot.Intake.setVelocity(100, pct);

    DriveWithAnglesAndSpeed({
      {-6, {45,  100}},
      {-6, {90, 50}},
      {-30, {90, 100}}
    }, 100, true, false, 1.5);

    robot.Intake.setVelocity(0, pct);

    DriveWithAnglesAndSpeed({
      {6, {45, 100}},
      {18, {169, 25}}
    }, 100, false, false, 2);

    robot.LeftWing.set(true);

    auto tsktsk = vex::task([]()->int{
      robot.lift.setPuncherPosition(&AdjustPuncherPosition);
      return 0;
    });
    
    wait(0.5, sec);
    robot.LaunchCatapultFor(48);
    AdjustPuncherPosition = false;

    robot.LeftWing.set(false);

  }

  robot.Cata.setBrake(coast); 

  Controller.ButtonL1.pressed(ToggleBothWings);
  Controller.ButtonA.pressed(ToggleRightWing);
  Controller.ButtonY.pressed(ToggleLeftWing);

  Controller.ButtonL2.pressed(ToggleBothBackWings);
  Controller.ButtonRight.pressed(ToggleRightBackWing);
  Controller.ButtonLeft.pressed(ToggleLeftBackWing);

  double max_turn_vel = 75;

  double right_vel;
  double left_vel;
  double target_right_vel;
  double target_left_vel;
  double target_turn_vel;
  double avg_speed;

  double startTime = Brain.Timer.value();

  LiftTask();

  while (true) {

    target_right_vel = Controller.Axis2.position();
    target_left_vel = Controller.Axis3.position();

    target_turn_vel = (target_right_vel - target_left_vel) * 0.5;
    avg_speed = (target_left_vel + target_right_vel) * 0.5;

    if (std::abs(target_turn_vel) > max_turn_vel) {
        right_vel = avg_speed + (max_turn_vel * 0.5 * GetSign(target_turn_vel));
        left_vel = avg_speed - (max_turn_vel * 0.5 * GetSign(target_turn_vel));
    }
    else {
        right_vel = target_right_vel;
        left_vel = target_left_vel;
    }

    RightDrive(setVelocity(right_vel, pct);)
    LeftDrive(setVelocity(left_vel, pct);)

    //Intake Control
    if (Controller.ButtonR1.pressing()) {
      robot.Intake.setVelocity(100, pct);
    } else if (Controller.ButtonR2.pressing()) {
      robot.Intake.setVelocity(-100, pct);
    } else {
      robot.Intake.setVelocity(0, pct);
    }

    wait(20, msec);
  }
  RightDrive(spin(forward, 0, pct);)
  LeftDrive(spin(forward, 0, pct);)
  robot.Intake.setVelocity(0, pct);
}