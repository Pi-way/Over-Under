#pragma once
#include "vex.h"

class Path{
public:
    ListOfLambdaWithPoint *PathPoints;
    std::vector<int> InformativePath;
    PointList ListOfPathPoints;

    Path(ListOfLambdaWithPoint *path_points = new ListOfLambdaWithPoint{{{0, 0}, NoLambda}, {{0, 0}, NoLambda}});
    void UpdateInformativeList(double look_ahead_radius);
};


class PurePursuitRobot{
public:

    Path *CurrentPath;
    double LookAheadRadius = 20;
    double TurnCorrectionSensitivity = 80; //Bigger is less sensitive

    double DeltaTime = 0;

    int DirectionMultiplier = 1;

    bool *PidNotDone = new bool(true);
    bool *TurnPidNotDone = new bool(true);
    bool *PointLocked = new bool(false);
    
    PID InitialPid = PID(14.75*0.5, 0.5, 0.1, 200, 25, 4, 100, PidNotDone, 10000000, 0.125);
    PID Pid = InitialPid;

    PID InitialTurnPid = PID(1.3 * 0.5, 0.001, 0.01, 200, 10, 6, 100, TurnPidNotDone, 10000000, 0.125);
    PID TurnPid = InitialTurnPid;

    std::vector<int> TaskList;

    PurePursuitRobot();
    void SetPath(Path *path);
    void PathGenLoop();

};

extern PurePursuitRobot PureRobot;
