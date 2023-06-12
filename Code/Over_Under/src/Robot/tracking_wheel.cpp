#include "vex.h"
using namespace vex;

TrackingWheel::TrackingWheel(rotation _encoder, double _radius){

    // Initialize member vairables

    encoder = _encoder;
    radius = _radius;

    // Reset the encoder

    encoder.resetPosition();
}

double TrackingWheel::get_travel(){

    // Get the change in angle of the encoder

    double theta = encoder.angle(deg);

    // Use the radius of the weel to determine how many inches were traveled

    return (theta / 180.0) * radius;

}