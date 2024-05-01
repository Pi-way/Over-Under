#include "vex.h"
#include <functional>

void pre_auton(void) {

	//#######################################################################################
	//#######################################################################################
	//#######################################################################################
	//#######################################################################################
	//#######################################################################################
	//#######################################################################################
	//#######################################################################################
	
	ms.Assign("Skills", "Left", "Winpoint", new auton{
		[](){
		robot.Inertial.setHeading(45, deg);

   	 	robot.Intake.setVelocity(100, pct);

		DriveWithAnglesAndSpeed({
		{-6, {45,  100}},
		{-6, {90, 50}},
		{-30, {90, 100}}
		}, 100, true, false, 1.5);

		robot.Intake.setVelocity(0, pct);

		DriveWithAnglesAndSpeed({
		{6, {45, 100}},
		{18, {169, 25}}
		}, 100, false, false, 2);

		robot.LeftWing.set(true);

		AdjustPuncherPosition = true;

		auto tsktsk = vex::task([]()->int{
			robot.lift.setPuncherPosition(&AdjustPuncherPosition);
			return 0;
		});
		
		wait(0.5, sec);
		robot.LaunchCatapultFor(48);
		AdjustPuncherPosition = false;

		robot.LeftWing.set(false);
		}, "STARTING OVER YAY"
	});

	//#######################################################################################
	//#######################################################################################
	//#######################################################################################
	//#######################################################################################
	//#######################################################################################
	//#######################################################################################
	//#######################################################################################

	ms.Assign("Match", "Right", "Complex", new auton {
		[](){
			double st = Brain.Timer.value();
			robot.Inertial.setHeading(253.5, deg);
			robot.RightWing.set(true);
			robot.Intake.setVelocity(60, pct);

			DriveWithAnglesAndSpeed({
				{54, {248, 100}}
			}, 100, false, false, 2);

			wait(0.5, sec);
			robot.RightWing.set(false);

			DriveWithAnglesAndSpeed({
				{-45, {248, 100}},
				{-8, {-60, 20}},
			}, 100, true, false, 2.25);

			robot.Intake.setVelocity(-100, pct);

			wait(0.5, sec);

			TurnAt(165, 100, true, false, 0.8, 0);
			DriveWithAnglesAndSpeed({
				{3, {165, 100}},
				{37, {180, 100}}
			}, 100, false, false, 2);

			wait(0.5, sec);
			robot.Intake.setVelocity(60, pct);

			waitUntil(PIDsRunning == 0);

			DriveWithAngle(-24, 180, 100, true, false, 0.6);
			TurnAt(-25, 100, true, false, 0.7);
			DriveWithAnglesAndSpeed({
				{3, {-20, 50}},
				{3, {-20, 100}},
				{30, {-70, 60}},
			}, 100, false, false, 1.25);

			
			wait(0.5, sec);
			robot.RightBackWing.set(true);
			robot.Intake.setVelocity(-100, pct);
			waitUntil(PIDsRunning == 0);
			robot.RightBackWing.set(false);

			TurnAt(-90, 100, true, false, 0.25);
			DriveWithAngle(30, -90, 100, true, false, 0.6);
			DriveWithAngle(-30, -60, 75, true, false, 1);
			DriveWithAnglesAndSpeed({
				{10, {0, 100}},
				{5, {-90, 60}},
				{30, {-90, 100}},
			}, 20, true, false, 1.75);
			DriveWithAngle(-8, -90, 100, true, false, 0.5);
			DriveWithAngle(100, -90, 100, true, false, 1);
			DriveWithAngle(-12, -90, 100, true, false, 0.5);

			std::cout << "That took " << Brain.Timer.value() - st << " seconds! (:" << std::endl;
		},
		"6-ball rush."
	});

	ms.Assign("Match", "Left", "Winpoint", new auton{
		[](){
			double st = Brain.Timer.value();
			robot.Inertial.setHeading(108, deg);
			robot.Intake.setVelocity(100, pct);

			robot.LeftWing.set(true);

			DriveWithAnglesAndSpeed({
				{48.5, {113, 100}}
			}, 100, false, false, 2);

			wait(0.5, sec);
			robot.LeftWing.set(false);

			waitUntil(PIDsRunning == 0);

			TurnAt(180, 100, true, false, 0.6);

			DriveWithAnglesAndSpeed({
				{20, {180, 100}}
			}, 100, false, false, 1);

			robot.LeftWing.set(true);
			waitUntil(PIDsRunning == 0);
			robot.LeftWing.set(false);

			DriveWithAnglesAndSpeed({
				{-12, {120, 25}},
				{-16, {120, 100}},
				{-24, {-135, 100}},
				{-10, {-90, 80}},
				{-16.5, {-90, 40}} // IF PUSHES ARE TOO CLOSE TO CALL, CHANGE THIS TO 20 PCT.
			}, 100, true, false, 2.5);

			DriveWithAnglesAndSpeed({
				{12, {-135, 25}},
				{20, {-135, 30}},
				{10, {90, 50}},
			}, 100, false, false, 3);

			robot.RightBackWing.set(true);
			wait(1, sec);
			robot.Intake.setVelocity(-100, pct);
			wait(1, sec);
			robot.RightBackWing.set(false);

			DriveWithAnglesAndSpeed({
				{-5, {200, 25}}
			}, 100, true, false, 1);
			DriveWithAnglesAndSpeed({
				{2, {200, 75}},
				{28, {180, 90}}
			}, 100, true, false, 2.5);

			robot.Intake.setVelocity(0, pct);

			DriveWithAnglesAndSpeed({
				{-20, {180, 100}},
				{-10, {-150, 100}},
				{-12, {-135, 100}},
				{-5, {-155, 100}}
			}, 100, true, false, 2);

			robot.RightBackWing.set(true);
			resetWings = false;

			std::cout << "That took " << Brain.Timer.value() - st << " seconds! (:" << std::endl;

		}, "Close side rush for winpoint."
	});

	ms.Assign("Match", "Left", "Half Winpoint", new auton{
		[](){

			robot.RightBackWing.set(true);
			robot.Inertial.setHeading(-135, deg);
			wait(0.25, sec);
			TurnAt(90, 50, true, false, 1);
			robot.RightBackWing.set(false);
			DriveWithAnglesAndSpeed({
				{-5, {200, 25}}
			}, 100, true, false, 1);
			robot.Intake.setVelocity(-100, pct);
			DriveWithAnglesAndSpeed({
				{2, {200, 75}},
				{35.5, {185, 90}}
			}, 100, true, false, 2.5);
			wait(1, sec);
			robot.Intake.setVelocity(0, pct);

		}, "Close side for winpoint (safe)."
	});

	ms.Assign("Match", "Right", "Simple", new auton {
		[](){
			double st = Brain.Timer.value();
			robot.Inertial.setHeading(-106.8, deg);
			robot.Intake.setVelocity(60, pct);

			robot.RightWing.set(true);

			DriveWithAnglesAndSpeed({
				{48.5, {-107, 100}}
			}, 100, false, false, 1.8);

			wait(0.5, sec);
			robot.RightWing.set(false);
			waitUntil(PIDsRunning == 0);

			TurnAt(0, 100, true, false, 0.7);
			robot.Intake.setVelocity(-100, pct);
			wait(0.4, sec);
			TurnAt(202, 100, true, false, 1);

			robot.Intake.setVelocity(60, pct);
			DriveWithAngle(15, 202, 100, true, false, 1);
			TurnAt(0, 100, true, false, 0.8);
			DriveWithAngle(15, 0, 100, false, false, 1);
			robot.Intake.setVelocity(-100, pct);
			waitUntil(PIDsRunning == 0);
			TurnAt(142, 100, true, false, 0.9);
			DriveWithAngle(24, 142, 100, false, false, 1.2);
			robot.Intake.setVelocity(60, pct);
			waitUntil(PIDsRunning == 0);

			DriveWithAngle(-20, 0, 70, false, false, 1);

			wait(0.75, sec);
			robot.Intake.setVelocity(-100, pct);
			waitUntil(PIDsRunning == 0);

			DriveWithAngle(35, 0, 100, false, false, 1);

			ToggleBothWings();
			waitUntil(PIDsRunning == 0);
			ToggleBothWings();

			DriveWithAnglesAndSpeed({
				{-6, {-45, 100}},
				{-12, {-135, 100}},
				{-24, {-135, 100}},
				{-18, {135, 100}},
				{-12, {90, 100}},
				{-30, {90, 100}}
			}, 100, false, false, 3);

			wait(1.25, sec);
			robot.RightBackWing.set(true);
			waitUntil(PIDsRunning == 0);

			DriveWithAngle(6, 90, 100, true, false, 1);
			

			std::cout << "That took " << Brain.Timer.value() - st << " seconds! (:" << std::endl;
		},
		"Far side rush for winpoint. (4- ball)"
	});

	ms.Assign("Match", "Right", "Complex", new auton {
		[](){
			double st = Brain.Timer.value();
			robot.Inertial.setHeading(-106.8, deg);
			robot.Intake.setVelocity(60, pct);

			robot.RightWing.set(true);

			DriveWithAnglesAndSpeed({
				{48.5, {-110, 100}}
			}, 100, false, false, 1.8);

			wait(0.5, sec);
			robot.RightWing.set(false);
			waitUntil(PIDsRunning == 0);

			TurnAt(0, 100, true, false, 0.7);
			robot.Intake.setVelocity(-100, pct);
			wait(0.4, sec);
			TurnAt(202, 100, true, false, 1);

			robot.Intake.setVelocity(60, pct);
			DriveWithAngle(15, 202, 100, true, false, 0.9);
			TurnAt(0, 100, true, false, 0.8);
			DriveWithAngle(15, 0, 100, false, false, 0.9);
			robot.Intake.setVelocity(-100, pct);
			waitUntil(PIDsRunning == 0);
			TurnAt(142, 100, true, false, 0.9);
			DriveWithAngle(24, 142, 100, false, false, 1.1);
			robot.Intake.setVelocity(60, pct);
			waitUntil(PIDsRunning == 0);

			DriveWithAngle(-18, 0, 100, true, false, 1.1);

			robot.Intake.setVelocity(-100, pct);

			robot.Intake.setVelocity(-100, pct);
			ToggleBothWings();
			wait(0.2, sec);
			DriveWithAngle(35, 0, 100, true, false, 1);
			ToggleBothWings();

			DriveWithAnglesAndSpeed({
				{-1, {0, 100}},
				{-3, {85, 50}}
			}, 100, true, false, 1);
			DriveWithAnglesAndSpeed({
				{15, {85, 100}},
				{6, {70, 50}},
				{6, {70, 100}},
				{48, {-45, 100}},
				{24, {-90, 100}}
			}, 100, false, false, 2.5);

			wait(1.25, sec);
			robot.RightBackWing.set(true);

			waitUntil(PIDsRunning == 0);
			robot.RightBackWing.set(false);
			DriveWithAngle(-1000, -60, 100, true, false, 10);
			

			std::cout << "That took " << Brain.Timer.value() - st << " seconds! (:" << std::endl;
		},
		"Far side rush 5-ball."
	});

	ms.Assign("Match", "Right", "Winpoint", new auton {
		[](){
			double st = Brain.Timer.value();
			robot.Inertial.setHeading(-180, deg);

			robot.Intake.setVelocity(100, pct);
			wait(0.75, sec);
			DriveWithAngle(5, 180, 100, true, false, 0.6);

			DriveWithAnglesAndSpeed({
				{-19, {180, 100}},
				{-22, {135, 50}},
				{-34, {90, 100}}
			}, 100, false, false, 2.25);

			wait(0.5, sec);
			robot.LeftBackWing.set(true);
			wait(1, sec);
			robot.LeftBackWing.set(false);
			waitUntil(PIDsRunning == 0);

			robot.LeftBackWing.set(false);
			DriveWithAngle(4, 180, 100, true, false, 0.5);

			TurnAt(180, 100, true, false, 0.3);
			TurnAt(-80, 100, true, false, 0.5);

			robot.Intake.setVelocity(-100, pct);

			DriveWithAngle(-12, -80, 100, true, false,  0.5);
			DriveWithAngle(24, -80, 100, true, false, 1);

			
			resetWings = false;
			std::cout << "That took " << Brain.Timer.value() - st << " seconds! (:" << std::endl;
		},
		"Far side 4-ball winpoint (plus touch)."
	});
	
	ms.SetTestAutonomous("Match", "Left", "Winpoint");

	Calibrate(true);

	while(ms.should_update && (Competition.isFieldControl() || Competition.isCompetitionSwitch())){
		ms.Update();
		wait(20, msec);
	}
}