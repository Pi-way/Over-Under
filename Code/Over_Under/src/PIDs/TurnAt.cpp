/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       TurnAt.cpp                                                */
/*    Author:       Team 98548J (Ace)                                         */
/*    Created:      12-15-22                                                  */
/*    Description:  Source file for Turn function                             */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include <vex.h>

PID TurnPID(1.3 * 0.5, 0.001, 0.01, 200, 10, 6, 0, nullptr, 0, 0);
bool sa100 = false;

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

  TurnPID = PID(1.3 * 0.5, 0.001, 0.01, 200, 10, 6, LocalSpeed, &NotDone, LocalTimeout, LocalSettle);

  RightDrive(setStopping((LocalCoast) ? coast : brake);)
  LeftDrive(setStopping((LocalCoast) ? coast : brake);)

  double Error = wrapAngleDeg(LocalDistance - robot.Inertial.heading(degrees));
  double OutputSpeed = 0;

  double ThisTime = Brain.Timer.systemHighResolution();
  double LastTime = ThisTime;

  RightDrive(spin(forward);)
  LeftDrive(spin(forward);)

  if(sa100){
    TurnPID.HasRampedUp = true;
    TurnPID.RampUp = 1000000;
  }
  
  while (NotDone)
  {
    LastTime = ThisTime;
    ThisTime = Brain.Timer.systemHighResolution();
    OutputSpeed = TurnPID.Update(Error, (ThisTime - LastTime)/1000000.0);

    RightDrive(setVelocity(-OutputSpeed, pct);)
    LeftDrive(setVelocity(OutputSpeed, pct);)

    wait(50, msec);

    Error = wrapAngleDeg(LocalDistance - robot.Inertial.heading(degrees));

  }

  RightDrive(setVelocity(0, pct);)
  LeftDrive(setVelocity(0, pct);)

  PIDsRunning -= 1;

  return 0;

}



// Wrapper function that will accept arguments for the main function (_Drive_())
void TurnAt(double turn_distance, double speed, bool wait_for_completion, bool coast, double coustom_timeout, double coustom_settle, bool start_at_100)
{

  // Assign local variables to global variables
  TurnDistance = turn_distance;
  Speed = speed;
  Coast = coast;
  CustomTimeout = coustom_timeout;
  SettleTime = coustom_settle;

  sa100 = start_at_100;

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