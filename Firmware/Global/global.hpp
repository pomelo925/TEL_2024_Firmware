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
  /* 砲台轉盤速度 */
  float Swivel_L_Speed = 0.f;
  float Swivel_R_Speed = 0.f;

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
