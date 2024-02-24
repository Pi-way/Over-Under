
#include "vex.h"

int _Drive_With_Angles_And_Speed_()
{   
  // Assign and declare local variables from global variables.
  double TurnCorrection = 1;
  
  std::vector<std::pair<double, std::pair<double, double>>> LocalList = globalDistanceHeadingSpeedList;

  double LocalDistance = 0;

  for(auto indicies = LocalList.begin(); indicies != LocalList.end(); indicies++ )
  {
    LocalDistance += indicies->first;
  }

  int currentIndex = 0;

  double localTurnDistance = LocalList[0].second.first;

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

  bool localShouldLetTurnSettle = shouldLetTurnSettle;

  bool NotDone = true;
  bool TurnNotDone = true;
  
  PID LocalPID(14.75*0.5, 0.5, 0.1, 200, 25, 4, 100, &NotDone, LocalTimeout, LocalSettle);
  PID LocalTurnPID(1.3 * 0.5, 0.002, 0.01, 200, 10, 6, LocalSpeed, &TurnNotDone, LocalTimeout, 0.125);
  PID BackupTurnPID = LocalTurnPID;

  LocalPID.SpeedCap = LocalList[0].second.second;

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
  double ThisTime = Brain.Timer.systemHighResolution();
  double LastTime = ThisTime;

  double RequestedRight = 0;
  double RequestedLeft = 0;

  double DeltaTime = 0;

  RightDrive(spin(forward);)
  LeftDrive(spin(forward);)
  
  LocalPID.Time = 0;

  bool resetTurn = false;

  //main loop
  while (NotDone || TurnNotDone) 
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

    RightDrive(setVelocity(RequestedRight, pct);)
    LeftDrive(setVelocity(RequestedLeft, pct);)

    wait(50, msec);

    //calculate horizontal and heading error
    Error = LocalDistance - Encoder->position();

    SmallError = LocalList[currentIndex].first - (Encoder->position() - DrivePos);
    LastTurnError = TurnError;
    TurnError = wrapAngleDeg(LocalList[currentIndex].second.first - robot.Inertial.heading(degrees));

    if (localShouldLetTurnSettle && currentIndex+1 >= LocalList.size() && !resetTurn){
      LocalTurnPID = BackupTurnPID;
      TurnNotDone = true;
      resetTurn = true;
      std::cout << "yo bro just reset" << std::endl;
    }


    if((GetSign(LocalList[currentIndex].first) == 1) ? (SmallError <= 0) : (SmallError >= 0))
    {
      if(currentIndex + 1 < LocalList.size()){
        currentIndex ++;
        DrivePos = Encoder->position();
        LocalPID.SpeedCap = LocalList[currentIndex].second.second;
      }
      else
      {
        //wait until done
      }
    }
  }

  delete Encoder;

  //stop the motors
  RightDrive(setVelocity(0, pct);)
  LeftDrive(setVelocity(0, pct);)

  PIDsRunning -= 1;

  return 0;

}


// Wrapper function that will accept arguments for the main function (_Drive_())
void DriveWithAnglesAndSpeed(std::vector<std::pair<double, std::pair<double, double>>> List, double speed, bool wait_for_completion, bool coast, double coustom_timeout, double coustom_settle)
{

  // Assign local variables to global variables
  globalDistanceHeadingSpeedList = List;
  Speed = speed;
  Coast = coast;
  CustomTimeout = coustom_timeout;
  SettleTime = coustom_settle;
  shouldLetTurnSettle = false;

  // Either wait for the function to complete, or run the function in a task
  if (wait_for_completion)
  {
    _Drive_With_Angles_And_Speed_();
  }
  else
  {
    PIDTask = task(_Drive_With_Angles_And_Speed_);
  }
}

// Wrapper function that will accept arguments for the main function (_Drive_())
void DriveWithAnglesAndSpeed(bool _shouldLetTurnSettle, std::vector<std::pair<double, std::pair<double, double>>> List, double speed, bool wait_for_completion, bool coast, double coustom_timeout, double coustom_settle)
{

  // Assign local variables to global variables
  globalDistanceHeadingSpeedList = List;
  Speed = speed;
  Coast = coast;
  CustomTimeout = coustom_timeout;
  SettleTime = coustom_settle;
  shouldLetTurnSettle = _shouldLetTurnSettle;

  // Either wait for the function to complete, or run the function in a task
  if (wait_for_completion)
  {
    _Drive_With_Angles_And_Speed_();
  }
  else
  {
    PIDTask = task(_Drive_With_Angles_And_Speed_);
  }
}