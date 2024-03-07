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

	//#######################################################################################
	//#######################################################################################
	//#######################################################################################
	//#######################################################################################
	//#######################################################################################
	//#######################################################################################
	//#######################################################################################
	
	ms.Assign("Skills", "Left", "Winpoint", new auton{
		[](){
			clearConsole();
			odom.Calibrate(-50, -56.15, 50);
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
			DriveWithAnglesAndSpeed(true, {{11, {75, 85}}}, 100, true, false, 1);
			robot.RightWing.set(true);
			TurnAtPoint({48, -6}, true, 100, false, false, 3);
			wait(0.75, sec);

			robot.Intake.setVelocity(-100, pct);
			
			robot.LaunchCatapultFor(3);

			double startTime = Brain.Timer.value();

			vex::task ball_set_up2 = vex::task([]()->int{
				robot.Cata.setBrake(coast);
				robot.RightWing.set(false);
				wait(0.5, sec);
				robot.Cata.setBrake(coast);
				return 0;
			});

			DriveWithAnglesAndSpeed(true, {{20, {-10, 100}}, {7, {180, 10}}}, 100, true, false, 1.25);
			robot.Intake.setVelocity(0, pct);
			DriveWithAnglesAndSpeed({{-40, {200, 80}}}, 100, true, false, 1.25);
			robot.Inertial.setHeading(180, deg);
			DriveWithAngle(4, 180, 100, true, false, 0.8);
			TurnAt(97, 100, true, false, 0.8);
			ToggleRightWing();
			DriveWithAnglesAndSpeed({{-80, {110, 75}}}, 100, true, false, 2.5);
			ToggleRightWing();
			TurnAt(0, 100, true, false, 0.8);

			DriveWithAnglesAndSpeed({
				{-25, {6.75, 100}},

				{-5, {90, 18}},
				{-10, {90, 50}},

				{-5, {128, 25}},
				{-5, {128, 100}},
				{-5, {180, 50}},

				{-15, {180, 50}}, //Align In Alley
				{-60, {180, 90}}, //Alley Push
				{-6, {180, 12}}, //Slow Down

				{-6, {-142, 45}},
				{-20.75, {-142, 50}},

				{-5, {-90, 45}},
				{-40, {-90, 100}},
			}, 100, false, false, 7);

			wait(1, sec);
			robot.MiniWing.set(true);
			wait(1.5, sec);

			waitUntil(PIDsRunning < 1);

			robot.MiniWing.set(false);

			DriveWithAngle(8, -90, 100, true, false, 1);
			DriveWithAngle(-15, -90, 100, true, false, 1);

			//Done with pushes

			DriveWithAnglesAndSpeed({
				{10, {-135, 45}},
				{4, {-135, 75}},
				{7, {-135, 100}},
			},100, true, false, 1.25);

			auto dist = GetDistance(odom.x, odom.y, 0, 36);

			DriveWithAnglesAndSpeed({
				{-10, {0, 40}},
				{-(dist - 28), {0, 100}},
				{-12, {-90, 20}},
				{-15, {-90, 100}}
			}, 100, false, false, 4);

			wait(3, sec);
			robot.RightWing.set(true); robot.LeftWing.set(true);
			waitUntil(PIDsRunning < 1);

			//Now we pivot to face goal for first push

			DriveWithAnglesAndSpeed({
				{-7, {-164, 30}},
				{-12, {-164, 100}}
			}, 100, true, false, 4);

			waitUntil(PIDsRunning < 1);

			// "pulse"
			DriveWithAngle(-15, -164, 100, true, false, 0.7, 0);
			DriveWithAngle(8, -165, 100, true, false, 0.7, 0);
			DriveWithAngle(-15, -166, 100, true, false, 0.7, 0);
			DriveWithAngle(8, -167, 100, true, false, 0.7, 0);
			DriveWithAngle(-15, -168, 100, true, false, 0.7, 0);


			DriveWithAnglesAndSpeed({
				{20, {-170, 100}},
				{5, {-90, 20}}
			}, 100, true, false, 1.5);

			DriveWithAnglesAndSpeed({
				
			})

			// ############# OLD CODE ###################

			// DriveWithAnglesAndSpeed({{
			// 	{7, {-90, 100}},
			// 	{5, {-135, 30}},
			// 	{8, {-135, 100}},
			// }}, 100, true, false, 2.5);

			// TurnAt(-50, 100, true, false, 0.8);
			

			// DriveWithAnglesAndSpeed({
			// 	{-10, {-45, 100}},
			// 	{-5, {-15, 25}},
			// 	{-12, {-15, 95}},

			// 	//Sharp Turn to face back to goal
			// 	{-3.25, {-45, 27}},
			// 	{-3.25, {-90, 27}},
			// 	{-3.25, {-135, 27}},
			// 	{-3.25, {180, 27}},

			// 	{-40, {180, 95}},
			// }, 100, false, false, 4);

			// wait(1, sec);

			// robot.LeftWing.set(true);

			// waitUntil(PIDsRunning < 1);

			// robot.LeftWing.set(false);

			// //Drive Away from first push, facing match load station
			// DriveWithAngle(26, 180, 95, true, false, 1.3);
			// TurnAt(-90, 100, true, false, 0.8);

			// robot.Intake.setVelocity(-100, pct);

			// DriveWithAnglesAndSpeed({
			// 	{-10, {-90, 95}},
			// 	{-5, {-135, 25}},
			// 	{-5, {-135, 90}},
			// 	{-5, {135, 25}},
			// 	{-5, {135, 75}},
			// }, 100, false, false, 2.5);

			// wait(0.5, sec);
			// robot.LeftWing.set(true);
			// waitUntil(PIDsRunning < 1);
			// TurnAt(150, 100, true, false, 0.8);
			// robot.LeftWing.set(false);
			// robot.RightWing.set(true);

			// DriveWithAnglesAndSpeed({
			// 	{-4, {150, 80}},
			// 	{-3, {170, 30}},
			// 	{-40, {180, 80}},
			// }, 100 , true, false, 2.5);

			// robot.RightWing.set(false);

			// robot.Intake.setVelocity(0, pct);

			// DriveWithAngle(26, 180, 95, true, false, 1.25);

			// TurnAtPoint({24, 0}, false, 100, true, false, 1);

			// robot.LeftWing.set(true);
			// robot.RightWing.set(true);

			// double angleBro = robot.Inertial.heading(deg);

			// DriveWithAnglesAndSpeed({
			// 	{-5, {angleBro, 95}},
			// 	{-3, {180, 25}},
			// 	{-7, {180, 75}},
			// 	{-30, {180, 100}},
			// }, 100, true, false, 2);

			// robot.Intake.setVelocity(-100, pct);

			// robot.LeftWing.set(false);
			// robot.RightWing.set(false);
			// DriveWithAngle(12, 180, 100, true, false, 1);


			std::cout << Brain.Timer.time(sec) - startTime << std::endl;
		}, "Skills V2"
	});

	ms.Assign("Skills", "Left", "Half Winpoint", new auton{
		[](){
			double st = Brain.Timer.value();
						odom.Calibrate(-50, -56.15, 50);
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
			DriveWithAnglesAndSpeed(true, {{11, {75, 85}}}, 100, true, false, 1);
			TurnAtPoint({48, -6}, true, 100, false, false, 3);
			wait(0.75, sec);
			robot.RightWing.set(true);
			robot.LaunchCatapultFor((Competition.isFieldControl() || Competition.isCompetitionSwitch()) ? 50 : 1);

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
			DriveWithAngle(10, 45, 25, true, false, 1.25, 0);
			TurnAtPoint({24, -36}, false, 100, true, false, 1.25, 0.125);

			angle = robot.Inertial.heading(deg);

			DriveWithAnglesAndSpeed({{-23, {angle, 85}}, {-12, {90, 25}}, {-32, {90, 85}}}, 100, true, false, 3.5);
			TurnAtPoint({48, -6}, false, 100, true, false, 1.25, 0.125);

			angle = robot.Inertial.heading(deg);

			DriveWithAngle(-50, angle, 100, true, false, 1.25); //StraightPushNoWings
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

			std::cout << Brain.Timer.time(sec) - st << std::endl;

		}, "Skillz."
	});

	//#######################################################################################
	//#######################################################################################
	//#######################################################################################
	//#######################################################################################
	//#######################################################################################
	//#######################################################################################
	//#######################################################################################

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

	ms.Assign("Match", "Right", "Complex", new auton{
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

			DriveWithAngle(-26, 180, 100, true, false, 1);
			DriveWithAnglesAndSpeed({{-22, {135, 75}}}, 100, false, false, 0.9, 0);
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
			DriveWithAnglesAndSpeed({{-6, {90, 50}}, {-18, {90, 100}}}, 100, true, false, 1, 0);
			DriveWithAngle(8, 90, 100, true, false, 0.7, 0);
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
			DriveWithAngle(24, -90, 100, true, false, 0.8, 0);
			DriveWithAngle(-10, -90, 100, true, false, 0.6, 0);
			TurnAt(-165, 100, true, false, 0.7, 0);

			robot.Intake.setVelocity(56, pct);
			DriveWithAngle(46.25, -170, 90, true, false, 1.3, 0);
			TurnAt(-21.5, 100, false, false, 0.65, 0);
			wait(0.5, sec);
			robot.Intake.setVelocity(0, pct);
			DriveWithAngle(12, -21.5, 100, false, false, 0.9, 0);
			wait(0.5, sec);
			robot.Intake.setVelocity(-100, pct);
			TurnAt(-135, 100, true, false, 0.9, 0);
			DriveWithAngle(20, -135, 100, false, false, 0.9, 0);
			robot.Intake.setVelocity(100, pct);
			wait(0.75, sec);
			robot.Intake.setVelocity(0, pct);
			DriveWithAnglesAndSpeed({{12, {180, 50}}}, 100, true, false, 0.5, 0);
			robot.RightWing.set(true);
			robot.LeftWing.set(true);
			DriveWithAngle(-40, 180, 100, false, false, 1.1, 0);
			wait(0.5, sec);
			robot.Intake.setVelocity(-50, pct);
			printf(" %3.3f\n", Brain.Timer.time(sec) - st);
			DriveWithAngle(12, 180, 100, true, false, 0.75, 0);
			printf(" %3.3f\n", Brain.Timer.time(sec) - st);

		},
		"yeeeet."
	});

	ms.Assign("Match", "Left", "Winpoint", new auton {
		[](){
			double st = Brain.Timer.time(sec);
			robot.Inertial.setHeading(101.81, deg);
			wait(0.125, sec);
			auto ODOM = new vex::task(updateOdometry);

			robot.Intake.setVelocity(60, pct);
			robot.Intake.spin(fwd);
			DriveWithAngle(48, 100, 91, true, false, 2);
			robot.Intake.setVelocity(5, pct);

			DriveWithAnglesAndSpeed({
				{-7, {101.81, 90}}, 
				{-5, {180, 25}},
				{-20, {180, 90}},
				{-18, {90, 25}}
			}, 100, false, false, 2.25);

			wait(2, sec);
			robot.Intake.setVelocity(0, pct);

			waitUntil(PIDsRunning < 1);
			robot.LeftWing.set(true);

			TurnAt(0, 75, true, true, 1);
			

			DriveWithAnglesAndSpeed({
				{1.45, {-45, 19}},
				{1.45, {-90, 19}},
				{1.45, {-135, 19}},
				{1.45, {180, 19}},
				{49, {180, 75}},
			}, 100, false, false, 4);
			robot.LeftWing.set(false);

			wait(2, sec);
			robot.Intake.setVelocity(-100, pct);

			DriveWithAngle(-20, 180, 100, true, false, 1.25);
			DriveWithAngle(17, 180, 100, true, false, 1);

			printf(" %3.3f\n", Brain.Timer.time(sec) - st);

		},
		"The new rush... with CHEESE!"
	});
	
	ms.SetTestAutonomous("Skills", "Left", "Winpoint");

	while(ms.should_update && (Competition.isFieldControl() || Competition.isCompetitionSwitch())){
		ms.Update();
		wait(20, msec);
	}
}