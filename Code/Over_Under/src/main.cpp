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

int main() {
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  Brain.Screen.setFont(vex::fontType::prop20);
  
  pre_auton();
  
  while (true) {
    wait(20, msec);
  }
}