#pragma once
using namespace vex;

class Robot{
public:

    Robot();

    // motor FR = motor(PORT7, ratio18_1, false);
    // motor BR = motor(PORT10, ratio18_1, false);
    // motor BL = motor(PORT1, ratio18_1, true);
    // motor FL = motor(PORT2, ratio18_1, true);

    motor FR = motor(PORT1, ratio6_1, true);
    motor FL = motor(PORT10, ratio6_1, false);

    motor BR = motor(PORT7, ratio6_1, true);
    motor BL = motor(PORT9, ratio6_1, false);

    motor BBR = motor(PORT16, ratio18_1, true);
    motor BBL = motor(PORT8, ratio18_1, false);

};

void usercontrol();
void autonomous();
void pre_auton();

extern competition Competition;
extern brain Brain;
extern controller Controller;
extern Robot robot;
//extern Odometry odom;
extern MatchSelector ms;
extern task msTask;

#define RightDrive(MotorMember) \
  robot.FR.MotorMember \
  robot.BR.MotorMember \
  robot.BBR.MotorMember

#define LeftDrive(MotorMember) \
  robot.FL.MotorMember \
  robot.BL.MotorMember \
  robot.BBL.MotorMember

extern rotation Encoder;
extern inertial Inertial;

typedef std::vector<std::pair<double, double >> distanceHeadingList;
extern distanceHeadingList globalDistanceHeadingList;

extern double Distance;
extern double Speed;
extern bool Coast;
extern double CustomTimeout;
extern double SettleTime;
extern int PIDsRunning;
extern double TurnDistance;
extern vex::task PIDTask;


/** 
  * @brief This function allows the robot to turn using a PID.
  * @param amount How far you want the robot to turn in degrees.
  * @param speed The maximum speed for the robot to turn at (in percent).
  * @param wait_for_completion Should the function execute normally (true), or in a task (false)?
  * @param coast This sets the motors stopping to coast if true.
  * @param coustom_timeout Maximum runtime of the function.
  * @param line Variable for Debugging & Logging
  */
void Turn(double amount, double speed = 100, bool wait_for_completion = true, bool coast = false, double coustom_timeout = 2.0, double coustom_settle = 0.125);

/** 
  * @brief This function allows the robot to turn using a PID.
  * @param amount Target absolute heading.
  * @param speed The maximum speed for the robot to turn at (in percent).
  * @param wait_for_completion Should the function execute normally (true), or in a task (false)?
  * @param coast This sets the motors stopping to coast if true.
  * @param coustom_timeout Maximum runtime of the function.
  * @param line Variable for Debugging & Logging
  */
extern bool ggps;
void TurnAt(double amount, double speed = 100, bool wait_for_completion = true, bool coast = false, double coustom_timeout = 2.0, double coustom_settle = 0.25, bool gps = false);
void DriveWithAngle(double distance, double turn_target, double speed = 75, bool wait_for_completion = true, bool coast = false, double coustom_timeout = 4.0, double coustom_settle = 0.125);
void DriveWithAngles(distanceHeadingList List, double speed = 100, bool wait_for_completion = true, bool coast = false, double coustom_timeout = 4555.0, double coustom_settle = 0.25);