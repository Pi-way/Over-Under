#include "vex.h"
#include <functional>

//HI AYLA THIS IS A COMMENT CHEESBURGER

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

				Intake(spin(fwd);)
				Intake(setVelocity(-100, pct);)
				
				DriveWithAngles({{-17, -145}});
				DriveWithAngle(6, -145, 100, true, false, 0.5);
				ToggleLeftWing();
				Intake(stop();)
				
				
				TurnAt(45);

				DriveWithAnglesAndSpeed({{-12, {20, 85}}, {-12, {90, 90}}});
				DriveWithAngle(-6, 90, 100, true, false, 1);
				
				DriveWithAngle(10, 0, 100, true, false);

				TurnAt(-22, 100, true, false, 0.75);
				DriveWithAngle(16, -22);
				TurnAt(0, 100, true, false, 0.75);

				//DriveWithAngles({{15, 0},{33, -135}}, 100, false);
				// wait(0.5, sec);
				// 
				// TurnAt(180);
				// Intake(spin(forward);)
				// Intake(setVelocity(-100, pct);)
				// wait(0.25, sec);
				// DriveWithAngle(20, 180, 100, true, false, 0.75, 0);
				// Intake(stop();)
				// DriveWithAngle(-10, 180);
				// DriveWithAngle(20, 180, 100, true, false, 0.75, 0);
				// DriveWithAngle(-30, 180);
				// TurnAt(90);

				bool not_at_pitch = true;

				DriveWithAngle(100, 0, 100, false, false, 1000, 0, &not_at_pitch);

				waitUntil(robot.Inertial.pitch(deg) > 25);

				not_at_pitch = false;

				robot.RightWingCylinder.set(true);
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
				ToggleLeftWing();

				Intake(spin(fwd);)
				Intake(setVelocity(-100, pct);)
				
				DriveWithAngles({{-17, -145}});
				DriveWithAngle(6, -145, 100, true, false, 0.5);
				ToggleLeftWing();
				Intake(stop();)

			},
			"Removes tri-ball from match load zone."
		}
	);

	ms.Assign(
		"Match", "Left", "Complex",
		new auton{
			[](){

				double start_time = Brain.Timer.systemHighResolution();
				robot.IntakeCylinder.set(true);
				robot.LaunchC();

				Intake(stop();)
				Intake(setVelocity(100, pct);)

				robot.Inertial.setHeading(-45, deg);
				
				DriveWithAnglesAndSpeed({{42, {-90, 80}}, {4, {180, 50}}});
				Intake(spinFor(-1.25, turns, false);)
				DriveWithAngle(20, 180, 100, true, false, 1);
				DriveWithAngle(-6, 180);
				TurnAt(90);

				DriveWithAnglesAndSpeed({{12, {90, 75}}, {36, {135, 50}}});
				TurnAt(152);
				robot.LaunchCatapult();


				double end_time = Brain.Timer.systemHighResolution();
				printf(" %3.3f\n", (end_time - start_time) / 1000000.0);


			},
			"Scores An alliance ball, and lines up to match load"
		}
	);

		ms.Assign(
		"Match", "Right", "Complex",
		new auton{
			[](){

				double start_time = Brain.Timer.systemHighResolution();

				robot.Inertial.setHeading(-180, degrees);

				robot.IntakeCylinder.set(true);
				robot.LaunchC();

				Intake(spin(forward);)
				Intake(setVelocity(100, pct);)

				DriveWithAngle(8, 180);

				Intake(setVelocity(0, pct);)

				DriveWithAnglesAndSpeed({{-5, {180, 95}}, {-30,{135, 75}}, {-24, {90, 85}}}, 100, false);

				wait(1, sec);

				ToggleLeftWing();
				
				waitUntil(PIDsRunning == 0);
				
				DriveWithAngle(12, 150, 100, false);
				wait(0.5, sec);
				ToggleLeftWing();

				DriveWithAnglesAndSpeed({{-12, {150, 80}}}, 100, true, false, 0.75);

				TurnAt(90, 100, true, false, 0.5);
				DriveWithAngle(6, 90, 100, true, false, 0.4);

				
				DriveWithAngle(-12, 90, 100, true, false, 1);
				

				DriveWithAngle(12, 90, 100, true, false, 1);

				TurnAt(-90);

				Intake(spin(forward);)
				Intake(setVelocity(-100, pct);)

				TurnAt(90);

				DriveWithAngle(-12, 90, 100, true, false, 1);

				
				
				double end_time = Brain.Timer.systemHighResolution();
				printf(" %3.3f\n", (end_time - start_time) / 1000000.0);
				

			},
			"Scores A lot-a balls"
		}
	);

	ms.Assign(
		"Skills", "Right", "Simple",
		new auton{
			[](){
				double start_time = Brain.Timer.systemHighResolution();

				robot.Inertial.setHeading(152, deg);
				robot.IntakeCylinder.set(true);

				Intake(spinFor(-1, turns, false);)

				robot.LaunchCatapultFor(49);

				task cata_task = task([]()->int{robot.Cata.setBrake(hold); robot.LaunchC(); return 0;});

				DriveWithAnglesAndSpeed({{-12, {112, 80}}, {-12, {90, 80}}, {-40, {90, 100}}, {-22, {0, 50}}}, 100, true, false, 4.5);
				DriveWithAngle(40, 0, 100, true, false, 3, 0);
				DriveWithAngle(-20, 0, 100, true, false, 1);

				robot.IntakeCylinder.set(false);
				ToggleBothWings();
				
				Intake(spin(fwd);)
				Intake(setVelocity(-100, pct);)
				DriveWithAnglesAndSpeed({{15, {25, 95}}, {15, {0, 95}}}, 100, true, false, 1.5);
				ToggleBothWings();
				DriveWithAngle(-12, 15, 100, true, false, 0.75);
				ToggleBothWings();
				DriveWithAnglesAndSpeed({{15, {25, 95}}, {15, {0, 95}}}, 100, true, false, 1.5);
				ToggleBothWings();
				DriveWithAnglesAndSpeed({{-3, {0, 95}}, {-50, {-90, 90}}},100, true, false, 3);
				DriveWithAngle(12, -90, 80);
				TurnAt(10);
				ToggleBothWings();
				DriveWithAnglesAndSpeed({{15, {-25, 95}}, {15, {0, 95}}}, 100, true, false, 1.5);
				ToggleBothWings();
				DriveWithAngle(-23, -15, 100, true, false, 0.75);
				ToggleBothWings();
				DriveWithAnglesAndSpeed({{15, {-25, 95}}, {15, {0, 95}}}, 100, true, false, 1.5);
				ToggleBothWings();
				DriveWithAngle(-10, 0);
				TurnAt(90);
				ToggleRightWing();
				DriveWithAnglesAndSpeed({{12, {80, 95}}, {30, {12, 95}}, {5, {45, 95}}}, 100, true, false, 1.5);

				double end_time = Brain.Timer.systemHighResolution();
				printf(" %3.3f\n", (end_time - start_time) / 1000000.0);

			},
			"This skills auto better work, or else I will quit my job. Oh wait, I already did. XD"
		}
	);

	ms.SetTestAutonomous("Match", "Left", "Half Winpoint");

	while(ms.should_update && (Competition.isFieldControl() || Competition.isCompetitionSwitch())){
		ms.Update();
		wait(20, msec);
	}

}