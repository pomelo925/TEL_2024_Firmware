/*
 * stepper.cpp
 *
 *  Created on: Oct 20, 2024
 *      Author: pomelo925
 */

#include <stepper.hpp>

STEPPER Stepper_L(GPIOG, GPIO_PIN_0, GPIOG, GPIO_PIN_12);
STEPPER Stepper_R(GPIOG, GPIO_PIN_11, GPIOG, GPIO_PIN_13);


/**
  * @brief 步進馬達，單步開環控制
  */
void STEPPER::open_loop_step(void){
  // 步數為 0，則不執行
  if(this->_goal_pos == 0) return;

  // 正反轉
  const bool isForward = this->_goal_pos > 0;

  if(isForward) HAL_GPIO_WritePin(this->_dirPort, this->_dirPin, GPIO_PIN_SET);
  else HAL_GPIO_WritePin(this->_dirPort,  this->_dirPin, GPIO_PIN_RESET);

  // 步進脈衝，每 1ms 產生一次脈衝
  HAL_GPIO_TogglePin(this->_stepPort, this->_stepPin);

  // 步數減少
  if(isForward) this->_goal_pos -= 1;
  else this->_goal_pos += 1;
}
