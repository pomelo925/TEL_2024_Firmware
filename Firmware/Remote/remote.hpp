/*
 * remote.hpp
 *
 *  Created on: Oct 16, 2024
 *      Author: pomelo925
 */

#pragma once

#include <stm32h7xx.h>

extern TIM_HandleTypeDef htim16;


class REMOTE{
public:
	const bool MODE = false;

	void init();
	void loop();
	void DMA_transfer_PPMData();
	void PPMData_transfer_global();

private:
	typedef struct {
	    uint16_t ch1;
	    uint16_t ch2;
	    uint16_t ch3;
	    uint16_t ch4;
	    uint16_t ch5;
	    uint16_t ch6;
	    uint16_t ch7;
	    uint16_t ch8;
	} PPM_Data;

	PPM_Data ppmData;
	uint32_t ppmBuffer[8];
};

extern REMOTE Remote;
