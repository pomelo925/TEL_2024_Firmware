/*
 * mainpp.cpp
 *
 *  Created on: Aug 5, 2024
 *      Author: pomelo925
 */


#include <mainpp.h>
#include <remote.hpp>
#include <mission.hpp>
#include <dc.hpp>
#include <global.hpp>
#include <interrupt.hpp>
#include <servo.hpp>

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


	SERVO::init();
	Interrupt.init();
	Remote.init();

	ServoTriggerR.UART_send_pos(1000, 1000);


	/* 無限迴圈 */
	while(1){
		/**
		 * Mission 中會去呼叫使機構作動的 Function
		 * 這類 Function 參數一律為 GLOBAL 參數集合
		 * Remote.MODE = true 時，GLOBAL 只由 Remote 遠端傳輸決定
		 * Remote.MODE = false 時，GLOBAL 則可從 live expression 調整
		 */

		Remote.ppmHigh_to_global();
	}
	return;
}
