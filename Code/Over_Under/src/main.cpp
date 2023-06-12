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

void pre_auton(void) {

}

void autonomous(void) {

}



void usercontrol(void) {
  
  while(1)
  {
    Brain.Screen.clearLine(1);
    Brain.Screen.setCursor(1,1);
    Brain.Screen.print(enncoder.angle(deg));

    wait(20, msec);
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
