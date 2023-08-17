/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       TurnAt.cpp                                                */
/*    Author:       Team 98548J (Ace)                                         */
/*    Created:      12-15-22                                                  */
/*    Description:  Source file for Turn function                             */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include <vex.h>

int _Turn_At_()
{
    // Assign and declare local variables from global variables.
  double LocalDistance = TurnDistance;

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
  
  PID LocalPID(0.475, 0.0625, 0.125, 200, 20, LocalSpeed, &NotDone, LocalTimeout, LocalSettle);

  RightDrive(setStopping((LocalCoast) ? coast : brake);)
  LeftDrive(setStopping((LocalCoast) ? coast : brake);)

  double Error = wrapAngleDeg(LocalDistance - Inertial.heading(degrees));
  double OutputSpeed = 0;

  double ThisTime = Brain.Timer.systemHighResolution();
  double LastTime = ThisTime;
  
  while (NotDone)
  {
    LastTime = ThisTime;
    ThisTime = Brain.Timer.systemHighResolution();
    OutputSpeed = LocalPID.Update(Error, (ThisTime - LastTime)/1000000);

    RightDrive(setVelocity(-OutputSpeed, pct);)
    LeftDrive(setVelocity(OutputSpeed, pct);)

    vex::task::yield();

    Error = wrapAngleDeg(LocalDistance - Inertial.heading(degrees));

  }

  RightDrive(setVelocity(0, pct);)
  LeftDrive(setVelocity(0, pct);)

  PIDsRunning -= 1;

  return 0;

}

// Wrapper function that will accept arguments for the main function (_Drive_())
void TurnAt(double turn_distance, double speed, bool wait_for_completion, bool coast, double coustom_timeout, double coustom_settle)
{

  // Assign local variables to global variables
  TurnDistance = turn_distance;
  Speed = speed;
  Coast = coast;
  CustomTimeout = coustom_timeout;
  SettleTime = coustom_settle;

  // Either wait for the function to complete, or run the function in a task
  if (wait_for_completion)
  {
    _Turn_At_();
  }
  else
  {
    PIDTask = task(_Turn_At_);
  }
}