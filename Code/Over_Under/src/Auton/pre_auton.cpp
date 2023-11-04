#include "vex.h"
#include <functional>

void pre_auton(void) {

	ms.Assign(
		"Match", "Right", "Half Winpoint",
		new auton{
			[](){

				double start_time = Brain.Timer.systemHighResolution();

				robot.Inertial.setHeading(180, deg);
				robot.IntakeCylinder.set(true);
				DriveWithAngle(20, 135);
				ToggleRightWing();
				DriveWithAngles({{-17, 145}});
				DriveWithAngles({{15, 0},{33, 135}}, 100, false);
				wait(0.5, sec);
				ToggleRightWing();
				TurnAt(180);
				Intake(spin(forward);)
				Intake(setVelocity(-100, pct);)
				wait(0.25, sec);
				DriveWithAngle(20, 180, 100, true, false, 0.75, 0);
				Intake(stop();)
				DriveWithAngle(-10, 180);
				DriveWithAngle(20, 180, 100, true, false, 0.75, 0);
				DriveWithAngle(-10, 180);
				TurnAt(-90);
				DriveWithAngles({{35, 0}});
				bool not_at_pitch = true;

				DriveWithAngle(45, 0, 100, false, false, 1000, 0, &not_at_pitch);

				waitUntil(robot.Inertial.pitch(deg) > 25);

				not_at_pitch = false;

				robot.LeftWingCylinder.set(true);
				robot.WingReleaseCylinder.set(true);

				wait(0.25, sec);

				robot.WingReleaseCylinder.set(false);

				double end_time = Brain.Timer.systemHighResolution();
				printf(" %3.3f\n", (end_time - start_time) / 1000000.0);

			},
			"Removes a tri-ball from the match load zone, scores an alliance ball, and touches the bar."
		}
	);

	ms.Assign(
		"Match", "Left", "Half Winpoint",
		new auton{
			[](){

				double start_time = Brain.Timer.systemHighResolution();

				robot.Inertial.setHeading(180, deg);
				robot.IntakeCylinder.set(true);
				DriveWithAngle(20, -135);
				ToggleLeftWing();
				DriveWithAngles({{-17, -145}});
				DriveWithAngles({{15, 0},{33, -135}}, 100, false);
				wait(0.5, sec);
				ToggleLeftWing();
				TurnAt(180);
				Intake(spin(forward);)
				Intake(setVelocity(-100, pct);)
				wait(0.25, sec);
				DriveWithAngle(20, 180, 100, true, false, 0.75, 0);
				Intake(stop();)
				DriveWithAngle(-10, 180);
				DriveWithAngle(20, 180, 100, true, false, 0.75, 0);
				DriveWithAngle(-30, 180);
				TurnAt(90);

				bool not_at_pitch = true;

				DriveWithAngle(45, 90, 100, false, false, 1000, 0, &not_at_pitch);

				waitUntil(robot.Inertial.pitch(deg) > 25);

				not_at_pitch = false;

				robot.LeftWingCylinder.set(true);
				robot.WingReleaseCylinder.set(true);

				wait(0.25, sec);

				robot.WingReleaseCylinder.set(false);

				double end_time = Brain.Timer.systemHighResolution();
				printf(" %3.3f\n", (end_time - start_time) / 1000000.0);


			},
			"Removes tri-ball from match load zone, scores an alliance ball, and touches the bar."
		}
	);

	ms.Assign(
		"Match", "Left", "Simple",
		new auton{
			[](){

				double start_time = Brain.Timer.systemHighResolution();

				robot.Inertial.setHeading(180, deg);
				robot.IntakeCylinder.set(true);
				DriveWithAngle(20, -135);

				DriveWithAngles({{-17, -145}});
				DriveWithAngles({{15, 0},{33, -135}}, 100, false);
				wait(0.5, sec);
				TurnAt(180);

				Intake(spin(forward);)
				Intake(setVelocity(-100, pct);)
				wait(0.25, sec);
				DriveWithAngle(20, 180, 100, true, false, 0.75, 0);
				Intake(stop();)
				DriveWithAngle(-10, 180);
				DriveWithAngle(20, 180, 100, true, false, 0.75, 0);
				DriveWithAngle(-10, 180);
				TurnAt(90);

				double end_time = Brain.Timer.systemHighResolution();
				printf(" %3.3f\n", (end_time - start_time) / 1000000.0);


			},
			"Removes tri-ball from match load zone, scores an alliance ball, and touches the bar."
		}
	);

	ms.Assign(
		"Skills", "Right", "Simple",
		new auton{
			[](){
				double start_time = Brain.Timer.systemHighResolution();

				robot.Inertial.setHeading(150, deg);
				robot.IntakeCylinder.set(true);

				robot.LaunchCatapultFor(48);

				task cata_task = task([]()->int{robot.Cata.setBrake(hold); robot.LaunchCatapult(); return 0;});

				TurnAt(-175, 100, true, false, 0.75, 0);
				DriveWithAngle(-31, -175, 100, true, false, 1);
				TurnAt(90, 100, true, false, 0.75);
				DriveWithAngle(-10, 90, 25, true, false, 0.5);

				wait(0.125, sec);
				robot.Inertial.setHeading(90, deg);
				robot.IntakeCylinder.set(false);
				DriveWithAngle(5, 90);
				TurnAt(0, 100, true, false, 1, 0);

				DriveWithAnglesAndSpeed({{35, {0, 100}},{24, {-90, 75}}, {24, {180, 75}}, {48, {-90, 80}}}, 100, true, true);
				TurnAt(0);
				ToggleBothWings();
				DriveWithAngle(30, 0, 90, true, false, 2);
				ToggleBothWings();
				DriveWithAngle(-20, 0, 90, true, false, 2);
				ToggleBothWings();
				DriveWithAngle(30, 0, 90, true, false, 2);
				ToggleBothWings();
				DriveWithAngle(-30, 0, 90, true, false, 2);

				double end_time = Brain.Timer.systemHighResolution();
				printf(" %3.3f\n", (end_time - start_time) / 1000000.0);

			},
			"This skills auto better work, or else I will quit my job. Oh wait, I already did. XD"
		}
	);

	ms.SetTestAutonomous("Skills", "Right", "Simple");

	while(ms.should_update && (Competition.isFieldControl() || Competition.isCompetitionSwitch())){
		ms.Update();
		wait(20, msec);
	}

}