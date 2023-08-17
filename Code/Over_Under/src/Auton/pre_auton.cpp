#include "vex.h"
#include <functional>

void pre_auton(void) {

	ms.Assign(
		"Match", "Right", "Half Winpoint",
		new auton{
			[](){
				DriveWithAngle(12, 0);
			},
			"Scores 1 alliance tri-ball, and touches barrier / elevation-bar"
		}
	);

	ms.SetTestAutonomous("Match", "Right", "Half Winpoint");

	while(ms.should_update && (Competition.isFieldControl() || Competition.isCompetitionSwitch())){
		ms.Update();
		wait(20, msec);
	}

}