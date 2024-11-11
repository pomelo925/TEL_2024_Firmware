/*
 * interrupt.cpp
 *
 *  Created on: Oct 16, 2024
 *      Author: pomelo925
 */

#include <interrupt.hpp>
#include <global.hpp>
#include <stepper.hpp>
#include <dc.hpp>
#include <chassis.hpp>
#include <servo.hpp>


extern TIM_HandleTypeDef htim14;
extern TIM_HandleTypeDef htim16;

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
	}


	// TIM16 : 1000hz
	if(htim->Instance == TIM16){
		/* DC MOTOR：底盤  */
		Chassis.run(Global.Chassis_X_Speed, Global.Chassis_Theta_Speed);
		DC_ChassisL.close_loop_pwm_output();
		DC_ChassisR.close_loop_pwm_output();
		DC_SwivelL.close_loop_pwm_output();
		DC_SwivelR.close_loop_pwm_output();

		DC_LauncherL1.open_loop_pwm_output(Global.launcher_L1_DUTY);
		DC_LauncherL2.open_loop_pwm_output(Global.launcher_L2_DUTY);
		DC_LauncherR1.open_loop_pwm_output(Global.launcher_R1_DUTY);
		DC_LauncherR2.open_loop_pwm_output(Global.launcher_R2_DUTY);

		/* SERVO：板機 */

	}


	// TIM24 : 1000hz
	if(htim->Instance == TIM24 ){
		/* SERVO：仰角 */
		ServoElevatorR.open_loop_step();
		ServoElevatorL.open_loop_step();
	}
	return;
}

