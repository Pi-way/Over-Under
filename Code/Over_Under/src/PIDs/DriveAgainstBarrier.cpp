/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       Drive.cpp                                                 */
/*    Author:       Team 98548J (Ace)                                         */
/*    Created:      7-14-2022                                                 */
/*    Description:  Source code file for Drive function                       */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "vex.h"

int _Drive_Against_Barrier_()
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
    task::yield();
  }

  bool NotDone = true;
  bool TurnNotDone = true;
  
  PID LocalPID(14.75*0.5, 0.5, 0.1, 200, 25, 4, LocalSpeed, &NotDone, LocalTimeout, LocalSettle);
  PID LocalTurnPID(1.3 * 0.5, 0.001, 0.01, 200, 10, 6, 100, &TurnNotDone, 1000000, 1000000);

  RightDrive(setPosition(0, deg);)
  LeftDrive(setPosition(0, deg);)

  RightDrive(setStopping((LocalCoast) ? coast : brake);)
  LeftDrive(setStopping((LocalCoast) ? coast : brake);)

  auto Encoder = robot.ForwardTrack.getObserver();

  double Error = LocalDistance - Encoder->position();
  double TurnError = wrapAngleDeg(LocalTurnDistance - robot.Inertial.heading(degrees));
  double OutputSpeed = 0;
  double TurnCorrectionSpeed = 0;

  double ThisTime = Brain.Timer.systemHighResolution();
  double LastTime = ThisTime;
  
  RightDrive(spin(forward);)
  LeftDrive(spin(forward);)

  while (NotDone && *NeverStopPtr)
  {
    LastTime = ThisTime;
    ThisTime = Brain.Timer.systemHighResolution();

    Error = LocalDistance - Encoder->position();
    TurnError = wrapAngleDeg(LocalTurnDistance - robot.Inertial.heading(degrees));

    OutputSpeed = LocalPID.Update(Error, (ThisTime - LastTime)/1000000.0);
    TurnCorrectionSpeed = LocalTurnPID.Update(TurnError, (ThisTime - LastTime)/1000000.0);

    RightDrive(setVelocity(OutputSpeed - TurnCorrectionSpeed, pct);)
    LeftDrive(setVelocity(OutputSpeed + TurnCorrectionSpeed, pct);)

    wait(20, msec);
  }

  delete Encoder;

  RightDrive(setVelocity(0, pct);)
  LeftDrive(setVelocity(0, pct);)

  PIDsRunning -= 1;

  return 0;

}

// Wrapper function that will accept arguments for the main function (_Drive_())
void DriveAgainstBarrier(double distance, double turn_target, double speed, bool wait_for_completion, bool coast, double coustom_timeout, double coustom_settle, bool *never_stop_ptr)
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
    _Drive_Against_Barrier_();
  }
  else
  {
    PIDTask = task(_Drive_Against_Barrier_);
  }
}