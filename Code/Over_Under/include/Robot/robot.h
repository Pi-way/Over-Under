#pragma once
using namespace vex;

extern controller Controller;
extern brain Brain;
extern rotation enncoder;

class Robot{
public:

    Robot();

    motor FR = motor(PORT9, ratio18_1, false);
    motor BR = motor(PORT10, ratio18_1, false);
    motor BL = motor(PORT1, ratio18_1, true);
    motor FL = motor(PORT2, ratio18_1, true);

    Odometry odometry = Odometry();

    void update_odometry();
};