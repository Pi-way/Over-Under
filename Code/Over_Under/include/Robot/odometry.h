#pragma once
using namespace vex;

class Odometry{
public:

    Odometry();

    TrackingWheel fwd = TrackingWheel(rotation(PORT12), 3.25, true);
    TrackingWheel strafe = TrackingWheel(rotation(PORT11), 3.25);

    inertial inert = inertial(PORT13, left);

    double strafe_radius = 4.5;

    double previous_heading = 0;
    double current_heading = 0;

    double x = 0;
    double y = 0;
    double internal_h_rad = 0;
    double h = 0;

    void Update();
    void Calibrate(double _x = 0, double _y = 0, double _h = 0);
    void SetPosition(double _x, double _y, double _h);
};