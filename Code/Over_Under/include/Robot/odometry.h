#pragma once
using namespace vex;

class Odometry{
public:

    Odometry();

    TrackingWheel fr_fwd = TrackingWheel(rotation(PORT5, false), 3.25, true);
    TrackingWheel fl_fwd = TrackingWheel(rotation(PORT6, false), 3.25);
    TrackingWheel b_s = TrackingWheel(rotation(PORT8, false), 3.25);

    inertial inert = inertial(PORT11, left);

    double fwd_radius = 5.75 / 2.0;
    double strafe_radius = 4.5;

    double previous_heading = 0;
    double current_heading = 0;

    double x = 0;
    double y = 0;
    double h = 0;

    void Update();
};