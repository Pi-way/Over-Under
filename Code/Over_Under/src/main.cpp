/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       caleb                                                     */
/*    Created:      6/6/2023, 11:54:08 AM                                     */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "vex.h"

using namespace vex;

competition Competition;

Odometry odom = Odometry();

void pre_auton(void) {

}

void autonomous(void) {

}

void newLine() {

  Brain.Screen.newLine();

}

void print_location() {

  Brain.Screen.clearScreen();
  Brain.Screen.setCursor(1, 1);

  Brain.Screen.print("X: ");
  Brain.Screen.print(odom.x);
  newLine();

  Brain.Screen.print("Y: ");
  Brain.Screen.print(odom.y);
  newLine();

  Brain.Screen.print("H: ");
  Brain.Screen.print(odom.h);
  newLine();
}

int odom_task_function() {

  int count = 0;

  while (true){
    odom.Update();

    if (count == 5) { print_location(); count = 0; }
    count ++;

    wait(20, msec);
  }

  return 0;
}

void usercontrol(void) {

  wait(0.25, sec);
  odom.inert.calibrate();
  wait(3, sec);

  task odom_task = task(odom_task_function);

  std::ofstream file("data.txt");

  file << "coordinates = [";

  Brain.Timer.clear();
  while (Brain.Timer.value() < 15) {
    wait(50, msec);

    file << "(" << odom.x << ", " << odom.y << "),";

  }

  file << "]";
  file.close();
  wait(3, sec);

  Brain.Screen.setFillColor("#555555");

  while(1) {
    wait(100, msec);
  }
  
}

int main() {

  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  pre_auton();

  while (true) {
    wait(100, msec);
  }
}
