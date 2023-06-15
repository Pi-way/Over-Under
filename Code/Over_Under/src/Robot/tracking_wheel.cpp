#include "vex.h"
using namespace vex;

TrackingWheel::TrackingWheel(rotation _encoder, double _diameter, bool _reverse){

    // Initialize member vairables

    encoder = _encoder;
    diameter = _diameter;
    reverse = _reverse;

    // Reset the encoder

    encoder.resetPosition();
}

double TrackingWheel::get_travel(){

    // Get the change in angle of the encoder
    previous_angle = current_angle;

    //if the reverse flag of this tracking wheel is set, make 
    current_angle = encoder.position(deg) * ((reverse) ? -1.0 : 1.0);

    double theta = current_angle - previous_angle;

    // Use the radius of the weel to determine how many inches were traveled

    return (theta / 360) * diameter * PI;

}