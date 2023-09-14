#include "vex.h"
using namespace vex;

Robot::Robot() {
    catapult_PID.HasRampedUp = true;
}

bool isAngleBetween(double angle, double startAngle, double endAngle) {
    // Normalize the angles to be within [0, 360) degrees
    angle = fmod(angle, 360.0);
    if (angle < 0) {
        angle += 360.0;
    }
    startAngle = fmod(startAngle, 360.0);
    if (startAngle < 0) {
        startAngle += 360.0;
    }
    endAngle = fmod(endAngle, 360.0);
    if (endAngle < 0) {
        endAngle += 360.0;
    }

    if (startAngle <= endAngle) {
        // If the range doesn't wrap around, simply check if angle is between startAngle and endAngle.
        return angle >= startAngle && angle <= endAngle;
    } else {
        // If the range wraps around, check if angle is outside the range.
        return angle >= startAngle || angle <= endAngle;
    }
}

void Robot::LaunchCatapult() {

    // Initialize variables for measuring time
    
    double this_time = Brain.Timer.value();
    double last_time = this_time;
    double delta_time = this_time - last_time;

    // Initialize variables for catapult control

    double cata_speed = 0;
    double cata_error = 0;
    Cata.spin(forward);
    Cata.setBrake(hold);
    double cata_pre_load_angle;
    not_done = true;
    catapult_PID = PID(4, 2, 10, 500, 10, 100, &not_done, 10, 0);

    // Check which set of teeth are currently in use

    if (isAngleBetween(catapult_rotation.angle(deg), cata_first_pre_load_angle, cata_first_pre_launch_angle + 10)) {
        // First set of teeth
        cata_pre_load_angle = cata_second_pre_load_angle;
    } else {
        // Second set of teeth
        cata_pre_load_angle = cata_first_pre_load_angle;
    }

    // Launch Catapult

    Cata.setVelocity(100, pct);
    while (wrapAngleDeg(cata_pre_load_angle - catapult_rotation.angle(deg)) > 0) {
        wait(20, msec);
    }

    // Reload Catapult

    while (not_done) {

        last_time = this_time;
        this_time = Brain.Timer.value();
        delta_time = this_time - last_time;

        // Check if the catapult is in the first or second set of slipping teeth

        if (isAngleBetween(catapult_rotation.angle(deg), cata_first_pre_load_angle, cata_first_pre_launch_angle + 10)) {
            // First set of teeth
            cata_error = wrapAngleDeg(cata_first_pre_launch_angle - catapult_rotation.angle(deg));
        } else {
            // Second set of teeth
            cata_error = wrapAngleDeg(cata_second_pre_launch_angle - catapult_rotation.angle(deg));
        }

        cata_speed = catapult_PID.Update(cata_error, delta_time);

        Brain.Screen.clearScreen();
        Brain.Screen.setCursor(1,1);
        Brain.Screen.print(cata_error);

        Brain.Screen.setCursor(2, 1);
        if(isAngleBetween(catapult_rotation.angle(deg), cata_first_pre_load_angle, cata_first_pre_launch_angle + 10)) {
            Brain.Screen.print("first teeth");
            Brain.Screen.newLine();
            Brain.Screen.print(cata_first_pre_launch_angle);
        } else {
            Brain.Screen.print("second teeth");
            Brain.Screen.newLine();
            Brain.Screen.print(cata_second_pre_launch_angle);
        }

        Brain.Screen.newLine();
        Brain.Screen.print(catapult_rotation.angle(deg));

        Cata.setVelocity(cata_speed, pct);
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

rotation Encoder(PORT8, true);
inertial Inertial(PORT9);
motor Cata(PORT7, ratio36_1, false);

distanceHeadingList globalDistanceHeadingList;

double Distance;
double Speed;
bool Coast;
double CustomTimeout;
double SettleTime;
int PIDsRunning = 0;
double TurnDistance;
vex::task PIDTask;