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
    robot.RightIntake.spin(fwd);
    robot.LeftIntake.spin(fwd);

    robot.BL.setVelocity(0, pct);
    robot.BR.setVelocity(0, pct);
    robot.FL.setVelocity(0, pct);
    robot.FR.setVelocity(0, pct);
    robot.BBL.setVelocity(0, pct);
    robot.BBR.setVelocity(0, pct);
    robot.RightIntake.setVelocity(0, pct);
    robot.LeftIntake.setVelocity(0, pct);

    robot.BL.setBrake(coast);
    robot.BR.setBrake(coast);
    robot.FL.setBrake(coast);
    robot.FR.setBrake(coast);
    robot.BBL.setBrake(coast);
    robot.BBR.setBrake(coast);
    robot.RightIntake.setBrake(coast);
    robot.LeftIntake.setBrake(coast);

    robot.Cata.setBrake(hold);
    robot.Cata.spin(forward);
    robot.Cata.setVelocity(0, pct);

    robot.IntakeCylinder.set(false);
    robot.LeftWingCylinder.set(false);
    robot.RightWingCylinder.set(false);
    robot.WingReleaseCylinder.set(false);

    return 0;
};