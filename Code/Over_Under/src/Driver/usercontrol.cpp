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

void SetLiftForCTier(){

  robot.BigElevate.setVelocity(-100, pct);
  robot.SmallElevate.setVelocity(-100, pct);
  robot.LiftRatchet.set(true);
  wait(0.1, sec);

  orientationType axis;
  double c_tier_position = 30;
  double last_error;
  double vel;

  if(std::abs(robot.LiftInertial.orientation(orientationType::pitch, deg)) > std::abs(robot.LiftInertial.orientation(orientationType::roll, deg))){
    axis = orientationType::pitch;
  } else {
    axis = orientationType::roll;
  }

  double error = wrapAngleDeg(c_tier_position - robot.LiftInertial.orientation(axis, deg));

  while (true) {

    last_error = error;
    error = wrapAngleDeg(c_tier_position - robot.LiftInertial.orientation(axis, deg));
    vel = error * 20;
    robot.BigElevate.setVelocity(vel, pct);
    robot.SmallElevate.setVelocity(vel, pct);

    if (GetSign(error) != GetSign(last_error)) {
      robot.BigElevate.setVelocity(0, pct);
      robot.SmallElevate.setVelocity(0, pct);
      return;
    }
    wait(20, msec);
  }
}
        

void LiftTask(){
  task Task = task([]()->int{

    robot.BigElevate.setStopping(hold);
    robot.SmallElevate.setStopping(hold);
    
    while (true) {
      if (Controller.ButtonUp.pressing()) {

        robot.BigElevate.setVelocity(-100, pct);
        robot.SmallElevate.setVelocity(-100, pct);
        robot.LiftRatchet.set(true);
        wait(0.1, sec);
        robot.BigElevate.setVelocity(100, pct);
        robot.SmallElevate.setVelocity(100, pct);
        waitUntil(!Controller.ButtonUp.pressing());

      } else if (Controller.ButtonDown.pressing()) {
          
        robot.LiftRatchet.set(false);
        robot.BigElevate.setVelocity(-100, pct);
        robot.SmallElevate.setVelocity(-100, pct);

      } else if (Controller.ButtonX.pressing()) {

        SetLiftForCTier();

      } else {

        robot.LiftRatchet.set(false);
        robot.BigElevate.setVelocity(0, pct);
        robot.SmallElevate.setVelocity(0, pct);

      }
      wait(20, msec);
    }
    return 0;
  });
}

void usercontrol(void) {

  Calibrate(false);
  ms.should_update = false;
  Brain.Screen.clearScreen();

  // Auto Driver Control Code
  if (ms.GetAlliance() == AllianceEnum::Skills){
      StartTimer();
      clearConsole();
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

  while (true && !DISABLED) {

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