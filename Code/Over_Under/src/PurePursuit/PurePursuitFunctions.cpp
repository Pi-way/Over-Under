#include "vex.h"

LambdaActivationRadiusPair NoLambda = {[](){}, {-1, 0}};

double Radians(double degrees){
    return degrees*((3.1415926535)/180.0);
}

double Degrees(double radians){
    return radians*(180.0/(3.1415926535));
}

double Cap(double input, double cap){
    if(std::abs(input) > cap){
        return cap * (input/std::abs(input));
    }else{
        return input;
    }
}

bool PointContainedBetween(Point point1, Point point2, Point point){

    if(point1[0] <= point2[0]){
        if(point1[1] <= point2[1]){
            return (point[0] >= point1[0] && point[0] <= point2[0]) && (point[1] >= point1[1] && point[1] <= point2[1]);
        }else{
            return (point[0] >= point1[0] && point[0] <= point2[0]) && (point[1] <= point1[1] && point[1] >= point2[1]);
        }
    }else{
        if(point1[1] <= point2[1]){
            return (point[0] <= point1[0] && point[0] >= point2[0]) && (point[1] >= point1[1] && point[1] <= point2[1]);
        }else{
            return (point[0] <= point1[0] && point[0] >= point2[0]) && (point[1] <= point1[1] && point[1] >= point2[1]);
        }
    }
}

IndexPointPairList GetIntersections(PointList path_points, Point circle_focus, double circle_radius){

    // see https://www.desmos.com/calculator/cejithefwg
    IndexPointPairList verified_intersections;


    // coordinates of the circle
    double c = circle_focus[0];
    double d = circle_focus[1];

    // radius of the circle
    double r = circle_radius;

    for(int index = 0; index < path_points.size() - 1; index ++){

        // slope and y - intercept for line
        if((path_points[index][0] - path_points[index + 1][0]) == 0){
            if(std::abs(path_points[index][0] - c) > circle_radius){
                //do nothing
            }else{

                // positive or negative (sqrt())
                double a = sqrt(pow(circle_radius, 2) + -pow((path_points[index][0] - c), 2));
                double j = path_points[index][0] - c;

                Point intersection1 = {0, 0};
                Point intersection2 = {0, 0};

                // calculate the circle intersections (raw)
                intersection1[0] = c + j;
                intersection1[1] = d + a;

                intersection2[0] = c + j;
                intersection2[1] = d - a;
                
                if(intersection1 == intersection2){
                    if(PointContainedBetween(path_points[index], path_points[index+1], intersection1)){
                        verified_intersections.push_back(std::pair<int, Point>(index, intersection1));
                    }
                }else{
                    if(PointContainedBetween(path_points[index], path_points[index+1], intersection1)){
                        verified_intersections.push_back(std::pair<int, Point>(index, intersection1));
                    }
                    if(PointContainedBetween(path_points[index], path_points[index+1], intersection2)){
                        verified_intersections.push_back(std::pair<int, Point>(index, intersection2));
                    }
                }
            }
        }else{

            double m = (path_points[index][1] - path_points[index + 1][1]) / (path_points[index][0] - path_points[index + 1][0]);
            double b = -m * path_points[index][0] + path_points[index][1];

            // get values for the quadratic formula
            double g = pow(m, 2) + 1;
            double h = 2*m*b - 2*m*d - 2*c;
            double i = pow(b, 2) + pow(c, 2) + pow(d, 2) - pow(r, 2) - 2*b*d;


            if((pow(h, 2) - 4*g*i) < 0){ // if this number is negative, then the circle doesn't intersect the line.
                // do nothing
            }else{

                Point intersection1 = {0, 0};
                Point intersection2 = {0, 0};

                // calculate the circle intersections (raw)
                intersection1[0] = (-h + sqrt(pow(h, 2) - 4*g*i)) / (2*g);
                intersection1[1] = m*intersection1[0] + b;

                intersection2[0] = (-h - sqrt(pow(h, 2) - 4*g*i)) / (2*g);
                intersection2[1] = m*intersection2[0] + b;              

                if(intersection1 == intersection2){
                    if(PointContainedBetween(path_points[index], path_points[index+1], intersection1)){
                        verified_intersections.push_back(std::pair<int, Point>(index, intersection1));
                    }
                }else{
                    if(PointContainedBetween(path_points[index], path_points[index+1], intersection1)){
                        verified_intersections.push_back(std::pair<int, Point>(index, intersection1));
                    }
                    if(PointContainedBetween(path_points[index], path_points[index+1], intersection2)){
                        verified_intersections.push_back(std::pair<int, Point>(index, intersection2));
                    }
                }
            }
        }
    }
    return verified_intersections;
}

double GetErrorFromPath(double x, double y, PointList path_points, int current_path_segment, int end_index){

    double error = 0;

    if(current_path_segment >= end_index){
        error = GetDistance(x, y, path_points[end_index][0], path_points[end_index][1]);
    }else{
        error += GetDistance(x, y, path_points[current_path_segment + 1][0], path_points[current_path_segment + 1][1]);
        for(int index = current_path_segment + 1; index < end_index; index ++){
            error += GetDistance(path_points[index][0], path_points[index][1], path_points[index + 1][0], path_points[index + 1][1]);
        }
    }
    return error;
}