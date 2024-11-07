/*
 * stepper.hpp
 *
 *  Created on: Oct 20, 2024
 *      Author: pomelo925
 */

#ifndef STEPPER_HPP_
#define STEPPER_HPP_

#include "stm32h7xx.h"
#include "stm32h7xx_hal.h"

class STEPPER{
public:
  STEPPER(GPIO_TypeDef* dirPort, uint16_t dirPin,
        GPIO_TypeDef* stepPort, uint16_t stepPin):
        _dirPort(dirPort), _dirPin(dirPin), _stepPort(stepPort), _stepPin(stepPin){};

  void open_loop_step(void);
  void set_goal_pos(const int goalPos){this->_goal_pos = goalPos;};


private:
  GPIO_TypeDef* _dirPort = nullptr;
  uint16_t _dirPin = 0;
  GPIO_TypeDef* _stepPort = nullptr;
  uint16_t _stepPin = 0;

  uint8_t _sub_division = 8;  // MS1 = GND, MS2 = GND => 8 细分控制精度
  uint16_t _round_step = 1600; // 步進旋轉一圈，需 1600 個脈衝

  // 目標步數
  int _goal_pos = 0;
};

extern STEPPER Stepper_L, Stepper_R;


#endif /* STEPPER_HPP_ */
