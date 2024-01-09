#pragma once
using namespace vex;

class TrackingWheel{
private:
    rotation encoder = NULL;

public:
    const bool reverse = false;
    const double diameter = NULL;

    TrackingWheel(rotation _encoder, double _diameter, bool _reverse = false);
    TrackingWheelObserver* getObserver();
};

class TrackingWheelObserver{
public:
    TrackingWheelObserver(TrackingWheel _parent);

    void setPosition(double _position);
    double getTravel();
    double position();

    double zero_offset = 0;
    double previous_angle = 0;
    double current_angle = 0;
};