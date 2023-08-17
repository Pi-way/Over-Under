// #include "vex.h"
// using namespace vex;

// Odometry::Odometry() {
//     // Constructor
// }

// void Odometry::Update() {
//     // Get change in encoder position in inches
//     double fr_fwd_travel = fr_fwd.get_travel();
//     double fl_fwd_travel = fl_fwd.get_travel();
//     double b_s_travel = b_s.get_travel();

//     // Calculate delta_theta.
//     previous_heading = current_heading;
//     current_heading = degToRad(inert.rotation(deg));
//     double delta_theta = current_heading - previous_heading;

//     // Calculate fwd and strafe travel distances; (x and y respectively.)
//     double fwd_travel = (fr_fwd_travel + fl_fwd_travel) / 2.0;
//     double strafe_travel = b_s_travel + delta_theta * strafe_radius;

//     // Rotate the point (fwd, strafe) to convert from a local coordinate shift to a global coordinate shift.
//     rotatePoint(fwd_travel, strafe_travel, previous_heading);

//     // Update each location variable. We keep things in radians internally because the math is nicer this way.
//     internal_h_rad = wrapAngleRad(current_heading);
//     h = radToDeg(internal_h_rad);
//     x += fwd_travel;
//     y += strafe_travel;
// }

// void Odometry::Callibrate(double _x, double _y, double _h) {
//     // Reset inertial sensor.
//     inert.calibrate();
//     inert.setHeading(_h, deg);
//     inert.setRotation(_h, deg);

//     // Apply changes to odometry location.
//     x = _x;
//     y = _y;
//     h = _h;
//     internal_h_rad = degToRad(h);
// }
