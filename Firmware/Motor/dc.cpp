/*
 * dc.cpp
 *
 *  Created on: Oct 16, 2024
 *      Author: pomelo925
 */

#include "stm32h7xx.h"
#include "stm32h7xx_hal.h"
#include "stm32h7xx_hal_adc.h" 
#include "stm32h7xx_hal_adc_ex.h"
#include "stm32h7xx_hal_dma.h"

#include <dc.hpp>

extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim5;
extern TIM_HandleTypeDef htim8;
extern TIM_HandleTypeDef htim15;
extern TIM_HandleTypeDef htim23;
extern TIM_HandleTypeDef htim24;

extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc2;


DC DC_LauncherL1(&htim15, TIM_CHANNEL_1, GPIOA, GPIO_PIN_15);
DC DC_LauncherL2(&htim23, TIM_CHANNEL_2, GPIOC, GPIO_PIN_13);
DC DC_LauncherR1(&htim8, TIM_CHANNEL_3, GPIOD, GPIO_PIN_2);
DC DC_LauncherR2(&htim8, TIM_CHANNEL_1, GPIOD, GPIO_PIN_3);

DC DC_SwivelL(&htim23, TIM_CHANNEL_1, GPIOD, GPIO_PIN_1,
		          GPIOD, GPIO_PIN_0);

DC DC_SwivelR(&htim24, TIM_CHANNEL_4, GPIOG, GPIO_PIN_10,
		          GPIOE, GPIO_PIN_7);

DC DC_ChassisL(&htim8, TIM_CHANNEL_2, GPIOC, GPIO_PIN_10,
                &htim5, 40.f, 0.f, 0.f,
                1024.f, 26.f, 0.001f);
                
DC DC_ChassisR(&htim8, TIM_CHANNEL_4, GPIOC, GPIO_PIN_11,
                &htim3, 40.f, 0.f, 0.f,
                1024.f, 26.f, 0.001f);

uint32_t _swivel_adc_L[3], _swivel_adc_R[3];


/**
 * @brief 初始化
 */
void DC::init(void){
  /* 啟動 Encoder Timer */
  HAL_TIM_Encoder_Start(this->getEncTimer(), TIM_CHANNEL_ALL);
  /* 啟動 PWM 輸出 Timer */
  HAL_TIM_PWM_Start_IT(this->getPwmTimer(), this->getPwmChannel());
  /* 關閉煞車 */
  HAL_GPIO_WritePin(this->_breakPort, this->_breakPin, GPIO_PIN_SET);

  return;
}


/**
 * @brief Swivel 初始化
 */
void DC::swivel_init(void){
  /* 啟動 ADC x DMA*/
  HAL_ADC_Start_DMA(&hadc1, _swivel_adc_R, 1);
  HAL_ADC_Start_DMA(&hadc2, _swivel_adc_L, 1);
  return;
}


/**
 * @brief 直接輸出固定占空比
 * @param 占空比（單位：%）
 */
void DC::open_loop_pwm_output(void){
  // 設置電機旋轉方向
	if(_duty >= 0) HAL_GPIO_WritePin(this->getDirPort(), this->getDirPin(), GPIO_PIN_SET);
	else HAL_GPIO_WritePin(this->getDirPort(), this->getDirPin(), GPIO_PIN_RESET);

  // PWM 輸出
	__HAL_TIM_SET_COMPARE(this->getPwmTimer(), this->getPwmChannel(), (abs(_duty) * PWM_SCALE) / 100);

  return;
}



/**
 * @brief PID 追速
 */
void DC::close_loop_pwm_output(){
  // 狀態更新
	DC::update_current_wheel_speed();
	DC::update_target_PWM();

  // 重置計數器
	__HAL_TIM_SetCounter(this->getEncTimer(), 0);

	// 設置電機旋轉方向
	HAL_GPIO_WritePin(this->getDirPort(), this->getDirPin(), this->getDir());
	
  // 輸出 PWM
	__HAL_TIM_SET_COMPARE(this->getPwmTimer(), this->getPwmChannel(),
                          static_cast<uint32_t>(fabs(this->_target_PWM)));

  return;
}


void DC::close_loop_adc_pwm_output(uint8_t idx){
  // 1. 更新當前位置
  if(idx == 0) this->_current_pos_adc = _swivel_adc_L[1];
  else this->_current_pos_adc = _swivel_adc_R[1];

  // 2. 計算誤差
  const uint32_t error = _target_pos_adc - _current_pos_adc;
  const uint32_t duty = ( error > 0) ? 5 : -5;
  this->set_duty(duty);
  // const float adjusted_duty = (duty > 0 ? 1 : -1) * (100 - abs(swivel_duty));    // 處理占空比反向邏輯（馬達端問題）

  if(error < 1){
    HAL_GPIO_WritePin(this->_breakPort, this->_breakPin, GPIO_PIN_RESET); // 煞車
    return;
  }

  // 3. 設置電機旋轉方向
  HAL_GPIO_WritePin(this->_breakPort, this->_breakPin, GPIO_PIN_RESET); // 解除煞車

	if(_duty >= 0) HAL_GPIO_WritePin(this->getDirPort(), this->getDirPin(), GPIO_PIN_SET);
	else HAL_GPIO_WritePin(this->getDirPort(), this->getDirPin(), GPIO_PIN_RESET);

  // 3. PWM 輸出
	__HAL_TIM_SET_COMPARE(this->getPwmTimer(), this->getPwmChannel(), (abs(_duty) * PWM_SCALE) / 100);

  return;
}


/**
 * @brief 設定目標輪速 (rad/s)
 * @param speed 目標輪速
 */
void DC::set_target_wheel_speed(float speed){
  _target_wheel_speed = speed;
  return;
};


/**
 * @brief 根據脈衝，更新當前輪速 (rad/s)
 */
void DC::update_current_wheel_speed() {
  _current_pulse = __HAL_TIM_GetCounter(this->_encTimer);
  const float speed = (float)(this->_current_pulse / 4 / this->_encoder_res / this->_sr_ratio / this->_interval) * 2.f * M_PI;
  this->_current_wheel_speed = this->_direction ? speed : -speed;
  return;
}


/**
 * @brief 更新目標 PWM 輸出
 */
void DC::update_target_PWM(void) {
  this->_error = this->_target_wheel_speed - this->_current_wheel_speed;

  static int count = 0;
  if (count < 6)  this->_integral = 0.f;
  else  this->_integral += (this->_error * this->_interval);
  count ++;

  // 防止積分飽和_error
  if (this->_integral < -this->PWM_SCALE / this->_ki) this->_integral = -this->PWM_SCALE / this->_ki;
  else if (this->_integral > this->PWM_SCALE / this->_ki) this->_integral = this->PWM_SCALE / this->_ki;
  

  const float derivative = (this->_error - this->_previous_error) / this->_interval;
  this->_previous_error = this->_error;

  // 計算 PID 輸出

  this->_target_PWM = this->_error * this->_kp + this->_integral * this->_ki + derivative * this->_kd;

  // 限制 PWM 輸出
  if (this->_target_PWM < -PWM_SCALE) this->_target_PWM = -PWM_SCALE;
  else if (this->_target_PWM > PWM_SCALE) this->_target_PWM = PWM_SCALE;
  
  // 更新方向
  _direction = (this->_target_PWM >= 0);
}


/**
 * @brief launcher 馬達靜止
 *
 */
void DC::freeze_launcher(void){
  DC_LauncherL1.set_duty(0);
  DC_LauncherL2.set_duty(0);
  DC_LauncherR1.set_duty(0);
  DC_LauncherR2.set_duty(0);

  return;
}
