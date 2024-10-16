/*
 * mainpp.cpp
 *
 *  Created on: Aug 5, 2024
 *      Author: pomelo925
 */


#include <mainpp.h>

void main_function(void){
	// DC_MOTOR::init();

	while(1){
		/** REMOTE MODE **/
		// if(MANUAL::MODE) MANUAL::loop();

		/** ROS MODE : STM32 Will be controlled by ROS **/
		// else ROS1::spinCycle();
	}
	return;
}
