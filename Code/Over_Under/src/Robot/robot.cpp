#include "vex.h"
using namespace vex;

Robot::Robot() {
    catapult_PID.HasRampedUp = true;
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
    catapult_PID = PID(4, 2, 10, 500, 10, 75, &not_done, 10, 0);

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