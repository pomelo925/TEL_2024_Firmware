/*
 * interrupt.cpp
 *
 *  Created on: Oct 16, 2024
 *      Author: pomelo925
 */

#include <interrupt.hpp>
#include <dc.hpp>

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if (htim->Instance == TIM6) {

    /* DC MOTOR：底盤  */
    DC_ChassisL.close_loop_pwm_output();
    DC_ChassisR.close_loop_pwm_output();
    DC_SwivelL.close_loop_pwm_output();
    DC_SwivelR.close_loop_pwm_output();
    

    /* DC MOTOR：發射砲  */
    DC_LauncherL1.open_loop_pwm_output(1000, GPIO_PIN_SET);
    DC_LauncherL2.open_loop_pwm_output(1000, GPIO_PIN_SET);
    DC_LauncherR1.open_loop_pwm_output(1000, GPIO_PIN_SET);
    DC_LauncherR2.open_loop_pwm_output(1000, GPIO_PIN_SET);


    /* STEPPER：填彈 */
    // 防止積分飽和
    

    /* SERVO：板機 */


    /* SERVO：仰角 */
  }
}

