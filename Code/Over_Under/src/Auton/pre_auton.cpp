#include "vex.h"
#include <functional>

void pre_auton(void) {

	ms.Assign(
		"Match", "Right", "Half Winpoint",
		new auton{
			[](){
				robot.Inertial.setHeading(0, deg);
				robot.Inertial.setHeading(45, deg);
				robot.IntakeCylinder.set(true);
				DriveWithAngles({{20, 65},{30, 180}});
				DriveWithAngle(20, 180, 100, true, false, 1.0);

			},
			"Scores 1 alliance tri-ball, and touches barrier / elevation-bar"
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

	ms.SetTestAutonomous("Match", "Right", "Half Winpoint");

	while(ms.should_update && (Competition.isFieldControl() || Competition.isCompetitionSwitch())){
		ms.Update();
		wait(20, msec);
	}

}