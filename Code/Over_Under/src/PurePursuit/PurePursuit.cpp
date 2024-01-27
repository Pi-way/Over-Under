#include "vex.h"


template <typename T>
std::vector<T> slicing(std::vector<T> const &v, int X, int Y)
{
    // Begin and End iterator
    auto first = v.begin() + X;
    auto last = v.begin() + Y + 1;
    // Copy the element
    PointList vector(first, last);

    // Return the results
    return vector;
}

Path::Path(ListOfLambdaWithPoint *path_points){
    PathPoints = path_points;

    for(int i = 0; i < PathPoints->size(); i ++){
        ListOfPathPoints.push_back((*PathPoints)[i].first);
    }
};

void Path::UpdateInformativeList(double look_ahead_radius){

    double point_distance = 0.0;
    int current_point_index = 0;

    for(int index = 0; index < PathPoints->size()-1; index ++){

        point_distance = 0.0;
        current_point_index = index;

        while(point_distance < look_ahead_radius){
            current_point_index += 1;
            if(current_point_index == PathPoints->size()-1){
                break;
            }
            point_distance = GetDistance((*PathPoints)[index].first[0],
                                            (*PathPoints)[index].first[1],
                                            (*PathPoints)[current_point_index].first[0],
                                            (*PathPoints)[current_point_index].first[1]);
        }
        InformativePath.push_back(current_point_index - index);
    }
}

PurePursuitRobot::PurePursuitRobot(){}

void PurePursuitRobot::SetPath(Path *path){
    CurrentPath = path;
    //For every path point check if there is a lambda associated with it, if there is, add it to the task list
    for(int i = 0; i < (*CurrentPath->PathPoints).size(); i++){
        if((*CurrentPath->PathPoints)[i].second.second != NoLambda.second){
            TaskList.push_back(i);
        }
    }
}

void PurePursuitRobot::PathGenLoop(){

    CurrentPath->UpdateInformativeList(LookAheadRadius);

    IndexPointPairList PathIntersections = {};
    IndexPointPairList *TruePathIntersections = new IndexPointPairList{};
    IndexPointPairList ValidNextIntersections = {};

    int CurrentPathSegment = 0;

    Point TargetPoint = {0, 0};
    double CorrectionAngle = 0;

    std::vector<int> *ActivatedTaskIndexes = new std::vector<int>{};

    Pid = InitialPid;
    TurnPid = InitialTurnPid;

    double Error;
    double OverallSpeed;
    double TurnSpeed;

    int StoppingPointIndex;
    bool FoundStoppingPoint;
    int Index = 0;

    std::vector<int> *ActivatedTaskIndexesToBeRemoved = new std::vector<int>{};
    std::vector<int>::iterator TaskListIterator;
    std::vector<int> TaskListplaceholder = {};

    double CurrentTime = Brain.Timer.value();
    double LastTime = Brain.Timer.value();

    while(true){
        vex::task::yield();
        
        //##################### determine the stopping_point_index #####################
        StoppingPointIndex = -1;

        // Note: doesn't iterate through ...field_path_points[current_path_segment:]
        FoundStoppingPoint = false;
        Index = 0;
        
        //Make sure stopping point doesn't advance when Point is locked
        if(!*PointLocked){
            while(!FoundStoppingPoint && TaskList.size() != 0 && Index + CurrentPathSegment <= (*CurrentPath->PathPoints).size() - 1){

                //if the Index + CurrentPathSegment contains a lambda
                if((*CurrentPath->PathPoints)[Index + CurrentPathSegment].second.second.first != -1 && std::count(TaskList.begin(), TaskList.end(), Index + CurrentPathSegment) > 0){
                
                    if((*CurrentPath->PathPoints)[Index + CurrentPathSegment].second.second.first == 1){
                        StoppingPointIndex = Index + CurrentPathSegment;
                        FoundStoppingPoint = true;
                    }else if((*CurrentPath->PathPoints)[Index + CurrentPathSegment].second.second.first == 2){
                        StoppingPointIndex = Index + CurrentPathSegment; // Activate and stop, wait for tasks to finish
                        FoundStoppingPoint = true;
                    }
                }
                Index += 1;
            }
            if(StoppingPointIndex == -1){
                StoppingPointIndex = (*CurrentPath->PathPoints).size() -1;
            }
        }else{
            StoppingPointIndex = CurrentPathSegment;
        }
        
        //##################### determine the target point #####################
        PathIntersections = GetIntersections(slicing(CurrentPath->ListOfPathPoints, 0, StoppingPointIndex), Point{odom.x, odom.y}, LookAheadRadius);
        //for_each will iterate through Path intersections to determine which ones the robot should target
        for_each(PathIntersections.begin(), PathIntersections.end(), 
            [this, CurrentPathSegment, TruePathIntersections](IndexPointPair Intersection) mutable 
            {
                if(Intersection.first >= CurrentPathSegment && Intersection.first < CurrentPath->InformativePath[CurrentPathSegment] + CurrentPathSegment)
                {
                    (*TruePathIntersections).push_back(Intersection);
                }
            }
        );
        //for_each will iterate through Path intersections to determine which ones the robot will target in the future
        for_each(PathIntersections.begin(), PathIntersections.end(),
            [this, CurrentPathSegment, ValidNextIntersections](IndexPointPair Intersection) mutable
            {
                if(Intersection.first >= CurrentPathSegment + 1 && Intersection.first < CurrentPath->InformativePath[CurrentPathSegment + 1] + CurrentPathSegment + 1)
                {
                    ValidNextIntersections.push_back(Intersection);
                }
            }
        );
        
        //make sure current path segment doesn't change when point is locked
        if(!*PointLocked){
            if(!(CurrentPathSegment == StoppingPointIndex)){
                if(GetDistance(odom.x, odom.y, (*CurrentPath->PathPoints)[CurrentPathSegment + 1].first[0], (*CurrentPath->PathPoints)[CurrentPathSegment + 1].first[1]) < LookAheadRadius){
                    if(CurrentPathSegment == StoppingPointIndex-1){
                        CurrentPathSegment = StoppingPointIndex;
                    }else{
                        CurrentPathSegment += 1;
                    }
                    (*TruePathIntersections) = ValidNextIntersections;
                }
            }
        }


        if((*TruePathIntersections).size() > 0){  // not empty
            if((*TruePathIntersections).size() > 1){  // has at least two elements
                if((*TruePathIntersections)[(*TruePathIntersections).size()-1].first > (*TruePathIntersections)[(*TruePathIntersections).size()-2].first){
                    TargetPoint = (*TruePathIntersections)[(*TruePathIntersections).size()-1].second;
                }else if(GetDistance((*CurrentPath->PathPoints)[(*TruePathIntersections)[(*TruePathIntersections).size()-1].first].first[0], (*CurrentPath->PathPoints)[(*TruePathIntersections)[(*TruePathIntersections).size()-1].first].first[1], (*TruePathIntersections)[(*TruePathIntersections).size()-1].second[0], (*TruePathIntersections)[(*TruePathIntersections).size()-1].second[1]) > GetDistance((*CurrentPath->PathPoints)[(*TruePathIntersections)[(*TruePathIntersections).size()-1].first].first[0], (*CurrentPath->PathPoints)[(*TruePathIntersections)[(*TruePathIntersections).size()-2].first].first[1], (*TruePathIntersections)[(*TruePathIntersections).size()-2].second[0], (*TruePathIntersections)[(*TruePathIntersections).size()-2].second[1])){
                    TargetPoint = (*TruePathIntersections)[(*TruePathIntersections).size()-1].second;
                }else{
                    TargetPoint = (*TruePathIntersections)[(*TruePathIntersections).size()-2].second;
                }
            }
            else{
                TargetPoint = (*TruePathIntersections)[0].second;
            }
        }
        else{
            TargetPoint = (*CurrentPath->PathPoints)[CurrentPathSegment].first; //This line didn't work in the first place ???
        }
        //Manage Lambdas ####################################################################################################################

        //Check if any lambdas are withing activation range, activating ones that are
        for_each(TaskList.begin(), TaskList.end(), [this, CurrentPathSegment, ActivatedTaskIndexes, ActivatedTaskIndexesToBeRemoved](int task_index) mutable
            {
                if(GetErrorFromPath(odom.x, odom.y, CurrentPath->ListOfPathPoints, CurrentPathSegment, task_index) <= (*CurrentPath->PathPoints)[task_index].second.second.second && CurrentPathSegment == task_index){
                    //Call lambda
                    (*CurrentPath->PathPoints)[task_index].second.first();
                    (*ActivatedTaskIndexes).push_back(task_index);
                    (*ActivatedTaskIndexesToBeRemoved).push_back(task_index);
                }
            }
        );

        for(int Index = 0; Index < (*ActivatedTaskIndexesToBeRemoved).size(); Index ++){
            TaskListIterator = remove(TaskList.begin(), TaskList.end(), (*ActivatedTaskIndexesToBeRemoved)[Index]);
            TaskListplaceholder.clear();
            for(auto I = TaskList.begin(); I != TaskListIterator; I ++){
                TaskListplaceholder.push_back(*I);
            }
            TaskList.clear();
            TaskList = TaskListplaceholder;
        }

        (*ActivatedTaskIndexesToBeRemoved).clear();

        //##################### determine the correction angle #####################
        CorrectionAngle = -GetAngleTo(odom.x, odom.y, odom.h + (DirectionMultiplier == -1 ? 180 : 0), TargetPoint[0], TargetPoint[1]);

        //##################### determine the error for the PID #####################
        Error = GetErrorFromPath(odom.x, odom.y, CurrentPath->ListOfPathPoints, CurrentPathSegment, StoppingPointIndex);

        //##################### calculate and apply the speeds for each half of the drivetrain #####################
        if(Error < 1){
            Error = 0;
        }

        CorrectionAngle = wrapAngleDeg(CorrectionAngle);

        LastTime = CurrentTime;
        CurrentTime = Brain.Timer.value();
        DeltaTime = CurrentTime - LastTime;

        OverallSpeed = Pid.Update(Error, DeltaTime);
        TurnSpeed = TurnPid.Update(CorrectionAngle, DeltaTime);

        OverallSpeed = (OverallSpeed * 70)/(std::abs(CorrectionAngle) + 70);

        printf(" %3.3f\n", OverallSpeed);

        RightDrive(setVelocity(OverallSpeed - TurnSpeed * 2, pct);)
        LeftDrive(setVelocity(OverallSpeed + TurnSpeed * 2, pct);)

        if(Error < 1){
            Pid = InitialPid;
        }
        
        wait(50, msec);
        vex::task::yield();
    }
}