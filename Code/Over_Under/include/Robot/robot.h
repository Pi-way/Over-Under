#pragma once
using namespace vex;
#include "Auton/PID.h"

extern competition Competition;
extern brain Brain;
extern controller Controller;

class Robot{
private:

  bool not_done = true;

  PID catapult_PID = PID(0, 0, 0, 0, 0, 10, 360, &not_done, 0, 0); //Changing this does nothing, change in Robot::LaunchCatapult()

public:

  Robot();

  void LaunchCatapult();

  motor FR = motor(PORT1, ratio6_1, false);
  motor FL = motor(PORT4, ratio6_1, true);

  motor BR = motor(PORT2, ratio6_1, true);
  motor BL = motor(PORT5, ratio6_1, false);

  motor BBR = motor(PORT3, ratio6_1, false);
  motor BBL = motor(PORT6, ratio6_1, true);

  rotation catapult_rotation = rotation(PORT19);
  motor Cata = motor(PORT9, ratio36_1, true);

  rotation Encoder = rotation(PORT20, true);
  inertial Inertial = inertial(PORT10);

  motor LeftIntake = motor(PORT7, ratio18_1, false);
  motor RightIntake = motor(PORT8, ratio18_1, true);

  digital_out IntakeCylinder = digital_out(Brain.ThreeWirePort.A);
  digital_out WingReleaseCylinder = digital_out(Brain.ThreeWirePort.B);
  digital_out RightWingCylinder = digital_out(Brain.ThreeWirePort.C);
  digital_out LeftWingCylinder = digital_out(Brain.ThreeWirePort.D);

};

void usercontrol();
void autonomous();
void pre_auton();

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

#define Intake(MotorMember) \
  robot.LeftIntake.MotorMember \
  robot.RightIntake.MotorMember \


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

// How long do the wing release cylinders need to be activated for to close the wings?

extern double wing_release_delay;
// How long do the wing release cylinders need to be activated for to open the wings?
extern double wing_open_delay;
extern double main_wing_open_delay;

extern bool right_wing_open;
extern bool left_wing_open;

void ToggleBothWings();
void ToggleRightWing(bool no_toggle = false, bool force_toggle_open = false);
void ToggleLeftWing(bool no_toggle = false, bool force_toggle_open = false);
void TurnAt(double amount, double speed = 100, bool wait_for_completion = true, bool coast = false, double coustom_timeout = 2.0, double coustom_settle = 0.2);
void DriveWithAngle(double distance, double turn_target, double speed = 100, bool wait_for_completion = true, bool coast = false, double coustom_timeout = 4.0, double coustom_settle = 0.2);
void DriveWithAngles(distanceHeadingList List, double speed = 100, bool wait_for_completion = true, bool coast = false, double coustom_timeout = 4555.0, double coustom_settle = 0.125);