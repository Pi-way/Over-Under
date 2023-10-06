#include "vex.h"
#include <functional>

void pre_auton(void) {

	ms.Assign(
		"Match", "Right", "Half Winpoint",
		new auton{
			[](){
				robot.Inertial.setHeading(0, deg);

				DriveWithAngle(40, 0, 100, true, false, 1.75, 0);
				DriveWithAngle(-40, 0, 75, true, false, 1.75, 0);
				// TurnAt(63, 100, true, false, 2, 0);
				// DriveWithAngle(-37, 63);

			},
			"Scores 1 alliance tri-ball, and touches barrier / elevation-bar"
		}
	);

	ms.Assign(
		"Skills", "Right", "Winpoint",
		new auton{
			[](){
				DriveWithAngle(12, 0);
			},
			"This skills auto better work, or else I will quit my job. Oh wait, I already did. XD"
		}
	);

	ms.SetTestAutonomous("Skills", "Right", "Winpoint");

	while(ms.should_update && (Competition.isFieldControl() || Competition.isCompetitionSwitch())){
		ms.Update();
		wait(20, msec);
	}

}