#pragma once
using namespace vex;
#include "Auton/PID.h"

extern competition Competition;
extern brain Brain;
extern controller Controller;

extern bool letrun;

class Robot{
private:

  bool not_done = true;

  PID catapult_PID = PID(0, 0, 0, 0, 0, 10, 360, &not_done, 0, 0); //Changing this does nothing, change in Robot::LaunchCatapult()

public:

  Robot();

  void LaunchCatapult();
  void LaunchCatapultButNot();
  void LaunchCatapultFor(int amount);

  motor FR = motor(PORT6, ratio6_1, false);
  motor FL = motor(PORT3, ratio6_1, true);

  motor BR = motor(PORT5, ratio6_1, true);
  motor BL = motor(PORT2, ratio6_1, false);

  motor BBR = motor(PORT4, ratio6_1, false);
  motor BBL = motor(PORT1, ratio6_1, true);

  rotation catapult_rotation = rotation(PORT8, true);
  motor Cata = motor(PORT7, ratio18_1, false);

  TrackingWheel ForwardTrack = TrackingWheel(rotation(PORT12), 2.75, false);
  TrackingWheel StrafeTrack = TrackingWheel(rotation(PORT11), 2.75, false);
  inertial Inertial = inertial(PORT13);

  motor Intake = motor(PORT10, ratio6_1, true);

  digital_out RightLift = digital_out(Brain.ThreeWirePort.C);
  digital_out LeftLift = digital_out(Brain.ThreeWirePort.D);
  digital_out RightWing = digital_out(Brain.ThreeWirePort.A);
  digital_out LeftWing = digital_out(Brain.ThreeWirePort.B);
  digital_out SideElevation = digital_out(Brain.ThreeWirePort.H);

};

class CatapultObserver{
public:
  enum  catapult_state{
    STARTED_LOADING,
    LOADING,
    STARTED_LAUNCHING,
    LAUNCHING,
  };

  const double maximum_catapult_position = 80.33;
  double last_catapult_position;

  double time;
  double last_time;

  catapult_state last_state;
  catapult_state current_state;

  std::pair<catapult_state, double> GetCatapultState();
};

void usercontrol();
void autonomous();
void pre_auton();

extern Robot robot;
extern Odometry odom;
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

typedef std::vector<std::pair<double, double >> distanceHeadingList;
extern distanceHeadingList globalDistanceHeadingList;
extern std::vector<std::pair<double, std::pair<double, double>>> globalDistanceHeadingSpeedList;
extern std::pair<double, double> Target;
extern bool UseFront;

extern double Distance;
extern double Speed;
extern bool Coast;
extern double CustomTimeout;
extern double SettleTime;
extern bool *FalsePtr;
extern bool *NeverStopPtr;
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

void ToggleLift();
void ToggleBothWings();
void ToggleRightWing();
void ToggleLeftWing();
void TurnAt(double amount, double speed = 100, bool wait_for_completion = true, bool coast = false, double coustom_timeout = 2.0, double coustom_settle = 0.2);
void TurnAtPoint(std::pair<double, double> target, bool use_front = true, double speed = 100, bool wait_for_completion = true, bool coast = false, double coustom_timeout = 2.0, double coustom_settle = 0.2);
void DriveWithAngle(double distance, double turn_target, double speed = 100, bool wait_for_completion = true, bool coast = false, double coustom_timeout = 4.0, double coustom_settle = 0.2, bool *never_stop_ptr = FalsePtr);
void DriveWithAngles(distanceHeadingList List, double speed = 100, bool wait_for_completion = true, bool coast = false, double coustom_timeout = 4555.0, double coustom_settle = 0.125);
void DriveWithAnglesAndSpeed(std::vector<std::pair<double, std::pair<double, double>>> List, double speed = 100, bool wait_for_completion = true, bool coast = false, double coustom_timeout = 4555.0, double coustom_settle = 0.125);
