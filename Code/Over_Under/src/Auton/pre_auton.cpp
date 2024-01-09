#include "vex.h"
#include <functional>

void pre_auton(void) {

	ms.Assign("Match", "Left", "Half Winpoint", new auton {
		[](){
			double st = Brain.Timer.time(sec);
			robot.Inertial.setHeading(41.5, deg);

			robot.RightWing.set(true);

			vex::task ball_set_up = vex::task([]()->int{
				robot.Intake.setVelocity(100, pct);
				robot.Intake.spin(fwd);
				wait(0.25, sec);
				robot.Intake.setVelocity(0, pct);
				robot.LaunchCatapult();
				robot.Cata.setVelocity(50, pct);
				//robot.Cata.spinFor(-1, turns);
				robot.Cata.setBrake(coast);
				wait(0.5, sec);
				robot.Cata.setBrake(hold);
				return 0;
			});
			vex::task wingdeploy = vex::task([]()->int{
				double st = Brain.Timer.time(sec);
				waitUntil(Brain.Timer.time(sec) - st > 14.9);
				robot.RightWing.set(true);
				return 0;
			});
			
			wait(0.25, sec);
			TurnAt(-45,75);

			robot.RightWing.set(false);

			DriveWithAnglesAndSpeed({{27, {-65, 60}}, {15, {180, 70}}}, 100, true, false, 4555, 1);
			DriveWithAngle(1.25, 180, 45);
			robot.Intake.setVelocity(-100, pct);
			wait(0.5, sec);
			robot.Intake.setVelocity(0, pct);
			DriveWithAngle(-8, 180);
			TurnAt(90);
			robot.RightWing.set(true);
			wait(0.5, sec);
			DriveWithAngle(-2, 135);
			TurnAt(180, 20);
			DriveWithAnglesAndSpeed({{-40, {180, 95}}}, 100, true, false, 1.4);
			robot.RightWing.set(false);
			TurnAt(-100);
			DriveWithAngle(-22, -100);
			TurnAt(-73);
			robot.RightWing.set(true);
			printf(" %3.3f\n", Brain.Timer.time(sec) - st);

		},
		"De-scores the match load tri-ball, then Scores the alliance tri-ball, and pushes two green tri-balls over to the far side. After... It touches the bar."
	});

	ms.Assign("Match", "Left", "Complex", new auton {
		[](){
			double st = Brain.Timer.time(sec);
			robot.Inertial.setHeading(41.5, deg);

			robot.RightWing.set(true);

			vex::task ball_set_up = vex::task([]()->int{
				robot.Intake.setVelocity(100, pct);
				robot.Intake.spin(fwd);
				wait(0.25, sec);
				robot.Intake.setVelocity(0, pct);
				robot.LaunchCatapult();
				robot.Cata.setVelocity(50, pct);
				//robot.Cata.spinFor(-1, turns);
				robot.Cata.setBrake(coast);
				wait(0.5, sec);
				robot.Cata.setBrake(hold);
				return 0;
			});
			
			wait(0.25, sec);
			TurnAt(-45,75);

			robot.RightWing.set(false);

			DriveWithAnglesAndSpeed({{27, {-65, 60}}, {15, {180, 70}}}, 100, true, false, 4555, 1);
			DriveWithAngle(1.25, 180, 45);
			robot.Intake.setVelocity(-100, pct);
			wait(0.5, sec);
			robot.Intake.setVelocity(0, pct);
			DriveWithAngle(-6.5, 180);
			TurnAt(90);
			robot.RightWing.set(true);
			wait(0.5, sec);
			DriveWithAngle(-2, 135);
			TurnAt(180, 20);
			DriveWithAnglesAndSpeed({{-40, {180, 95}}}, 100, true, false, 1.4);
			robot.RightWing.set(false);
			TurnAt(-45);
			DriveWithAngle(-50, -45, 100, true, 2.5);
			TurnAt(-20);
			robot.RightWing.set(true);
			robot.RightLift.set(true);
			robot.LeftLift.set(true);
			printf(" %3.3f\n", Brain.Timer.time(sec) - st);

		},
		"De-scores the match load tri-ball, then Scores the alliance tri-ball, and pushes two green tri-balls over to the far side. After... It lines up to shoot"
	});

	ms.Assign("Skills", "Left", "Half Winpoint", new auton{
		[](){

			double st = Brain.Timer.time(sec);
			robot.Inertial.setHeading(41.5, deg);

			vex::task ball_set_up = vex::task([]()->int{
				robot.Intake.setVelocity(100, pct);
				robot.Intake.spin(fwd);
				wait(0.25, sec);
				robot.Intake.setVelocity(0, pct);
				return 0;
			});

			DriveWithAnglesAndSpeed({{-5, {40, 95}}, {-25, {90, 80}}}, 100, true, false, 2);
			
			DriveWithAnglesAndSpeed({{20, {7, 90}}});
			TurnAt(-20);
			robot.RightWing.set(true);
			robot.LaunchCatapultFor(47);
			robot.RightWing.set(false);
			DriveWithAnglesAndSpeed({{6, {-135, 60}}});
			DriveWithAnglesAndSpeed({{-8, {-150, 90}}, {-70, {180, 95}}, {-10, {135, 25}}, {-23, {135, 95}}, {-5, {90, 25}}, {-19, {90, 95}}}, 100, true, false, 6);
			DriveWithAngle(7, 90);
			DriveWithAngle(-20, 90, 100, true, false, 1.25);

		}, "Skillz."
	});

	ms.Assign("Match", "Right", "Simple", new auton{
		[](){
			double st = Brain.Timer.time(sec);
			robot.Inertial.setHeading(180, deg);

			vex::task ball_set_up = vex::task([]()->int{
				robot.Intake.spin(fwd);
				robot.Intake.setVelocity(35, pct);
				wait(1.125, sec);
				robot.Intake.setVelocity(0, pct);
				return 0;
			});

			wait(0.5, sec);

			DriveWithAngle(-24, 180);
			DriveWithAnglesAndSpeed({{-24, {135, 50}}}, 100, false);
			wait(0.5, sec);
			robot.LeftWing.set(true);
			wait(0.75, sec);
			robot.LeftWing.set(false);
			DriveWithAngle(-24, 90, 100, true, false, 1.25);
			DriveWithAngle(10, 90);
			TurnAt(-90);
			robot.Intake.setVelocity(-100, pct);
			DriveWithAngle(24, -90, 100, true, false, 1.25);
			DriveWithAngle(-5, -90);
			printf(" %3.3f\n", Brain.Timer.time(sec) - st);

		},
		"yeeeet."
	});
	
	ms.SetTestAutonomous("Match", "Right", "Simple");

	while(ms.should_update && (Competition.isFieldControl() || Competition.isCompetitionSwitch())){
		ms.Update();
		wait(20, msec);
	}
}