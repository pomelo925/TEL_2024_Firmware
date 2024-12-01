/*
 * interrupt.cpp
 *
 *  Created on: Oct 16, 2024
 *      Author: pomelo925
 */

#include <interrupt.hpp>
#include <stepper.hpp>
#include <dc.hpp>
#include <chassis.hpp>
#include <servo.hpp>
#include <turret.hpp>

extern TIM_HandleTypeDef htim6;
extern TIM_HandleTypeDef htim16;

extern ADC_HandleTypeDef hadc2;

INTERRUPT Interrupt;

void INTERRUPT::init(void){
	HAL_TIM_Base_Start_IT(&htim6);
	HAL_TIM_Base_Start_IT(&htim16);
	return;
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	// TIM6 : 5000hz
	if(htim->Instance == TIM6){
		/* STEPPER：填彈 */
		Stepper_L.open_loop_step();
		Stepper_R.open_loop_step();
		ServoElevatorR.open_loop_step();
		ServoElevatorL.open_loop_step();
	}

	// TIM16 : 1000hz
	if(htim->Instance == TIM16){
		/* DC MOTOR：底盤  */
		DC_ChassisL.close_loop_pwm_output();
		DC_ChassisR.close_loop_pwm_output();

		DC_LauncherL1.open_loop_pwm_output();
		DC_LauncherL2.open_loop_pwm_output();
		DC_LauncherR1.open_loop_pwm_output();
		DC_LauncherR2.open_loop_pwm_output();

//		DC_SwivelL.close_loop_adc_pwm_output(0);
//		DC_SwivelR.close_loop_adc_pwm_output(1);
		DC_SwivelL.open_loop_pwm_output();
		DC_SwivelR.open_loop_pwm_output();

		/* Turret：更新發射填彈的外部時鐘計算 */
		Turret.update_timer();
		ServoTriggerL.tremble();
		ServoTriggerR.tremble();

	}

	return;
}

