#pragma once
using namespace vex;

class Odometry{
public:

    Odometry();

    TrackingWheelObserver* ForwardObserver = nullptr; 
    TrackingWheelObserver* StrafeObserver = nullptr;

    double strafe_radius = 2.75;

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