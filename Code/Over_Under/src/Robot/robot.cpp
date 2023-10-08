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
    Cata.setVelocity(75, pct);
    while (wrapAngleDeg(catapult_rotation.angle(deg)) < 20) {
        wait(20, msec);
    }

    not_done = true;
    catapult_PID = PID(4, 2, 10, 500, 10, 75, 360, &not_done, 10, 0);

    // Reload Catapult
    while (not_done) {

        last_time = this_time;
        this_time = Brain.Timer.value();
        delta_time = this_time - last_time;

        Cata_error = wrapAngleDeg(catapult_rotation.angle(deg));
        Cata_speed = catapult_PID.Update(Cata_error, delta_time);

        if(Controller.ButtonL1.pressing()) {
            Cata_speed = 75;
        }

        Cata.setVelocity(Cata_speed, pct);
        wait(20, msec);
    }

    Cata.stop();
}

competition Competition;
brain Brain;
controller Controller;
Robot robot;
//Odometry odom;
MatchSelector ms;
task msTask;

distanceHeadingList globalDistanceHeadingList;

double Distance;
double Speed;
bool Coast;
double CustomTimeout;
double SettleTime;
int PIDsRunning = 0;
double TurnDistance;
vex::task PIDTask;