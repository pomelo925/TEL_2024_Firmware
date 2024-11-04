/*
 * interrupt.cpp
 *
 *  Created on: Oct 16, 2024
 *      Author: pomelo925
 */

#include <interrupt.hpp>
#include <global.hpp>
#include <dc.hpp>
#include <chassis.hpp>

extern TIM_HandleTypeDef htim12;

INTERRUPT Interrupt;

void INTERRUPT::init(void){
	HAL_TIM_Base_Start_IT(&htim12);
	return;
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if(htim->Instance == TIM16){
		/* DC MOTOR：底盤  */
		Chassis.run(Global.Chassis_X_Speed, Global.Chassis_Theta_Speed);
		DC_ChassisL.close_loop_pwm_output();
		DC_ChassisR.close_loop_pwm_output();
		DC_SwivelL.close_loop_pwm_output();
		DC_SwivelR.close_loop_pwm_output();

		/* STEPPER：填彈 */
		// 防止積分飽和


		/* SERVO：板機 */


		/* SERVO：仰角 */
		return;
	}

	if(htim->Instance == TIM12){
	// TIM freq: 1000hz
		DC_LauncherL1.open_loop_pwm_output(Global.launcher_L1_DUTY);
		DC_LauncherL2.open_loop_pwm_output(Global.launcher_L2_DUTY);
		DC_LauncherR1.open_loop_pwm_output(Global.launcher_R1_DUTY);
		DC_LauncherR2.open_loop_pwm_output(Global.launcher_R2_DUTY);

		return;
	}
}

