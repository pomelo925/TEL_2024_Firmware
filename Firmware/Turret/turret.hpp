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
	static void init(void);
  void operate_with_default_mode(uint8_t device, uint8_t mode);
	void update_timer(void);
  void shoot_and_reload(uint8_t device, bool trigger_once);
  void fine_tune(uint8_t device, float swivel_duty, float elevation_angle);

private:
  int _current_mode_left = 0;
  int _current_mode_right = 0;
	
  // shoot_and_reload() 狀態機變數
  int _step_L = 0, _step_R = 0;
  uint32_t _ms_L = 0, _ms_R = 0;
  bool _working_R = false, _working_L = false;
  bool _work_unit_done_L = false, _work_unit_done_R = false;
  void _execute_shoot_and_reload_L(void);
  void _execute_shoot_and_reload_R(void);

	// fine_tune() 變數
	uint32_t _fine_tune_ms_L = 0;
	uint32_t _fine_tune_ms_R = 0;
	void tune_default_mode_elevator_deg(uint8_t device, uint32_t step);

	// init() 變數
	static uint32_t _init_ms;

	// default mode 參數
	uint8_t _CURRENT_LAUNCH_MODE_L = 0;
	uint8_t _CURRENT_LAUNCH_MODE_R = 0;

	uint32_t _LEFT_TOP_ELEVATOR_L = 50;
	uint32_t _RIGHT_TOP_ELEVATOR_L = 50;
	uint32_t _LEFT_MIDDLE_ELEVATOR_L = 50;
	uint32_t _RIGHT_MIDDLE_ELEVATOR_L = 50;
	uint32_t _LEFT_BOTTOM_ELEVATOR_L = 50;
	uint32_t _RIGHT_BOTTOM_ELEVATOR_L = 50;

	uint32_t _LEFT_TOP_ELEVATOR_R = 50;
	uint32_t _RIGHT_TOP_ELEVATOR_R = 50;
	uint32_t _LEFT_MIDDLE_ELEVATOR_R = 50;
	uint32_t _RIGHT_MIDDLE_ELEVATOR_R = 50;
	uint32_t _LEFT_BOTTOM_ELEVATOR_R = 50;
	uint32_t _RIGHT_BOTTOM_ELEVATOR_R = 50;


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
