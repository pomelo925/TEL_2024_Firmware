/*
 * turret.hpp
 *
 *  Created on: Oct 16, 2024
 *      Author: pomelo925
 */

#pragma once

#include "stm32h7xx.h"

class TURRET{
public:
    void operate_with_default_mode(uint8_t device, int mode);
    void shoot(int device);
    void fine_tune(uint8_t device, float swivel_speed, float elevation_speed);
    void reload(uint8_t device);

private:
    int _current_mode_left = 0;
    int _current_mode_right = 0;

	enum DIR{
		_FORWARD = 1,
		_BACKWARD = 2,
		_LEFT = 3,
		_RIGHT = 4
	};
};

extern TURRET Turret;
