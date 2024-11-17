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
    void operate_with_default_mode(uint8_t device, uint8_t mode);
		void update_shoot_and_reload_timer(void);
    void shoot_and_reload(uint8_t device);
    void fine_tune(uint8_t device, float swivel_speed, float elevation_speed);

private:
    int _current_mode_left = 0;
    int _current_mode_right = 0;
	
	// shoot_and_reload() 狀態機變數
		int _step_L = 0, _step_R = 0; 
    uint32_t _ms_L = 0, _ms_R = 0;

    bool _task_complete_L = true, _task_complete_R = true;
		void _execute_shoot_and_reload_L(void);
    void _execute_shoot_and_reload_R(void);

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

extern TURRET Turret;
