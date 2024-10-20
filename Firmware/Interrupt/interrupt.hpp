/*
 * interrupt.hpp
 *
 *  Created on: Oct 16, 2024
 *      Author: pomelo925
 */

#pragma once

#include "stm32h7xx_hal.h"

extern TIM_HandleTypeDef htim6;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);