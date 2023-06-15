#pragma once
using namespace vex;

class Robot{
public:

    Robot();

    motor FR = motor(PORT7, ratio18_1, false);
    motor BR = motor(PORT10, ratio18_1, false);
    motor BL = motor(PORT1, ratio18_1, true);
    motor FL = motor(PORT2, ratio18_1, true);

};

extern competition Competition;
extern brain Brain;
extern controller Controller;
extern Robot robot;
extern Odometry odom;