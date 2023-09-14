
#include "vex.h"

int _Drive_With_Angles_()
{   
  // Assign and declare local variables from global variables.
  double TurnCorrection = 1;
  
  distanceHeadingList LocalList = globalDistanceHeadingList;

  double LocalDistance = 0;

  for(auto indicies = LocalList.begin(); indicies != LocalList.end(); indicies++ )
  {
    LocalDistance += ( indicies->first / (2.75*3.1415) ) * 360.0;
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
  
  PID LocalPID(0.11, 0.05, 0.125, 125, 10, LocalSpeed, &NotDone, LocalTimeout, LocalSettle);

  Encoder.setPosition(0, deg);
  RightDrive(setStopping((LocalCoast) ? coast : brake);)
  LeftDrive(setStopping((LocalCoast) ? coast : brake);)

  double DrivePos = 0;

  double SmallError = (( LocalList[currentIndex].first / (2.75*3.1415) ) * 360.0) - (Encoder.position(deg) - DrivePos);

  //calculate initial horizontal and heading error using drivetrain encoders and the inertial sensor
  double Error = LocalDistance - Encoder.position(deg);
  double TurnError = wrapAngleDeg(localTurnDistance - Inertial.heading(degrees));
  double OutputSpeed = 0;

  //initialize time variables for calculating delta time
  double ThisTime = Brain.Timer.time(seconds);
  double LastTime = ThisTime;

  double OutputRight = 0;
  double OutputLeft = 0;

  double ActualAccel = 0;
  double MaxAccel = 400;

  double RequestedRight = 0;
  double RequestedLeft = 0;

  double DeltaTime = 0;

  RightDrive(spin(forward);)
  LeftDrive(spin(forward);)

  //main loop
  while (NotDone)
  {
    //record current time for delta time calculation
    LastTime = ThisTime;
    ThisTime = Brain.Timer.time(seconds);

    DeltaTime = ThisTime - LastTime;

    //recieve the output speed of the PID object after giving it our error and delta time (in seconds)
    OutputSpeed = LocalPID.Update(Error, DeltaTime);

    //set each motor's speed to the sum of the PID speed and turn error multiplied by its coeficient

    RequestedRight = (OutputSpeed - (TurnError * TurnCorrection)) / (std::abs(TurnError/30.0) + 1.0);
    RequestedLeft = (OutputSpeed + (TurnError * TurnCorrection)) / (std::abs(TurnError/30.0) + 1.0);

    ActualAccel = (RequestedRight - OutputRight) / DeltaTime;
    if (std::abs(ActualAccel) > MaxAccel){
      OutputRight += MaxAccel * DeltaTime * GetSign(ActualAccel);
    }
    else
    {
      OutputRight = RequestedRight;
    }

    ActualAccel = (RequestedLeft - OutputLeft) / DeltaTime;
    if (std::abs(ActualAccel) > MaxAccel){
      OutputLeft += MaxAccel * DeltaTime *  GetSign(ActualAccel);
    }
    else
    {
      OutputLeft = RequestedLeft;
    }

    RightDrive(setVelocity(OutputRight, pct);)
    LeftDrive(setVelocity(OutputLeft, pct);)

    vex::task::yield();

    //calculate horizontal and heading error
    Error = LocalDistance - Encoder.position(degrees);

    SmallError = (( LocalList[currentIndex].first / (2.85*3.1415) ) * 360.0) - (Encoder.position(deg) - DrivePos);
    TurnError = wrapAngleDeg(LocalList[currentIndex].second - Inertial.heading(degrees));

    if((GetSign(LocalList[currentIndex].first) == 1) ? (SmallError <= 0) : (SmallError >= 0))
    {
      if(currentIndex + 1 < LocalList.size()){
        currentIndex ++;
        DrivePos = Encoder.position(deg);
      }
      else
      {
        //wait until done
      }
    }
  }

  //stop the motors
  RightDrive(setVelocity(0, pct);)
  LeftDrive(setVelocity(0, pct);)

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