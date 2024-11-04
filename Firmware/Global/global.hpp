/*
 * medium.hpp
 *
 *  Created on: Oct 20, 2024
 *      Author: pomelo925
 */

#pragma once

#include <stm32h7xx.h>

class GLOBAL {
public:
  /* 底盤運動速度 */
  float Chassis_X_Speed = 0.f;
  float Chassis_Theta_Speed = 0.f;

  /* 砲台轉盤速度 */
  float Swivel_L_Speed = 0.f;
  float Swivel_R_Speed = 0.f;

  /* 砲台發射，開路控制， PWM 占空比*/
  float launcher_R1_DUTY = 0.f;
  float launcher_R2_DUTY = 0.f;
  float launcher_L1_DUTY = 0.f;
  float launcher_L2_DUTY = 0.f;

  /* 步進馬達位置 */
  uint8_t stepper_pos = 0;

  /* 遙控器撥桿狀態 */
  uint8_t swa = 0;
  uint8_t swb = 0;
  uint8_t swc = 0;
  uint8_t swd = 0;

  /* 主模式 */
  uint8_t mode = 0;
};

extern GLOBAL Global;
