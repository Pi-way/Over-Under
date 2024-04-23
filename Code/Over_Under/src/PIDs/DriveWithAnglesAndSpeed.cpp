
#include "vex.h"

int _Drive_With_Angles_And_Speed_()
{   
  // Assign and declare local variables from global variables.
  double TurnCorrection = 1;
  
  std::vector<std::pair<double, std::pair<double, double>>> LocalList = globalDistanceHeadingSpeedList;

  double LocalDistance = 0;

  int currentIndex = 0;

  double localTurnDistance = LocalList[0].second.first;

  double LocalSpeed = Speed / 100.0 * 12.0;
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
  bool DummyTurnNotDone = true;
  bool TurnNotDone = true;
  
  PID LocalPID(0.85, 0.1, 0.06, 50, 2, 3, 12, &NotDone, LocalTimeout, LocalSettle);
  PID BackupLocalPID = LocalPID;
  PID LocalTurnPID(0.275, 0.1, 0.025, 75, 5, 6, LocalSpeed, (LocalList.size() == 1) ? &TurnNotDone: &DummyTurnNotDone, LocalTimeout, 0.125);
  PID BackupTurnPID = LocalTurnPID;

  LocalPID.SpeedCap = LocalList[0].second.second;

  auto Encoder = robot.ForwardTrack.getObserver();

  RightDrive(setStopping((LocalCoast) ? coast : brake);)
  LeftDrive(setStopping((LocalCoast) ? coast : brake);)

  double DrivePos = 0;

  double DistanceUntilSignChange = 0;
  int FirstDistanceSign = GetSign(LocalList[currentIndex].first);
  int LastFirstDistanceSign = FirstDistanceSign;

  for(auto distance = LocalList.begin() + currentIndex; distance != LocalList.end(); distance++ )
  {
    if (GetSign(distance->first) != FirstDistanceSign) {
      break;
    }
    DistanceUntilSignChange += distance->first;
  }

  double SmallError = LocalList[currentIndex].first - (Encoder->position() - DrivePos);
  double Error = DistanceUntilSignChange - Encoder->position();
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

  double startTime = Brain.Timer.value();

  //main loop
  while (((NotDone && (Brain.Timer.value() - startTime < LocalTimeout && currentIndex < LocalList.size())) || TurnNotDone) && !(Brain.Timer.value() - startTime > LocalTimeout)) 
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

    RightDrive(spin(fwd, RequestedRight + (1 * GetSign(RequestedRight)), voltageUnits::volt);)
    LeftDrive(spin(fwd, RequestedLeft, voltageUnits::volt);)

    wait(50, msec);
    
    //calculate horizontal and heading error
    Error = DistanceUntilSignChange - Encoder->position();

    SmallError = LocalList[currentIndex].first - (Encoder->position() - DrivePos);
    LastTurnError = TurnError;
    TurnError = wrapAngleDeg(LocalList[currentIndex].second.first - robot.Inertial.heading(degrees));

    if (localShouldLetTurnSettle && currentIndex+1 >= LocalList.size() && !resetTurn){
      LocalTurnPID = BackupTurnPID;
      TurnNotDone = true;
      resetTurn = true;
      LocalTurnPID.Time = Brain.Timer.value() - startTime;
    }

    if((LocalList[currentIndex].first > 0) ? (SmallError <= 0) : (SmallError >= 0))
    {
      if(currentIndex + 1 < LocalList.size()){
        LastFirstDistanceSign = FirstDistanceSign;

        currentIndex ++;
        DrivePos = Encoder->position();
        LocalPID.SpeedCap = LocalList[currentIndex].second.second / 100.0 * 12.0;

        //Allow Direction Switches
        FirstDistanceSign = GetSign(LocalList[currentIndex].first);

        //Check the lenght of the list to avoid a memory permission error
        if(LocalList.size() > 1){
          //If the sign has changed since the last distance
          if(GetSign(LocalList[currentIndex].first) != GetSign(LocalList[currentIndex - 1].first)){

            std::cout<<"accumulating distance again"<<std::endl;
            for(auto distance = LocalList.begin() + currentIndex; distance != LocalList.end(); distance++ )
            {
              if (GetSign(distance->first) != FirstDistanceSign) {
                break;
              }
              DistanceUntilSignChange += distance->first;
            }
          }
        }
       
        

        if(LastFirstDistanceSign != FirstDistanceSign){
          LocalPID = BackupLocalPID;
          LocalPID.Time = Brain.Timer.value() - startTime;
          NotDone = true;
        }
        
        if (currentIndex + 1 == LocalList.size()){
          LocalTurnPID = BackupTurnPID;
          LocalTurnPID.Time = Brain.Timer.value() - startTime;
          LocalTurnPID.HasRampedUp = false;
          LocalTurnPID.HasRampedUp = false;
          LocalTurnPID.NotDone = &TurnNotDone;
        }

      }
      else
      {

      }
    }
  }

  delete Encoder;

  //stop the motors
  RightDrive(stop();)
  LeftDrive(stop();)

  PIDsRunning -= 1;
  wait(20, msec);

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