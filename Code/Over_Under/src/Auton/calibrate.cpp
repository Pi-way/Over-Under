#include "vex.h"

int Calibrate()
{
    //odom.Calibrate();
    robot.Inertial.calibrate();

    robot.BL.spin(fwd);
    robot.BR.spin(fwd);
    robot.FL.spin(fwd);
    robot.FR.spin(fwd);
    robot.BBL.spin(fwd);
    robot.BBR.spin(fwd);
    robot.Intake.spin(fwd);

    robot.BL.setVelocity(0, pct);
    robot.BR.setVelocity(0, pct);
    robot.FL.setVelocity(0, pct);
    robot.FR.setVelocity(0, pct);
    robot.BBL.setVelocity(0, pct);
    robot.BBR.setVelocity(0, pct);
    robot.Intake.setVelocity(0, pct);

    robot.BL.setBrake(coast);
    robot.BR.setBrake(coast);
    robot.FL.setBrake(coast);
    robot.FR.setBrake(coast);
    robot.BBL.setBrake(coast);
    robot.BBR.setBrake(coast);
    robot.Intake.setBrake(coast);

    robot.Cata.setBrake(hold);
    robot.Cata.spin(forward);
    robot.Cata.setVelocity(0, pct);

    robot.RightLift.set(false);
    robot.LeftLift.set(false);
    robot.RightWing.set(false);
    robot.LeftWing.set(false);

    return 0;
};