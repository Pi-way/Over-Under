#include "vex.h"
using namespace vex;

bool letrun = true;

Robot::Robot() {
    catapult_PID.HasRampedUp = true;
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

void ToggleLift(){
  robot.RightLift.set(!robot.RightLift.value());
  robot.LeftLift.set(!robot.LeftLift.value());
}

void Robot::LaunchCatapultButNot() {

    // Initialize variables for measuring time
    double this_time = Brain.Timer.value();
    double last_time = this_time;
    double delta_time = this_time - last_time;

    // Initialize variables for Catapult control
    double Cata_speed = 0;
    double Cata_error = 0;
    Cata.spin(forward);
    Cata.setBrake(hold);

    // Launch Catapult
    Cata.setVelocity(100, pct);
    while (wrapAngleDeg(catapult_rotation.angle(deg) - 20) < 20 || wrapAngleDeg(catapult_rotation.angle(deg) - 20) > 80) {
        wait(20, msec);
    }

    not_done = true;
    catapult_PID = PID(2.25, 0.1, 0.07, 500, 10, 360, 100, &not_done, 10, 0);

    letrun = true;

    // Reload Catapult
    while ((not_done || Controller.ButtonL1.pressing()) && letrun) {

        last_time = this_time;
        this_time = Brain.Timer.value();
        delta_time = this_time - last_time;

        Cata_error = wrapAngleDeg(catapult_rotation.angle(deg) - 29);
        Cata_speed = catapult_PID.Update(Cata_error, delta_time);

        if(Controller.ButtonL1.pressing()) {
            Cata_speed = 100;
        }

        Cata.setVelocity(Cata_speed, pct);
        wait(20, msec);
    }
    letrun = true;
    Cata.stop();
}

vex::task tiskTisk;
void Robot::LaunchCatapult() {

    // Initialize variables for measuring time
    double this_time = Brain.Timer.value();
    double last_time = this_time;
    double delta_time = this_time - last_time;

    // Initialize variables for Catapult control
    double Cata_speed = 0;
    double Cata_error = 0;
    Cata.spin(forward);
    Cata.setBrake(hold);

    // Launch Catapult
    Cata.setVelocity(100, pct);
    while (wrapAngleDeg(catapult_rotation.angle(deg)) < 20 || wrapAngleDeg(catapult_rotation.angle(deg)) > 80) {
        wait(20, msec);
    }

    not_done = true;
    catapult_PID = PID(2.25, 0.1, 0.07, 500, 10, 360, 100, &not_done, 10, 0);

    letrun = true;

    // Reload Catapult
    while ((not_done || Controller.ButtonL1.pressing()) && letrun) {

        last_time = this_time;
        this_time = Brain.Timer.value();
        delta_time = this_time - last_time;

        Cata_error = wrapAngleDeg(catapult_rotation.angle(deg));
        Cata_speed = catapult_PID.Update(Cata_error, delta_time);

        if(Controller.ButtonL1.pressing()) {
          Cata_speed = 100;
          catapult_PID.HasReachedEnd = false;
          not_done = true;
        }

        Cata.setVelocity(Cata_speed, pct);
        wait(20, msec);
    }
    letrun = true;
    Cata.stop();

    wait(0.25, sec);
    if(wrapAngleDeg(robot.catapult_rotation.angle(deg)) > 5){
      letrun = false;
      robot.LaunchCatapult();
    }
}


std::pair<CatapultObserver::catapult_state, double>
CatapultObserver::GetCatapultState() {

  //time
  double this_time = Brain.Timer.value();
  double delta_time = this_time - last_time;
  time += delta_time;
  last_time = this_time;

  //position
  double current_catapult_position = robot.catapult_rotation.position(deg);
  double delta_catapult_position = current_catapult_position - last_catapult_position;
  last_catapult_position = current_catapult_position;

  //check if catapult is being loaded
  if(delta_catapult_position < 0 && (current_catapult_position < maximum_catapult_position - 5 || current_catapult_position > 1)) {
    current_state = LOADING;
      //check if this is the state that has switched
    if(last_state == LAUNCHING) {
      time = 0;
      current_state = STARTED_LOADING;
    }
  }

  //check if catapult is being launched
  if(delta_catapult_position > 0 && (current_catapult_position < maximum_catapult_position - 5 || current_catapult_position > 1)) {
    current_state = LAUNCHING;
    //check if this is the state that has switched
    if(last_state == LOADING) {
      time = 0;
      current_state = STARTED_LAUNCHING;
    }
  }

  last_state = current_state;

  Brain.Screen.clearScreen();
  Brain.Screen.setCursor(1,1);
  Brain.Screen.print((int)current_state);
  Brain.Screen.setCursor(2, 1);
  Brain.Screen.print(time);

  //exception state
  return {current_state, time};
}

void Robot::LaunchCatapultFor(int amount){
  
  //create observer and start observing
  CatapultObserver observer;
  observer.GetCatapultState();
  std::pair<CatapultObserver::catapult_state, double> state;

  int launches = 0;

  robot.Cata.spin(fwd);

  double return_to_speed_delay = .125;

  robot.Cata.setVelocity(100, pct);

  while (true) {

    waitUntil(robot.catapult_rotation.angle(deg) < 5);

    wait(return_to_speed_delay, sec);

    robot.Cata.setVelocity(100, pct);

    launches += 1;

    if(launches > amount - 1){
      break;
    }

    waitUntil(robot.catapult_rotation.angle(deg) > 20);

    wait(20, msec);

  }

  Cata.setVelocity(0, pct);

}

competition Competition;
brain Brain;
controller Controller;
Robot robot;
PurePursuitRobot PureRobot;
Odometry odom;
MatchSelector ms;
task msTask;
bool UseFront;

distanceHeadingList globalDistanceHeadingList;
std::vector<std::pair<double, std::pair<double, double>>> globalDistanceHeadingSpeedList;
std::pair<double, double> Target;

double Distance;
double Speed;
bool Coast;
double CustomTimeout;
double SettleTime;
bool *NeverStopPtr;
bool *FalsePtr = new bool(true);
int PIDsRunning = 0;
double TurnDistance;
vex::task PIDTask;