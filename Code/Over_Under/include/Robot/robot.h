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
public:

  Robot();

  void LaunchCatapult();
  void LaunchCatapultButNot();
  void LaunchCatapultFor(int n);
  void LaunchCatapultUntilButtonPressed();

  motor FR = motor(WatchPort(PORT1), ratio6_1, false);
  motor FL = motor(WatchPort(PORT5), ratio6_1, true);

  motor BR = motor(WatchPort(PORT2), ratio6_1, true);
  motor BL = motor(WatchPort(PORT6), ratio6_1, false);

  motor BBR = motor(WatchPort(PORT3), ratio6_1, false);
  motor BBL = motor(WatchPort(PORT7), ratio6_1, true);

  rotation catapult_rotation = rotation(WatchPort(PORT13), false);
  motor Cata = motor(WatchPort(PORT14), ratio18_1, false);

  TrackingWheel ForwardTrack = TrackingWheel(rotation(WatchPort(PORT17), true), 2.75, false);
  inertial Inertial = inertial(WatchPort(PORT9));

  motor Intake = motor(WatchPort(PORT8), ratio18_1, true);

  digital_out RightWing = digital_out(Brain.ThreeWirePort.D);
  digital_out LeftWing = digital_out(Brain.ThreeWirePort.E);
  digital_out RightBackWing = digital_out(Brain.ThreeWirePort.F);
  digital_out LeftBackWing = digital_out(Brain.ThreeWirePort.G);

  digital_out LiftRatchet = digital_out(Brain.ThreeWirePort.B);
  inertial LiftInertial = inertial(WatchPort(PORT12));
  motor BigElevate = motor(WatchPort(PORT11), ratio36_1, false);
  motor SmallElevate = motor(WatchPort(PORT18), ratio18_1, false);

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
void ToggleBothBackWings();
void ToggleRightBackWing();
void ToggleLeftBackWing();

void TurnAt(double amount, double speed = 100, bool wait_for_completion = true, bool coast = false, double coustom_timeout = 1.0, double coustom_settle = 0.2, bool start_at_100 = false);
void DriveWithAngle(double distance, double turn_target, double speed = 100, bool wait_for_completion = true, bool coast = false, double coustom_timeout = 4.0, double coustom_settle = 0.2, bool *never_stop_ptr = FalsePtr);
void DriveWithAngles(distanceHeadingList List, double speed = 100, bool wait_for_completion = true, bool coast = false, double coustom_timeout = 4555.0, double coustom_settle = 0.125);
void DriveWithAnglesAndSpeed(std::vector<std::pair<double, std::pair<double, double>>> List, double speed = 100, bool wait_for_completion = true, bool coast = false, double coustom_timeout = 4555.0, double coustom_settle = 0.125);
void DriveWithAnglesAndSpeed(bool shouldLetTurnSettle, std::vector<std::pair<double, std::pair<double, double>>> List, double speed = 100, bool wait_for_completion = true, bool coast = false, double coustom_timeout = 4555.0, double coustom_settle = 0.125);