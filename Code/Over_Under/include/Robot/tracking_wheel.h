#pragma once
using namespace vex;

class TrackingWheelObserver;

class TrackingWheel{
private:
    rotation encoder = NULL;

public:
    const bool reverse = false;
    const double diameter = NULL;

    TrackingWheel(rotation _encoder, double _diameter, bool _reverse = false);
    TrackingWheelObserver* getObserver();

    friend class TrackingWheelObserver;
};

class TrackingWheelObserver{
public:
    TrackingWheelObserver(TrackingWheel* _parent);

    void setPosition(double _position);
    double getTravel();
    double position();

    TrackingWheel* parent = nullptr;

    double zero_offset = 0;
    double previous_angle = 0;
    double current_angle = 0;
};