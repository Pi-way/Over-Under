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
  double CorrectionCoeficient = 1;
    // Assign and declare local variables from global variables.
  double LocalDistance = ((Distance/(2.75*3.1415))*(1))*360.0;
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
  
  PID LocalPID((0.375) * 0.5, 0.01, 0.07, 175, 25, LocalSpeed, &NotDone, LocalTimeout, LocalSettle);
  PID LocalTurnPID((1.8) * 0.5, 0.05, 1.125, 100, 100, LocalSpeed, &TurnNotDone, 1000000000, 1000000000);

  RightDrive(setPosition(0, deg);)
  LeftDrive(setPosition(0, deg);)

  RightDrive(setStopping((LocalCoast) ? coast : brake);)
  LeftDrive(setStopping((LocalCoast) ? coast : brake);)

  robot.Encoder.setPosition(0, deg);
  double Error = LocalDistance - robot.Encoder.position(deg);
  double TurnError = wrapAngleDeg(LocalTurnDistance - robot.Inertial.heading(degrees));
  double OutputSpeed = 0;
  double TurnCorrectionSpeed = 0;

  double ThisTime = Brain.Timer.systemHighResolution();
  double LastTime = ThisTime;
  
  RightDrive(spin(forward);)
  LeftDrive(spin(forward);)

  while (true)
  {
    LastTime = ThisTime;
    ThisTime = Brain.Timer.systemHighResolution();

    Error = LocalDistance - robot.Encoder.position(deg);
    TurnError = wrapAngleDeg(LocalTurnDistance - robot.Inertial.heading(degrees));

    OutputSpeed = LocalPID.Update(Error, (ThisTime - LastTime)/1000000);
    TurnCorrectionSpeed = LocalTurnPID.Update(TurnError, (ThisTime - LastTime)/1000000);

    RightDrive(setVelocity(OutputSpeed - TurnCorrectionSpeed, pct);)
    LeftDrive(setVelocity(OutputSpeed + TurnCorrectionSpeed, pct);)

    wait(20, msec);

    printf("\x1B[2J\x1B[H");
    printf("%f\n", Error);
  }

  RightDrive(setVelocity(0, pct);)
  LeftDrive(setVelocity(0, pct);)

  PIDsRunning -= 1;

  return 0;

}

// Wrapper function that will accept arguments for the main function (_Drive_())
void DriveWithAngle(double distance, double turn_target, double speed, bool wait_for_completion, bool coast, double coustom_timeout, double coustom_settle)
{

  // Assign local variables to global variables
  Distance = distance;
  TurnDistance = turn_target;
  Speed = speed;
  Coast = coast;
  CustomTimeout = coustom_timeout;
  SettleTime = coustom_settle;

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