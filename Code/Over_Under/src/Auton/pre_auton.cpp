#include "vex.h"
#include <functional>

void pre_auton(void) {

	ms.Assign(
		"Match", "Right", "Half Winpoint",
		new auton{
			[](){
				robot.Inertial.setHeading(45, deg);
				robot.IntakeCylinder.set(true);
				DriveWithAngles({{20, 65},{30, 180}});
				DriveWithAngle(20, 180, 100, true, false, 1.0);

			},
			"Scores 1 alliance tri-ball, and touches barrier / elevation-bar"
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
				DriveWithAngle(-10, 180);
				TurnAt(0);
				DriveWithAngles({{15, 110},{20, 90}});
				DriveWithAngle(5, 90, 25, true, false, 0.5, 0);


				double end_time = Brain.Timer.systemHighResolution();
				printf(" %3.3f\n", (end_time - start_time) / 1000000.0);


			},
			"Removes tri-ball from match load zone, scores some balls"
		}
	);

	ms.Assign(
		"Skills", "Right", "Winpoint",
		new auton{
			[](){
				robot.Inertial.setHeading(0, deg);
				DriveWithAngles({{55, -17},{15, -90},{30, -180},{30, -90},{12,0}});
				//DriveWithAngle(65, 0);
				// printf("hey");
				// ToggleBothWings();
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