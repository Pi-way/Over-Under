#include "vex.h"

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
