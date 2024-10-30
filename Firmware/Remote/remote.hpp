/*
 * remote.hpp
 *
 *  Created on: Oct 16, 2024
 *      Author: pomelo925
 */

#pragma once

#include "stm32h7xx.h"
#include "stm32h7xx_hal.h"
#include "stm32h7xx_hal_dma.h"


class REMOTE{
public:
	int count=0;

	uint32_t ppmRaw[18];
	uint16_t ppmCnt[9], ppmHigh[8];

	void init(void);

	uint8_t switch_mapping(uint16_t ppmHigh);
	float joystick_mapping(uint16_t ppmHigh, float output_min, float output_max);

	void ppm_raw_to_cnt(void);
	void ppm_cnt_to_high(void);
	void ppmHigh_to_global(void);

	uint8_t switch_mode_mapping(void);
	void global_update(void);

private:
  // PPM 訊號間隔判定（單位：us）
  const uint16_t _PPM_INTERVAL = 6000; //超過 6ms 視為新週期。

	// PPM 訊號占空比判定（單位：us），僅用於判定 Switch 撥桿
	const uint16_t _PPM_SWITCH_DUTY_MIN = 1050; //小於 1050 視為低電平
	const uint16_t _PPM_SWITCH_DUTY_MAX = 1950; //大於 1950 視為高電平。

	// PPM 訊號占空比判定（單位：us），僅用於判定 Joystick 類比搖桿
	const uint16_t _PPM_JOYSTICK_DUTY_MIN = 1004; 
	const uint16_t _PPM_JOYSTICK_DUTY_MAX = 2011;
	const uint16_t _PPM_JOYSTICK_DUTY_DEFAULT = 1508;

	// Switch 撥桿狀態
	enum SWITCH{
		_SWITCH_UP = 1,
		_SWITCH_MIDDLE = 2,
		_SWITCH_DOWN = 3
	};
};

extern REMOTE Remote;
