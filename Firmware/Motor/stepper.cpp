/*
 * stepper.cpp
 *
 *  Created on: Oct 20, 2024
 *      Author: pomelo925
 */

#include <stepper.hpp>

STEPPER Stepper_L(GPIOD, GPIO_PIN_0, GPIOG, GPIO_PIN_12);
STEPPER Stepper_R(GPIOG, GPIO_PIN_11, GPIOA, GPIO_PIN_13);

/**
  * @brief 步進移動步數
  * @param isForward 正反轉
  * @param goal_pos 目標位置
  */
void STEPPER::open_loop_step_moveTo(uint8_t goal_pos){ 
  static volatile uint32_t ms = 0;
  const bool isForward = (goal_pos - _current_pos) > 0 ? true : false;

  ms ++;

  // 正反轉
	if(isForward) HAL_GPIO_WritePin(this->_dirPort, this->_dirPin, GPIO_PIN_SET);
	else HAL_GPIO_WritePin(this->_dirPort,  this->_dirPin, GPIO_PIN_RESET);

  // 步進脈衝，每 1ms 產生一次脈衝
  if(ms %= 1){   
    if(_current_pos != goal_pos){
      HAL_GPIO_TogglePin(this->_stepPort, this->_stepPin);

      if(isForward) _current_pos ++;
      else _current_pos --;
    }
  }
}


void STEPPER::open_loop_step_move(bool isForward, uint8_t pos){
  static volatile uint32_t ms = 0;
  ms ++;

  // 正反轉
  if(isForward) HAL_GPIO_WritePin(this->_dirPort, this->_dirPin, GPIO_PIN_SET);
  else HAL_GPIO_WritePin(this->_dirPort,  this->_dirPin, GPIO_PIN_RESET);

  // 步進脈衝，每 1ms 產生一次脈衝
  if(ms %= 1 && pos){   
	  HAL_GPIO_TogglePin(this->_stepPort, this->_stepPin);
    pos --;

    if(isForward) _current_pos ++;
    else _current_pos --;
  }
}


/**
  * @brief 步進移動步數
  * @param isForward 正反轉
  */
void STEPPER::open_loop_move(bool isForward){
  static volatile uint32_t ms = 0;
  ms ++;

  // 正反轉
	if(isForward) HAL_GPIO_WritePin(this->_dirPort, this->_dirPin, GPIO_PIN_SET);
	else HAL_GPIO_WritePin(this->_dirPort,  this->_dirPin, GPIO_PIN_RESET);

  // 步進脈衝，每 1ms 產生一次脈衝
  if(ms %= 1){
    HAL_GPIO_TogglePin(this->_stepPort, this->_stepPin);

    if(isForward) _current_pos ++;
    else _current_pos --;
  }
}



/**
  * @brief 歸零校準
  */
void STEPPER::zero_cali(void){
  _current_pos = 0;
}
