#include "vex.h"
using namespace vex;

Odometry::Odometry(){

    inert.setHeading(0, deg);
    inert.setRotation(0, deg);

}

void Odometry::Update(){

    // Get change in encoder position in inches

    double fr_fwd_travel = fr_fwd.get_travel();
    double fl_fwd_travel = fl_fwd.get_travel();
    double b_s_travel = b_s.get_travel();
    

    previous_heading = current_heading;
    current_heading = degreesToRadians(inert.rotation(deg));

    double delta_theta = (current_heading - previous_heading) / 2.0;

    double fwd_travel = (fr_fwd_travel + fl_fwd_travel) / 2.0;
    double strafe_travel = b_s_travel - delta_theta * strafe_radius;


    rotatePoint(fwd_travel, strafe_travel, h + delta_theta);

    h = wrapAngle(current_heading);
    x += fwd_travel;
    y += strafe_travel;

}