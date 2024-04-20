#include "vex.h"

bool resetWings = true;

int Calibrate(bool inert)
{
    if(inert){
        robot.Inertial.calibrate();
        robot.LiftInertial.calibrate();
    }

    robot.BL.spin(fwd);
    robot.BR.spin(fwd);
    robot.FL.spin(fwd);
    robot.FR.spin(fwd);
    robot.BBL.spin(fwd);
    robot.BBR.spin(fwd);
    robot.Intake.spin(fwd);
    robot.BigElevate.spin(fwd);
    robot.SmallElevate.spin(fwd);

    robot.BL.setBrake(coast);
    robot.BR.setBrake(coast);
    robot.FL.setBrake(coast);
    robot.FR.setBrake(coast);
    robot.BBL.setBrake(coast);
    robot.BBR.setBrake(coast);
    robot.Intake.setBrake(coast);
    robot.BigElevate.setBrake(hold);
    robot.SmallElevate.setBrake(hold);

    robot.BL.setVelocity(0, pct);
    robot.BR.setVelocity(0, pct);
    robot.FL.setVelocity(0, pct);
    robot.FR.setVelocity(0, pct);
    robot.BBL.setVelocity(0, pct);
    robot.BBR.setVelocity(0, pct);
    robot.Intake.setVelocity(0, pct);
    robot.BigElevate.setVelocity(0, pct);
    robot.SmallElevate.setVelocity(0, pct);

    robot.Cata.spin(forward);
    robot.Cata.setBrake(coast);
    robot.Cata.setVelocity(0, pct);

    if(resetWings){
        robot.RightWing.set(false);
        robot.LeftWing.set(false);
        robot.RightBackWing.set(false);
        robot.LeftBackWing.set(false);
    }

    robot.LiftRatchet.set(false);

    return 0;
};