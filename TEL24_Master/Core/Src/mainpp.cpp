/*
 * mainpp.cpp
 *
 *  Created on: Oct 16, 2024
 *      Author: pomelo925
 */


#include <mainpp.h>

void main_function(void){
	// DC_MOTOR::init();

	while(1){
		/** MANUAL MODE : activate functions via live expressions **/
		// if(MANUAL::MODE) MANUAL::loop();

		/** ROS MODE : STM32 Will be controlled by ROS **/
		// else ROS1::spinCycle();
	}
	return;
}
