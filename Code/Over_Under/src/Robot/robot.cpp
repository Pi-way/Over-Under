#include "vex.h"
using namespace vex;

Robot::Robot() {
    
}

competition Competition;
brain Brain;
controller Controller;
Robot robot;
//Odometry odom;
MatchSelector ms;
task msTask;

rotation Encoder(PORT3, true);
inertial Inertial(PORT4);

distanceHeadingList globalDistanceHeadingList;

double Distance;
double Speed;
bool Coast;
double CustomTimeout;
double SettleTime;
int PIDsRunning;
double TurnDistance;
vex::task PIDTask;