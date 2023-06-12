#pragma once
using namespace vex;

class Odometry{
public:

    Odometry();

    TrackingWheel fr_fwd = TrackingWheel(rotation(PORT5, true), 3.25);
    TrackingWheel fl_fwd = TrackingWheel(rotation(PORT6, false), 3.25);
    TrackingWheel b_s = TrackingWheel(rotation(PORT8, false), 3.25);

    double fwd_radius = 5.71;
    double strafe_radius = 4.42;

    void Update();
};