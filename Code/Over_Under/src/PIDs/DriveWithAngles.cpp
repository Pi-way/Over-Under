
#include "vex.h"

int _Drive_With_Angles_()
{   
  // Assign and declare local variables from global variables.
  double TurnCorrection = 1;
  
  distanceHeadingList LocalList = globalDistanceHeadingList;

  double LocalDistance = 0;

  for(auto indicies = LocalList.begin(); indicies != LocalList.end(); indicies++ )
  {
    LocalDistance += indicies->first;
  }

  int currentIndex = 0;

  double localTurnDistance = LocalList[0].second;

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
  bool TurnNotDoneFR = true;

  PID LocalPID(0.85, 0.1, 0.06, 50, 2, 3, LocalSpeed/100.0*12.0, &NotDone, LocalTimeout, LocalSettle);
  PID LocalTurnPID(0.2, 0.1, 0.015, 50, 5, 6, 12, &TurnNotDone, 1000000, 0.125);

  auto Encoder = robot.ForwardTrack.getObserver();

  RightDrive(setStopping((LocalCoast) ? coast : brake);)
  LeftDrive(setStopping((LocalCoast) ? coast : brake);)

  double DrivePos = 0;

  double SmallError = LocalList[currentIndex].first - (Encoder->position() - DrivePos);

  //calculate initial horizontal and heading error using drivetrain encoders and the inertial sensor
  double Error = LocalDistance - Encoder->position();
  double TurnError = wrapAngleDeg(localTurnDistance - robot.Inertial.heading(degrees));
  double LastTurnError;
  double OutputSpeed = 0;
  double TurnOutputSpeed = 0;

  //initialize time variables for calculating delta time
  double ThisTime = Brain.Timer.time(seconds);
  double LastTime = ThisTime;

  double RequestedRight = 0;
  double RequestedLeft = 0;

  double DeltaTime = 0;

  RightDrive(spin(forward);)
  LeftDrive(spin(forward);)

  //main loop
  while (NotDone || TurnNotDoneFR)
  {
    //record current time for delta time calculation
    LastTime = ThisTime;
    ThisTime = Brain.Timer.systemHighResolution();

    DeltaTime = (ThisTime - LastTime)/1000000.0;

    //receive the output speed of the PID object after giving it our error and delta time (in seconds)
    OutputSpeed = LocalPID.Update(Error, DeltaTime);
    TurnOutputSpeed = LocalTurnPID.Update(TurnError, DeltaTime);

    RequestedRight = (OutputSpeed - TurnOutputSpeed);
    RequestedLeft = (OutputSpeed + TurnOutputSpeed);

    RightDrive(spin(fwd, RequestedRight + (0.75 * GetSign(RequestedRight)), voltageUnits::volt);)
    LeftDrive(spin(fwd, RequestedLeft, voltageUnits::volt);)

    wait(50, msec);

    //calculate horizontal and heading error
    Error = LocalDistance - Encoder->position();

    SmallError = LocalList[currentIndex].first - (Encoder->position() - DrivePos);
    LastTurnError = TurnError;
    TurnError = wrapAngleDeg(LocalList[currentIndex].second - robot.Inertial.heading(deg));

    if(currentIndex + 1 == LocalList.size()) {
      if(std::abs(TurnError) < 1){
        TurnNotDoneFR = false;
      }
    }

    if((GetSign(LocalList[currentIndex].first) == 1) ? (SmallError <= 0) : (SmallError >= 0))
    {
      if(currentIndex + 1 < LocalList.size()){
        currentIndex ++;
        DrivePos = Encoder->position();
      }
      else
      {
        //wait until done
      }
    }
  }

  delete Encoder;

  //stop the motors
  RightDrive(stop();)
  LeftDrive(stop();)

  PIDsRunning -= 1;

  return 0;

}


// Wrapper function that will accept arguments for the main function (_Drive_())
void DriveWithAngles(distanceHeadingList List, double speed, bool wait_for_completion, bool coast, double coustom_timeout, double coustom_settle)
{

  // Assign local variables to global variables
  globalDistanceHeadingList = List;
  Speed = speed;
  Coast = coast;
  CustomTimeout = coustom_timeout;
  SettleTime = coustom_settle;

  // Either wait for the function to complete, or run the function in a task
  if (wait_for_completion)
  {
    _Drive_With_Angles_();
  }
  else
  {
    PIDTask = task(_Drive_With_Angles_);
  }
}