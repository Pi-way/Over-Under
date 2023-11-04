#include "vex.h"
using namespace vex;

Robot::Robot() {
    catapult_PID.HasRampedUp = true;
}

// How long do the wing release cylinders need to be activated for to close the wings?
double wing_release_delay = 0.175;
// How long do the wing release cylinders need to be activated for to open the wings?
double wing_open_delay = 0.25;
double main_wing_open_delay = 0.3;

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
    Cata.setVelocity(50, pct);
    while (wrapAngleDeg(catapult_rotation.angle(deg)) < 20) {
        wait(20, msec);
    }

    not_done = true;
    catapult_PID = PID(4, 2, 0.2, 500, 10, 360, 100, &not_done, 10, 0);

    wait(0.45, sec);

    // Reload Catapult
    while (not_done || Controller.ButtonL1.pressing()) {

        last_time = this_time;
        this_time = Brain.Timer.value();
        delta_time = this_time - last_time;

        Cata_error = wrapAngleDeg(catapult_rotation.angle(deg));
        Cata_speed = catapult_PID.Update(Cata_error, delta_time);

        if(Controller.ButtonL1.pressing()) {
            Cata_speed = 85;
        }

        Cata.setVelocity(Cata_speed, pct);
        wait(20, msec);

        Brain.Screen.clearScreen();
        Brain.Screen.setCursor(1,1);
        Brain.Screen.print(Cata_error);

    }

    Cata.stop();
}

void Robot::LaunchC() {
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
    Cata.setVelocity(50, pct);
    while (wrapAngleDeg(catapult_rotation.angle(deg)) < 20) {
        wait(20, msec);
    }

    not_done = true;
    catapult_PID = PID(4, 2, 0.2, 500, 10, 360, 100, &not_done, 10, 0);

    wait(0.45, sec);

    // Reload Catapult
    while (not_done || Controller.ButtonL1.pressing()) {

        last_time = this_time;
        this_time = Brain.Timer.value();
        delta_time = this_time - last_time;

        Cata_error = wrapAngleDeg(catapult_rotation.angle(deg) - 10);
        Cata_speed = catapult_PID.Update(Cata_error, delta_time);

        if(Controller.ButtonL1.pressing()) {
            Cata_speed = 85;
        }

        Cata.setVelocity(Cata_speed, pct);
        wait(20, msec);

        Brain.Screen.clearScreen();
        Brain.Screen.setCursor(1,1);
        Brain.Screen.print(Cata_error);

    }

    Cata.stop();
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

    robot.Cata.setVelocity(10, pct);

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
//Odometry odom;
MatchSelector ms;
task msTask;

distanceHeadingList globalDistanceHeadingList;
std::vector<std::pair<double, std::pair<double, double>>> globalDistanceHeadingSpeedList;

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