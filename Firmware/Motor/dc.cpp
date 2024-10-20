/*
 * motor.cpp
 *
 *  Created on: Oct 16, 2024
 *      Author: pomelo925
 */

#include <dc.hpp>


extern TIM_HandleTypeDef htim8;
extern TIM_HandleTypeDef htim23;

DC DC_ChassisL(GPIOA, GPIO_PIN_14, &htim8, TIM_CHANNEL_1);
DC DC_ChassisR(GPIOA, GPIO_PIN_15, &htim8, TIM_CHANNEL_2);

DC DC_SwivelL(GPIOC, GPIO_PIN_10, &htim8, TIM_CHANNEL_3);
DC DC_SwivelR(GPIOC, GPIO_PIN_11, &htim8, TIM_CHANNEL_4);

DC DC_LauncherL1(GPIOD, GPIO_PIN_0, &htim23, TIM_CHANNEL_1);
DC DC_LauncherL2(GPIOD, GPIO_PIN_1, &htim23, TIM_CHANNEL_2);
DC DC_LauncherR1(GPIOD, GPIO_PIN_2, &htim23, TIM_CHANNEL_3);
DC DC_LauncherR2(GPIOD, GPIO_PIN_3, &htim23, TIM_CHANNEL_4);


/**
 * @brief 馬達初始化
 */
void DC::init(void){
  /* 啟動對應 Timer */
	HAL_TIM_Encoder_Start(_htim, TIM_CHANNEL_ALL);

  return;
}


/**
 * @brief 直接輸出固定占空比
 */
void DC::open_loop_pwm_output(uint16_t duty, GPIO_PinState set){
  // 設置電機旋轉方向
	HAL_GPIO_WritePin(this->getDirPort(), this->getDirPin(), set);

  // PWM 輸出
	__HAL_TIM_SET_COMPARE(this->getHtim(), this->getChannel(), duty);

  return;
}
