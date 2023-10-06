#define PI 3.14159

double wrapAngleRad(double angle);

double wrapAngleDeg(double angle);

double degToRad(double degrees);

double radToDeg(double radians);

/// @brief USES A REFRENCE!!! WILL CHANGE VARIABLES INPUTTED
void rotatePoint(double& x, double& y, double angle);

uint8_t* copyStringStreamToHeap(std::stringstream& ss);

void clearConsole();