/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       TurnAt.cpp                                                */
/*    Author:       Team 98548J (Ace)                                         */
/*    Created:      12-15-22                                                  */
/*    Description:  Source file for Turn function                             */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include <vex.h>

double printer = 0;

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
  
  PID LocalPID((1.7675)*0.45, 0.0005, 0.0225, 200, 10, 6, LocalSpeed, &NotDone, LocalTimeout, LocalSettle);

  RightDrive(setStopping((LocalCoast) ? coast : brake);)
  LeftDrive(setStopping((LocalCoast) ? coast : brake);)

  double Error = wrapAngleDeg(LocalDistance - robot.Inertial.heading(degrees));
  double OutputSpeed = 0;

  double ThisTime = Brain.Timer.systemHighResolution();
  double LastTime = ThisTime;

  RightDrive(spin(forward);)
  LeftDrive(spin(forward);)

  task printing_task = task([]()->int{
    while(true){
      wait(100, msec);
      printf("\n");
      printf("\033[31m %3.3f", (float)printer);
    }
    return 0;
  });
  
  while (NotDone)
  {
    LastTime = ThisTime;
    ThisTime = Brain.Timer.systemHighResolution();
    OutputSpeed = LocalPID.Update(Error, (ThisTime - LastTime)/1000000.0);

    RightDrive(setVelocity(-OutputSpeed, pct);)
    LeftDrive(setVelocity(OutputSpeed, pct);)

    wait(50, msec);
    printer = LocalPID.Error;

    Error = wrapAngleDeg(LocalDistance - robot.Inertial.heading(degrees));

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