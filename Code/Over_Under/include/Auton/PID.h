/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       PID.h                                                     */
/*    Author:       Team 98548J (Ace)                                         */
/*    Created:      7-14-2022                                                 */
/*    Description:  Header file for PID class                                 */
/*                                                                            */
/*----------------------------------------------------------------------------*/
#pragma once
/** 
  * @brief A class for tuneable PIDs
  * @param p Tune this value for proportional.
  * @param i Tune this value for integral.
  * @param d Tune this value for derivative.
  * @param r Tune this for the ramp-up speed.
  * @param integral_cap Sets a limit on the effect of integral (% of motor power).
  * @param speed_cap Sets a maximum output speed for the PID.
  * @param timeout The maximum amount of time the PID will run for.
  * @param not_done A pointer that will be set to false when the PID is done.
  * @param settle_time How many sec the PID will allow itself to 'settle' before stopping.
  */
class PID
{
private:
  void DataLogCheck();
  void DataLogEnd();
public:

  double P;
  double I;
  double D;
  double R;

  double IntegralCap;
  double SpeedCap;

  double RampUp = 0;
  double Error = 0;
  double PreviousError = 0;

  double PIDSpeed;
  double SmartSpeed;

  double Integral = 0;
  double Derivative = 0;
  bool HasRampedUp = false;

  bool HasReachedEnd = false;
  double TimeReachedEnd = 0;
  double Timeout;
  double SettleTime;
  double Time = 0;

  bool RanOnce = false;
  bool *NotDone;

  PID(double p, double i, double d, double r, double integral_cap, double speed_cap, bool *not_done, double timeout, double settle_time);
    
  /**
    * @brief Updates the PID and related tasks.
    * @returns A value representing the output speed/power of the PID.
    * @param error Distance left to travel.
    * @param dt Time elapsed since last call (Delta time).
    */ 
  double Update(double error, double dt);

};

/**
  * @brief A function used to get the sign of a number.
  * @returns -1 if the number is negative, 1 if the number is positive.
  * @param number The number in question.
  */ 
int GetSign(double number);