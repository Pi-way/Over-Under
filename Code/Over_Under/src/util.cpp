#include "vex.h"

int GetSign(double number)
{
    return (number < 0) ? -1 : 1;
}

double wrapAngleRad(double angle)
{

    angle = fmod(angle + PI, 2.0 * PI);
    return (angle >= 0) ? angle - PI : angle + PI;

}

double wrapAngleDeg(double angle)
{

    angle = fmod(angle + 180.0, 360.0);
    return (angle >= 0) ? angle - 180.0 : angle + 180.0;

}

double degToRad(double degrees) {

    return degrees * (PI / 180.0);

}

double radToDeg(double radians) {

    return radians * (180.0 / PI);
}

void rotatePoint(double& x, double& y, double angle) {

    double newX = x * cos(angle) - y * sin(angle);
    double newY = x * sin(angle) + y * cos(angle);

    x = newX;
    y = newY;

}

uint8_t* copyStringStreamToHeap(std::stringstream& ss) {
  // Get the length of the string stream
  ss.seekg(0, std::ios::end);
  const size_t length = ss.tellg();
  ss.seekg(0, std::ios::beg);

  // Allocate memory on the heap
  uint8_t* ptr = new uint8_t[length];

  // Copy the string stream contents to the allocated memory
  ss.read(reinterpret_cast<char*>(ptr), length);

  return ptr;
}

void clearConsole() {
    printf("\x1B[2J\x1B[H");
}

double GetDistance(double x1, double y1, double x2, double y2){
    return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

double GetAngleTo(double x, double y, double heading, double target_x, double target_y){

    // calculate a point directly in front of the robot (exactly 1 inch away)
    double x2 = x + cos(degToRad(heading));
    double y2 = y + sin(degToRad(heading));

    // get Distance B and C for arccos function
    double distance_b = GetDistance(x, y, target_x, target_y);
    double distance_c = GetDistance(x2, y2, target_x, target_y);

    // get the positive angle from the robot to the target
    double angle_to;
    double a = pow(distance_c, 2) - pow(distance_b, 2) - 1;
    double b = -2 * distance_b;
    
    if(b == 0.0){
        angle_to = 0;
    }else if(a/b > 1 || a/b < -1){
        angle_to = 0;
    }else{
        angle_to = radToDeg(acos(a/b));
    }
        
    // Move the target point so that relative to the robot, the robot would be at (0, 0) and assign that point to MTX and MTY
    double mtx = target_x - x;
    double mty = target_y - y;

    // Get rotated Y value that will determine if the robot needs to drive left or right
    double rotated_y = -mtx*sin(degToRad(heading))+mty*cos(degToRad(heading));

    // Make LocalTurn positive or negative depending on which direction the robot needs to drive
    return angle_to * GetSign(rotated_y);
}