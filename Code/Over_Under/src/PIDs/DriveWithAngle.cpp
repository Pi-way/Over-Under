/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       Drive.cpp                                                 */
/*    Author:       Team 98548J (Ace)                                         */
/*    Created:      7-14-2022                                                 */
/*    Description:  Source code file for Drive function                       */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "vex.h"

int _Drive_With_Angle_()
{
    // Assign and declare local variables from global variables.
  double LocalDistance = Distance;
  double LocalTurnDistance = TurnDistance;
  double LocalSpeed = Speed;
  bool LocalCoast = Coast;
  double LocalTimeout = CustomTimeout;
  double LocalSettle = SettleTime;

  // Update PIDsRunning
  PIDsRunning ++;

  // Wait until other PIDs have completed
  while(PIDsRunning > 1){
    wait(20, msec);
  }

  bool NotDone = true;
  bool TurnNotDone = true;

  PID LocalPID(0.85, 0.1, 0.06, 50, 2, 3, LocalSpeed/100.0*12.0, &NotDone, LocalTimeout, LocalSettle);
  PID LocalTurnPID(0.2, 0.1, 0.015, 50, 5, 6, 12, &TurnNotDone, 100000, 0.125);

  RightDrive(setPosition(0, deg);)
  LeftDrive(setPosition(0, deg);)

  RightDrive(setStopping((LocalCoast) ? coast : brake);)
  LeftDrive(setStopping((LocalCoast) ? coast : brake);)

  auto Encoder = robot.ForwardTrack.getObserver();

  double Error = LocalDistance - Encoder->position();
  double TurnError = wrapAngleDeg(LocalTurnDistance - robot.Inertial.heading(degrees));
  double OutputSpeed = 0;
  double TurnCorrectionSpeed = 0;

  double RequestedRight;
  double RequestedLeft;

  double ThisTime = Brain.Timer.systemHighResolution();
  double LastTime = ThisTime;
  
  RightDrive(spin(forward);)
  LeftDrive(spin(forward);)

  double startTime = Brain.Timer.value();

  while (NotDone && Brain.Timer.value() - startTime < LocalTimeout)
  {
    LastTime = ThisTime;
    ThisTime = Brain.Timer.systemHighResolution();

    Error = LocalDistance - Encoder->position();
    TurnError = wrapAngleDeg(LocalTurnDistance - robot.Inertial.heading(degrees));

    OutputSpeed = LocalPID.Update(Error, (ThisTime - LastTime)/1000000.0);
    TurnCorrectionSpeed = LocalTurnPID.Update(TurnError, (ThisTime - LastTime)/1000000.0);

    RequestedRight = (OutputSpeed - TurnCorrectionSpeed);
    RequestedLeft = (OutputSpeed + TurnCorrectionSpeed);

    RightDrive(spin(fwd, RequestedRight + (0.75 * GetSign(RequestedRight)), voltageUnits::volt);)
    LeftDrive(spin(fwd, RequestedLeft, voltageUnits::volt);)

    wait(20, msec);
  }

  delete Encoder;

  RightDrive(stop();)
  LeftDrive(stop();)

  PIDsRunning -= 1;

  return 0;

}

// Wrapper function that will accept arguments for the main function (_Drive_())
void DriveWithAngle(double distance, double turn_target, double speed, bool wait_for_completion, bool coast, double coustom_timeout, double coustom_settle, bool *never_stop_ptr)
{

  // Assign local variables to global variables
  Distance = distance;
  TurnDistance = turn_target;
  Speed = speed;
  Coast = coast;
  CustomTimeout = coustom_timeout;
  SettleTime = coustom_settle;
  NeverStopPtr = never_stop_ptr;

  // Either wait for the function to complete, or run the function in a task
  if (wait_for_completion)
  {
    _Drive_With_Angle_();
  }
  else
  {
    PIDTask = task(_Drive_With_Angle_);
  }
}