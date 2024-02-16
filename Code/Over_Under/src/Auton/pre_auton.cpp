#include "vex.h"
#include <functional>

int updateOdometry() {
	while(true){
		odom.Update();
		Brain.Screen.clearScreen();
		Brain.Screen.setCursor(1, 1);
		Brain.Screen.print("X: ");
		Brain.Screen.print(odom.x);
		Brain.Screen.newLine();
		Brain.Screen.print("Y: ");
		Brain.Screen.print(odom.y);
		Brain.Screen.newLine();
		Brain.Screen.print("H: ");
		Brain.Screen.print(odom.h);
		Brain.Screen.newLine();
		wait(20, msec);
	}
	return 0;
}

void pre_auton(void) {

	ms.Assign("Match", "Left", "Half Winpoint", new auton {
		[](){
			double st = Brain.Timer.time(sec);
			robot.Inertial.setHeading(46.5, deg);

			robot.RightWing.set(true);

			vex::task ball_set_up = vex::task([]()->int{
				robot.Intake.setVelocity(100, pct);
				robot.Intake.spin(fwd);
				wait(0.25, sec);
				robot.Intake.setVelocity(0, pct);
				return 0;
			});
			vex::task wingdeploy = vex::task([]()->int{
				double st = Brain.Timer.time(sec);
				waitUntil(Brain.Timer.time(sec) - st > 14.9);
				robot.RightWing.set(true);
				return 0;
			});
			
			wait(0.25, sec);
			TurnAt(-45,75,true, false, 1);

			robot.RightWing.set(false);

			DriveWithAnglesAndSpeed({{15, {-75, 60}}, {15, {180, 70}}, {3, {180, 5}}, {6, {180, 90}}}, 100, true, false, 2, 0.125);
			robot.Intake.setVelocity(-100, pct);
			wait(0.5, sec);
			robot.Intake.setVelocity(0, pct);
			DriveWithAngle(-8, 180, 100, true, false, 1);
			TurnAt(90, 100, true, false, 1);
			robot.RightWing.set(true);
			wait(0.5, sec);
			DriveWithAngle(-2, 135, 100, true, false, 0.75);
			TurnAt(180, 35, true, false, 0.8, 0);
			DriveWithAnglesAndSpeed({{-40, {180, 95}}}, 100, true, false, 1.4);
			robot.RightWing.set(false);
			wait(0.25, sec);
			TurnAt(-100, 100, true, false, 1);
			DriveWithAngle(-23.5, -100, 100, true, false, 1.125);
			TurnAt(-73, 100, true, false, 1);
			robot.RightWing.set(true);
			printf(" %3.3f\n", Brain.Timer.time(sec) - st);

		},
		"De-scores the match load tri-ball, then Scores the alliance tri-ball, and pushes two green tri-balls over to the far side. After... It touches the bar."
	});

	ms.Assign("Skills", "Left", "Half Winpoint", new auton{
		[](){
			double st = Brain.Timer.time(sec);
			odom.Calibrate(-50, -56.15, 46.27);
			auto ODOM = new vex::task(updateOdometry);
			robot.Inertial.setHeading(46.5, deg);

			vex::task ball_set_up = vex::task([]()->int{
			robot.Intake.setVelocity(100, pct);
			robot.Intake.spin(fwd);
			wait(0.25, sec);
			robot.Intake.setVelocity(0, pct);
			return 0;
			});

			DriveWithAnglesAndSpeed({{-7, {42, 85}},{-8, {90, 25}},{-30, {90, 75}}}, 100, true, false, 1.5);
			DriveWithAnglesAndSpeed({{14.5, {60, 85}}}, 100, true, false, 1);
			TurnAtPoint({48, -6}, true, 100, false, false, 3);
			wait(0.75, sec);
			robot.RightWing.set(true);
			robot.LaunchCatapultFor(Competition.isFieldControl() ? (50) : (1));

			vex::task ball_set_up2 = vex::task([]()->int{
				robot.Cata.setBrake(coast);
				robot.RightWing.set(false);
				wait(0.5, sec);
				robot.Cata.setBrake(coast);
				return 0;
			});

			TurnAtPoint({-40, -67}, false, 100, true, false, 1.25, 0.125);

			double angle = robot.Inertial.heading(deg);

			robot.Intake.setVelocity(-100, pct);

			DriveWithAnglesAndSpeed({{-30, {angle, 100}}, {-12, {188, 20}}, {-45, {188, 95}}, {-10, {135, 25}}, {-11, {135, 95}}, {-10, {90, 25}}, {-24, {90, 95}}}, 100, true, false, 5);
			DriveWithAngle(9, 90, 100, true, false, 0.75);
			DriveWithAngle(-14, 90, 100, true, false, 0.75);
			DriveWithAngle(3, 45, 25, true, false, 1.25, 0);
			TurnAtPoint({24, -36}, false, 100, true, false, 1.25, 0.125);

			angle = robot.Inertial.heading(deg);

			DriveWithAnglesAndSpeed({{-23, {angle, 85}}, {-12, {90, 25}}, {-32, {90, 85}}}, 100, true, false, 3.5);
			TurnAtPoint({48, -6}, false, 100, true, false, 1.25, 0.125);

			angle = robot.Inertial.heading(deg);

			DriveWithAngle(-50, angle, 100, true, false, 1.25); //StriaghtPushNoWings
			DriveWithAnglesAndSpeed({{6.5, {angle, 95}}, {9, {-90, 7}}, {28, {-90, 95}}}, 100, true, false, 2.5);//DriveAwayFar

			TurnAtPoint({48, 0}, false, 100, true, false, 1.25, 0.125);
			angle = robot.Inertial.heading(deg);

			ToggleBothWings();

			DriveWithAngle(-50, angle, 100, true, false, 1.5);//PushWithWingsFar

			ToggleBothWings();

			DriveWithAnglesAndSpeed({{6.5, {180, 95}}, {9, {90, 12}}, {33, {90, 95}}}, 100, true, false, 2.5);//DriveAwayClose
			TurnAtPoint({48, 0}, false, 100, true, false, 1.25, 0.125);
			angle = robot.Inertial.heading(deg);

			ToggleBothWings();

			DriveWithAngle(-50, angle, 100, true, false, 1.25);//PushWithWingsClose
			
			ToggleBothWings();
			DriveWithAngle(12, angle, 100, true, false, 1);

			// TurnAtPoint({12, 0}, true, 100, true, false, 1.25, 0.125);
			// angle = robot.Inertial.heading(deg);

			// DriveWithAngle(30, angle, 100, true, false, 1.25);

			// TurnAtPoint({48, 0}, false, 100, true, false, 1.25, 0.125);
			// angle = robot.Inertial.heading(deg);

			// ToggleBothWings();

			// DriveWithAngle(-50, angle, 100, true, false, 1);


			std::cout << Brain.Timer.time(sec) - st << std::endl;

		}, "Skillz."
	});

	

	ms.Assign("Match", "Right", "Simple", new auton{
		[](){
			double st = Brain.Timer.time(sec);
			robot.Inertial.setHeading(180, deg);

			vex::task ball_set_up = vex::task([]()->int{
				robot.Intake.spin(fwd);
				robot.Intake.setVelocity(75, pct);
				wait(1, sec);
				robot.Intake.setVelocity(0, pct);
				return 0;
			});

			wait(0.25, sec);
			DriveWithAngle(3, 180, 100, true, false, 0.3);

			DriveWithAngle(-25, 180, 100, true, false, 1);
			DriveWithAnglesAndSpeed({{-22, {135, 50}}}, 100, false, false, 0.9);
			vex::task ball_set_up_2 = vex::task([]()->int{
				robot.Cata.setBrake(coast);
				wait(1, sec);
				robot.Cata.setBrake(coast);
				return 0;
			});
			wait(0.125, sec);
			robot.LeftWing.set(true);
			wait(0.75, sec);
			robot.LeftWing.set(false);
			wait(0.125, sec);
			DriveWithAnglesAndSpeed({{-6, {90, 50}}, {-18, {90, 100}}}, 100, true, false, 1.2);
			DriveWithAngle(8, 90, 100, true, false, 0.7);
			RightDrive(setVelocity(-100, pct);)
			LeftDrive(setVelocity(100, pct);)
			RightDrive(spin(fwd);)
			LeftDrive(spin(fwd);)
			wait(0.4, sec);
			TurnAt(-90, 100, false, true, 0.5, 0, true); 
			TurnPID.HasRampedUp = true;
			TurnPID.RampUp = 1000;

			waitUntil(PIDsRunning < 1);

			robot.Intake.setVelocity(-100, pct);
			wait(0.25, sec);
			DriveWithAngle(24, -90, 100, true, false, 0.8);
			DriveWithAngle(-8, -90, 100, true, false, 0.6);
			TurnAt(-168, 100, true, false, 0.7);

			robot.Intake.setVelocity(56, pct);
			DriveWithAngle(46.25, -170, 90, true, false, 1.3);
			TurnAt(-21.5, 100, false, false, 0.65);
			wait(0.5, sec);
			robot.Intake.setVelocity(0, pct);
			DriveWithAngle(12, -21.5, 100, false, false, 0.9);
			wait(0.5, sec);
			robot.Intake.setVelocity(-100, pct);
			TurnAt(-135, 100, true, false, 0.9);
			DriveWithAngle(20, -135, 100, false, false, 0.9);
			robot.Intake.setVelocity(100, pct);
			wait(0.75, sec);
			robot.Intake.setVelocity(0, pct);
			DriveWithAnglesAndSpeed({{12, {180, 50}}}, 100, true, false, 0.5);
			robot.RightWing.set(true);
			robot.LeftWing.set(true);
			DriveWithAngle(-40, 180, 100, false, false, 1.1);
			wait(0.5, sec);
			robot.Intake.setVelocity(-50, pct);
			printf(" %3.3f\n", Brain.Timer.time(sec) - st);
			DriveWithAngle(12, 180, 100, true, false, 0.75);
			printf(" %3.3f\n", Brain.Timer.time(sec) - st);

		},
		"yeeeet."
	});

	ms.Assign("Match", "Left", "Winpoint", new auton {
		[](){
			double st = Brain.Timer.time(sec);
			odom.Calibrate(-31.71, -52.67 + 4.0, 90);
			wait(0.125, sec);
			auto ODOM = new vex::task(updateOdometry);

			
			

			DriveWithAngle(-47.75, 90, 85, true, false, 1.5, 0);
			ToggleLeftWing();
			wait(0.25, sec);
			DriveWithAnglesAndSpeed({{4, {180, 12.5}}}, 100, false, false, 1, 0);
			wait(0.25, sec);
			ToggleLeftWing();
			wait(4, sec);
			TurnAtPoint({0, -16}, false, 100, true, false, 1);
			double heading = robot.Inertial.heading(deg);
			ToggleBothWings();
			vex::task ball_set_up = vex::task([]()->int{
				wait(0.125, sec);
				robot.Intake.setVelocity(100, pct);
				robot.Intake.spin(fwd);
				wait(0.25, sec);
				robot.Intake.setVelocity(0, pct);
				return 0;
			});
			DriveWithAngle(-48, heading, 100, true, false, 1.75);

			const double distt = GetDistance(odom.x, odom.y, -26, -15);
			std::cout << "Distance (:" << distt << std::endl;
	
			ToggleBothWings();
			DriveWithAngle(distt + 6, 180, 100, false, false, 1);

			wait(0.25, sec);

			robot.Intake.spin(fwd);
			robot.Intake.setVelocity(-100, pct);
			wait(1.75, sec);
			robot.Intake.setVelocity(0, pct);

			DriveWithAnglesAndSpeed({{-6, {-90, 15}}, {-12, {-45, 15}}, {-10, {-45, 100}}, {-4, {-90, 15}}, {-8, {-90, 100}}, {-15, {20, 12}}}, 100, true, false, 3);
			ToggleRightWing();

			resetWings = false;

			printf(" %3.3f\n", Brain.Timer.time(sec) - st);

		},
		"Disrupt."
	});

		ms.Assign("Match", "Right", "Complex", new auton {
		[](){
			double st = Brain.Timer.time(sec);
			odom.Calibrate(30, -56, -90 - 19);
			wait(0.125, sec);
			auto ODOM = new vex::task(updateOdometry);

			vex::task ball_set_up_2 = vex::task([]()->int{
				robot.Intake.spin(fwd);
				robot.Intake.setVelocity(100, pct);
				return 0;
			});

			DriveWithAnglesAndSpeed({{45, {-90 - 10, 80}}}, 100, true, false, 1.5);

			robot.Intake.setVelocity(0, pct);

			DriveWithAngle(-30, -90 - 19, 100, true, false, 1);
			TurnAt(-20, 100, true, false, 1);

			robot.Intake.setVelocity(-100, pct);

			wait(0.75, sec);

			TurnAt(-145, 100, true, false, 1);
			robot.Intake.setVelocity(100, pct);
			DriveWithAngle(30, -145, 100, true, false, 1.25);

			
			printf(" %3.3f\n", Brain.Timer.time(sec) - st);

		},
		"Disrupt."
	});
	
	ms.SetTestAutonomous("Match", "Right", "Simple");

	while(ms.should_update && (Competition.isFieldControl() || Competition.isCompetitionSwitch())){
		ms.Update();
		wait(20, msec);
	}
}