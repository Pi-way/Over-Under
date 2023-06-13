#include <cmath>
#include "vex.h"

double wrapAngle(double angle)
{
    angle = fmod(angle, 2.0 * PI);
    return (angle >= 0) ? angle : angle + 2.0 * PI;
}

void rotatePoint(double& x, double& y, double angle) {
    double newX = x * cos(angle) - y * sin(angle);
    double newY = x * sin(angle) + y * cos(angle);

    x = newX;
    y = newY;
}

double degreesToRadians(double degrees) {
    return degrees * (PI / 180.0);
}