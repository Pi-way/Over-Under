#pragma once
using namespace vex;

class TrackingWheel{
public:

    TrackingWheel(rotation _encoder, double _radius);

    rotation encoder = NULL;
    double radius = NULL;

    double previous_angle = 0;
    double current_angle = 0;

    double get_travel();
};