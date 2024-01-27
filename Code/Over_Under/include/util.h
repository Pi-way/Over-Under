#define PI 3.14159

int GetSign(double number);

double wrapAngleRad(double angle);

double wrapAngleDeg(double angle);

double degToRad(double degrees);

double radToDeg(double radians);

/// @brief USES A REFRENCE!!! WILL CHANGE VARIABLES INPUTTED
void rotatePoint(double& x, double& y, double angle);

uint8_t* copyStringStreamToHeap(std::stringstream& ss);

void clearConsole();

double GetAngleTo(double x, double y, double heading, double target_x, double target_y);

double GetDistance(double x1, double y1, double x2, double y2);