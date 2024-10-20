/*
 * motor.hpp
 *
 *  Created on: Oct 16, 2024
 *      Author: pomelo925
 */

#pragma once

#include "stm32h7xx.h"
#include "stm32h7xx_hal.h"

class DC{
public:
  /**
   * @param dirPort 方向 GPIO 端口
   * @param dirPin 方向 GPIO 引腳
   * @param htim PWM 輸出定時器
   * @param channel PWM 輸出定時器通道
   */
  DC(GPIO_TypeDef* dirPort, uint16_t dirPin, TIM_HandleTypeDef* htim, uint32_t channel)
    : _dirPort(dirPort), _dirPin(dirPin), _htim(htim), _channel(channel){}

  
  void init(void);
  void open_loop_pwm_output(uint16_t duty, GPIO_PinState set);

  GPIO_TypeDef* getDirPort(void) const { return _dirPort; }
  uint16_t getDirPin(void) const { return _dirPin; }
  TIM_HandleTypeDef* getHtim(void) const { return _htim; }
  uint32_t getChannel(void) const { return _channel; }


private:
  GPIO_TypeDef* _dirPort;
  uint16_t _dirPin;
  TIM_HandleTypeDef* _htim; 
  uint32_t _channel; 
};


extern DC DC_ChassisR;
extern DC DC_ChassisL;

extern DC DC_SwivelL;
extern DC DC_SwivelR;

extern DC DC_LauncherR1;
extern DC DC_LauncherR2;
extern DC DC_LauncherL1;
extern DC DC_LauncherL2;
