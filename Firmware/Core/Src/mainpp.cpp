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
		/**
		 * 實際機構作動的 function 會由布林值 XX_ON 決定其狀態
		 * Remote.MODE = true 時，XX_ON 會由 Remote 傳輸
		 * Remote.MODE = false 時，XX_ON 則可從 live expression 調整
		 * XX_ON 的定義及預設值位於資料夾 /STATE。
		 */

		 if(Remote.MODE) Remote.loop();
		 else Mission.loop();
	}
	return;
}
