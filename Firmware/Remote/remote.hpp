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
	uint16_t ppmCnt[9], ppmHigh[8], ppmValue[8]; 

	void init(void);
	void ppm_raw_to_cnt(void);
	void ppm_cnt_to_high(void);
	void ppm_high_to_value(void);
	void ppmValue_to_global(void);

private:
	uint16_t LOW_INTERVAL = 400;  // unit: us
};

extern REMOTE Remote;
