#pragma once
using namespace vex;

class TrackingWheel{
public:

    TrackingWheel(rotation _encoder, double _diameter, bool _reverse = false);

    rotation encoder = NULL;
    double diameter = NULL;

    bool reverse = false;

    double previous_angle = 0;
    double current_angle = 0;

    double get_travel();
};