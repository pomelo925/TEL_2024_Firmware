/*
 * mainpp.cpp
 *
 *  Created on: Aug 5, 2024
 *      Author: pomelo925
 */


#include <mainpp.h>
#include <remote.hpp>
#include <dc.hpp>
#include <interrupt.hpp>
#include <servo.hpp>
#include <turret.hpp>

void main_function(void){
	/* 初始化 */
	DC_ChassisL.init();
	DC_ChassisR.init();
	DC_SwivelL.init();
	DC_SwivelR.init();
	DC_LauncherL1.init();
	DC_LauncherL2.init();
	DC_LauncherR1.init();
	DC_LauncherR2.init();
	Remote.init();

//	DC::swivel_init();
	SERVO::init();
	INTERRUPT::init();
	TURRET::init();



	/* 無限迴圈 */
	while(1){}
	return;
}
