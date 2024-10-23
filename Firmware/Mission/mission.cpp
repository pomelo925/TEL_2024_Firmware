/*
 * mission.cpp
 *
 *  Created on: Oct 16, 2024
 *      Author: pomelo925
 */

#include <mission.hpp>
#include <chassis.hpp>
#include <global.hpp>

MISSION Mission;

void MISSION::mission_1(void){
	Chassis.run(GLOBAL::Chassis_X_Speed, GLOBAL::Chassis_Theta_Speed);
	return;
}


void MISSION::mission_2(void){
	return;
}

void MISSION::run(uint16_t Mode){
	switch(Mode){
		case 1: MISSION::mission_1(); break;
		case 2: MISSION::mission_2(); break;
		default: return;
	}
}
