/*
 * mission.hpp
 *
 *  Created on: Oct 16, 2024
 *      Author: pomelo925
 */

#pragma once

#include "stm32h7xx.h"

class MISSION{
public:
	void run(uint16_t Mode);
	
	void mission_1(void);
	void mission_2(void);
};

extern MISSION Mission;
