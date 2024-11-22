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

	float joystick_mapping(uint16_t ppmHigh, float output_min, float output_max);
	uint8_t switch_mapping(uint16_t ppmHigh);
	uint8_t switch_mode_mapping(void);

	void ppm_raw_to_cnt(void);
	void ppm_cnt_to_high(void);
	void ppm_high_to_switch_and_mode(void);

	uint8_t joystick_state(uint8_t dir);
	void mode_execute(void);

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

	// case 2 (A0D1) 啟動 Flag，切換模式後需重新啟動
	bool _under_case2 = false;
	uint8_t swb_state_last = 0, swc_state_last = 0;


	// 當前 Switch 撥桿狀態
	uint8_t _swa = _SWITCH_UP;
  uint8_t _swb = _SWITCH_UP;
  uint8_t _swc = _SWITCH_UP;
  uint8_t _swd = _SWITCH_UP;

	// Switch 撥桿狀態對應模式
	uint8_t _mode = 0;

	// Switch 撥桿狀態
	enum SWITCH{
		_SWITCH_UP = 1,
		_SWITCH_MIDDLE = 2,
		_SWITCH_DOWN = 3
	};

	// JOYSTICK 搖桿狀態
	enum JOYSTICK{
		_LEFT_TOP = 1,
		_MIDDLE_TOP = 2,
		_RIGHT_TOP = 3,
		_LEFT_MIDDLE = 4,
		_MIDDLE_MIDDLE = 5,
		_RIGHT_MIDDLE = 6,
		_LEFT_BOTTOM = 7,
		_MIDDLE_BOTTOM = 8,
		_RIGHT_BOTTOM = 9
	};

	enum DIR{
		_FORWARD = 1,
		_BACKWARD = 2,
		_LEFT = 3,
		_RIGHT = 4
	};
};

extern REMOTE Remote;
