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

};

extern GLOBAL Global;
