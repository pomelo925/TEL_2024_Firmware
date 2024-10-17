/*
 * mainpp.cpp
 *
 *  Created on: Aug 5, 2024
 *      Author: pomelo925
 */


#include <mainpp.h>
#include <remote.hpp>
#include <mission.hpp>

void main_function(void){

	while(1){
		/** REMOTE MODE **/
		 if(REMOTE::MODE) REMOTE::loop();

		/** STM32 controlled via default settings or live expressions **/
		 else MISSION::loop();
	}
	return;
}
