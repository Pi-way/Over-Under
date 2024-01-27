#include "vex.h"
using namespace vex;

TrackingWheel::TrackingWheel(rotation _encoder, double _diameter, bool _reverse) : reverse(_reverse), diameter(_diameter){

    encoder = _encoder;

    // Reset the encoder
    encoder.resetPosition();
}

TrackingWheelObserver::TrackingWheelObserver(TrackingWheel* _parent){
    parent = _parent;
}

TrackingWheelObserver* TrackingWheel::getObserver(){
    auto temp_observer = new TrackingWheelObserver(this);
    temp_observer->setPosition(0);
    return temp_observer;
}

void TrackingWheelObserver::setPosition(double _position){
    zero_offset = parent->encoder.position(deg) + _position;
}
double TrackingWheelObserver::getTravel(){
    // Get the change in angle of the encoder
    previous_angle = current_angle;

    //if the reverse flag of this tracking wheel is set, make 
    current_angle = parent->encoder.position(deg) * ((parent->reverse) ? -1.0 : 1.0);

    double theta = current_angle - previous_angle;

    // Use the radius of the wheel to determine how many inches were traveled

    return (theta / 360) * parent->diameter * PI;
}

double TrackingWheelObserver::position(){
    return ((parent->encoder.position(deg) - zero_offset) / 360.0) * parent->diameter * PI;
}