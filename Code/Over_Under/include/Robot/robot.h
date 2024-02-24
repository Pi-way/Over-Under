#pragma once
using namespace vex;
#include "Auton/PID.h"

extern competition Competition;
extern brain Brain;
extern controller Controller;

extern bool letrun;

extern std::vector<vex::device> PortWatch;

int32_t WatchPort(int32_t port);

class Robot{
private:

  bool not_done = true;

  PID catapult_PID = PID(0, 0, 0, 0, 0, 10, 360, &not_done, 0, 0); //Changing this does nothing, change in Robot::LaunchCatapult()

public:

  Robot();

  void LaunchCatapult();
  void LaunchCatapultButNot();
  void LaunchCatapultFor(int n);
  void LaunchCatapultUntilButtonPressed();

  motor FR = motor(WatchPort(PORT6), ratio6_1, false);
  motor FL = motor(WatchPort(PORT3), ratio6_1, true);

  motor BR = motor(WatchPort(PORT5), ratio6_1, true);
  motor BL = motor(WatchPort(PORT2), ratio6_1, false);

  motor BBR = motor(WatchPort(PORT4), ratio6_1, false);
  motor BBL = motor(WatchPort(PORT1), ratio6_1, true);

  rotation catapult_rotation = rotation(WatchPort(PORT8), false);
  motor Cata = motor(WatchPort(PORT7), ratio18_1, false);

  TrackingWheel ForwardTrack = TrackingWheel(rotation(WatchPort(PORT12)), 2.75, false);
  TrackingWheel StrafeTrack = TrackingWheel(rotation(WatchPort(PORT11)), 2.75, false);
  inertial Inertial = inertial(WatchPort(PORT13));

  motor Intake = motor(WatchPort(PORT10), ratio6_1, true);

  distance FrontDistance = distance(WatchPort(PORT15));
  distance BackDistance = distance(WatchPort(PORT14));

  digital_out RightWing = digital_out(Brain.ThreeWirePort.A);
  digital_out LeftWing = digital_out(Brain.ThreeWirePort.B);
  digital_out SideElevation = digital_out(Brain.ThreeWirePort.E);
  digital_out MiniWing = digital_out(Brain.ThreeWirePort.D);

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

extern double CatapultSpeed;
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
extern bool shouldLetTurnSettle;

extern PID TurnPID;

// How long do the wing release cylinders need to be activated for to close the wings?

extern double wing_release_delay;
// How long do the wing release cylinders need to be activated for to open the wings?
extern double wing_open_delay;
extern double main_wing_open_delay;

extern bool right_wing_open;
extern bool left_wing_open;

void ToggleBothWings();
void ToggleRightWing();
void ToggleLeftWing();
void TurnAt(double amount, double speed = 100, bool wait_for_completion = true, bool coast = false, double coustom_timeout = 2.0, double coustom_settle = 0.2, bool start_at_100 = false);
void TurnAtPoint(std::pair<double, double> target, bool use_front = true, double speed = 100, bool wait_for_completion = true, bool coast = false, double coustom_timeout = 2.0, double coustom_settle = 0.2);
void DriveWithAngle(double distance, double turn_target, double speed = 100, bool wait_for_completion = true, bool coast = false, double coustom_timeout = 4.0, double coustom_settle = 0.2, bool *never_stop_ptr = FalsePtr);
void DriveWithAngles(distanceHeadingList List, double speed = 100, bool wait_for_completion = true, bool coast = false, double coustom_timeout = 4555.0, double coustom_settle = 0.125);
void DriveWithAnglesAndSpeed(std::vector<std::pair<double, std::pair<double, double>>> List, double speed = 100, bool wait_for_completion = true, bool coast = false, double coustom_timeout = 4555.0, double coustom_settle = 0.125);
void DriveWithAnglesAndSpeed(bool shouldLetTurnSettle, std::vector<std::pair<double, std::pair<double, double>>> List, double speed = 100, bool wait_for_completion = true, bool coast = false, double coustom_timeout = 4555.0, double coustom_settle = 0.125);
void DriveAgainstBarrier(double distance, double turn_target, double speed = 100, bool wait_for_completion = true, bool coast = false, double coustom_timeout = 4.0, double coustom_settle = 0.2, bool *never_stop_ptr = FalsePtr);