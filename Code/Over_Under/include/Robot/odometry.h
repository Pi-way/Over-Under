#pragma once
using namespace vex;

class Odometry{
public:

    Odometry();

    TrackingWheel fr_fwd = TrackingWheel(rotation(PORT5), 3.25, true);
    TrackingWheel fl_fwd = TrackingWheel(rotation(PORT6), 3.25);
    TrackingWheel b_s = TrackingWheel(rotation(PORT8), 3.25);

    inertial inert = inertial(PORT11, left);

    double strafe_radius = 4.5;

    double previous_heading = 0;
    double current_heading = 0;

    double x = 0;
    double y = 0;
    double internal_h_rad = 0;
    double h = 0;

    void Update();
    void Callibrate(double _x = 0, double _y = 0, double _h = 0);
    void SetPosition(double _x, double _y, double _h);
};