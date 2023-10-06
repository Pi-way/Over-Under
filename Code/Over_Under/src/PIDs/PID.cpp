/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       PID.cpp                                                   */
/*    Author:       Team 98548J (Ace)                                         */
/*    Created:      7-14-2022                                                 */
/*    Description:  Source file for PID class                                 */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "vex.h"

int GetSign(double number)
{
    return (number < 0) ? -1 : 1;
}

PID::PID(double p, double i, double d, double r, double integral_cap, double speed_cap, bool *not_done, double timeout, double settle_time)
{
    P = p;
    I = i;
    D = d;
    R = r;

    IntegralCap = integral_cap;
    SpeedCap = speed_cap;
    Timeout = timeout;
    SettleTime = settle_time;

    NotDone = not_done;
}

double PID::Update(double error, double dt)
{   
    
    Time += dt;
    PreviousError = Error;
    Error = error;
    Integral += error * dt;
    Derivative = (Error - PreviousError) / dt;


    if(std::abs(Integral) * I > IntegralCap)
    {
        Integral = (IntegralCap * GetSign(Error)) / I;
    }

    PIDSpeed = Error * P + Integral * I + Derivative * D;

    if(!HasRampedUp)
    {
        RampUp += (R * dt) * GetSign(Error);
        if(std::abs(RampUp) < std::abs(PIDSpeed))
        {
            SmartSpeed = RampUp;
        }
        else
        {   
            HasRampedUp = true;
            SmartSpeed = PIDSpeed;
        }
    }
    else
    {
        SmartSpeed = PIDSpeed;
    }

    if(std::abs(SmartSpeed) > SpeedCap)
    {
        SmartSpeed = SpeedCap * GetSign(SmartSpeed);
    }

    if(GetSign(Error) != GetSign(PreviousError) && RanOnce == true && HasReachedEnd == false)
    {
        HasReachedEnd = true;
        TimeReachedEnd = Time;
    }

    if(HasReachedEnd == true && Time - TimeReachedEnd > SettleTime)
    {
        *NotDone = false;
    }

    if(Time > Timeout)
    {
        *NotDone = false;
    }

    RanOnce = true;
    return SmartSpeed;
}
