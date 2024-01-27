#include "vex.h"
using namespace vex;

Odometry::Odometry() {  
    // Constructor
    ForwardObserver = robot.ForwardTrack.getObserver();
    StrafeObserver = robot.StrafeTrack.getObserver();
}

void Odometry::Update() {
    // Get change in encoder position in inches
    double fwd_travel = ForwardObserver->getTravel();
    double b_s_travel = StrafeObserver->getTravel();

    // Calculate delta_theta.
    previous_heading = current_heading;
    current_heading = degToRad(wrapAngleDeg(360 - robot.Inertial.rotation(deg)));
    double delta_theta = degToRad(wrapAngleDeg(radToDeg(current_heading) - radToDeg(previous_heading)));

    // Calculate fwd and strafe travel distances; (x and y respectively.)
    double strafe_travel = b_s_travel + delta_theta * strafe_radius;

    // Rotate the point (fwd, strafe) to convert from a local coordinate shift to a global coordinate shift.
    rotatePoint(fwd_travel, strafe_travel, previous_heading);

    // Update each location variable. We keep things in radians internally because the math is nicer this way.
    internal_h_rad = wrapAngleRad(current_heading);
    h = radToDeg(internal_h_rad);
    x += fwd_travel;
    y += strafe_travel;
}

void Odometry::Calibrate(double _x, double _y, double _h) {
    // Reset inertial sensor.
    robot.Inertial.setHeading(_h, deg);
    robot.Inertial.setRotation(_h, deg);

    // Apply changes to odometry location.
    x = _x;
    y = _y;
    h = _h;
    internal_h_rad = degToRad(h);
}
