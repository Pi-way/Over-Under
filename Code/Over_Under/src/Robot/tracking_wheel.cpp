#include "vex.h"
using namespace vex;

TrackingWheel::TrackingWheel(rotation _encoder, double _diameter, bool _reverse) : reverse(_reverse), diameter(_diameter){

    encoder = _encoder;

    // Reset the encoder
    encoder.resetPosition();
}

TrackingWheelObserver* TrackingWheel::getObserver(){
    return
}

// double TrackingWheel::get_travel(){

//     // Get the change in angle of the encoder
//     previous_angle = current_angle;

//     //if the reverse flag of this tracking wheel is set, make 
//     current_angle = encoder.position(deg) * ((reverse) ? -1.0 : 1.0);

//     double theta = current_angle - previous_angle;

//     // Use the radius of the weel to determine how many inches were traveled

//     return (theta / 360) * diameter * PI;

// }