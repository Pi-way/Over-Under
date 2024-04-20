#include "vex.h"
using namespace vex;

Robot::Robot() {
}

void ToggleRightWing() {
  robot.RightWing.set(!robot.RightWing.value());
}

void ToggleLeftWing() {
  robot.LeftWing.set(!robot.LeftWing.value());
}

void ToggleBothWings() {
  if (robot.RightWing.value() || robot.LeftWing.value()) {
    robot.RightWing.set(false);
    robot.LeftWing.set(false);
  } else {
    robot.RightWing.set(true);
    robot.LeftWing.set(true);
  }
}

void ToggleRightBackWing() {
  robot.RightBackWing.set(!robot.RightBackWing.value());
}

void ToggleLeftBackWing() {
  robot.LeftBackWing.set(!robot.LeftBackWing.value());
}

void ToggleBothBackWings() {
  if (robot.RightBackWing.value() || robot.LeftBackWing.value()) {
    robot.RightBackWing.set(false);
    robot.LeftBackWing.set(false);
  } else {
    robot.RightBackWing.set(true);
    robot.LeftBackWing.set(true);
  }
}


void Robot::LaunchCatapultFor(int n){

  double lPosition;
  double cPosition;
  double velocity;
  double triggerPoint = 10;
  int counter = 0;
  bool notDone = true;

  robot.Cata.setVelocity(CatapultSpeed, pct);
  robot.Cata.spin(fwd);

  while(counter < n + 1){
    lPosition = cPosition;
    cPosition = wrapAngleDeg(robot.catapult_rotation.position(deg));
    velocity = cPosition - lPosition;

    if(lPosition < triggerPoint && cPosition > triggerPoint && velocity > 0){
      counter ++;
    }

    wait(20, msec);
  }

  robot.Cata.setVelocity(0, pct);

}

void Robot::LaunchCatapultUntilButtonPressed(){
  robot.Cata.setVelocity(CatapultSpeed, pct);
  robot.Cata.spin(fwd);
  robot.Intake.setVelocity(-100, pct);
  robot.Cata.spin(fwd);
  waitUntil(Controller.ButtonL1.pressing());
  robot.Cata.setVelocity(0, pct);
}


int32_t WatchPort(int32_t port){
  PortWatch.push_back(vex::device(port));
  return port;
}

competition Competition;
brain Brain;
controller Controller;
Robot robot;
MatchSelector ms;
task msTask;

distanceHeadingList globalDistanceHeadingList;
std::vector<std::pair<double, std::pair<double, double>>> globalDistanceHeadingSpeedList;
std::pair<double, double> Target;
std::vector<vex::device> PortWatch;

double Distance;
double Speed;
bool Coast;
double CustomTimeout;
double SettleTime;
bool *NeverStopPtr;
bool *FalsePtr = new bool(true);
int PIDsRunning = 0;
double TurnDistance;
bool shouldLetTurnSettle = false;
vex::task PIDTask;