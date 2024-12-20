/*
 * servo.hpp
 *
 *  Created on: Oct 20, 2024
 *      Author: pomelo925
 */

#ifndef SERVO_HPP_
#define SERVO_HPP_

#include "stm32h7xx.h"
#include "stm32h7xx_it.h"
#include "stm32h7xx_hal_uart.h"

class SERVO{
public:
	static void init(void);

/* TYPE 1 SERVO */
	SERVO(TIM_HandleTypeDef* TIMx, uint32_t channel):
		_TIMx(TIMx), _channel(channel){
		_goal_deg=50;
	};

	void open_loop_step(void);
	void set_goal_deg(uint32_t goal_deg){_goal_deg = goal_deg;};


/* TYPE 2 SERVO */
	SERVO(UART_HandleTypeDef* huart, uint16_t id):
		_huart(huart), _id(id){
	};

	void UART_send_pos(uint16_t position, uint16_t Time);
	void UART_send_pos_tremble(uint16_t position, uint16_t Time);
	void set_is_moving(bool is_moving){this->is_moving = is_moving;};
	void tremble(void);


private:
/* TYPE 1 SERVO */
	void _deg_to_ccr(void);

	TIM_HandleTypeDef* _TIMx = nullptr;
	uint32_t _channel = 0;

	uint32_t _goal_deg = 0;
	uint32_t _goal_ccr = 0;

	const uint16_t _MIN_DEG = 0;
	const uint16_t _MAX_DEG = 100;
	const int _CYCLE_MS = 10499;


/* TYPE 2 SERVO */
	UART_HandleTypeDef* _huart = nullptr;
	uint16_t _id = 0;
	int _checksum_calc = 0;
	uint16_t _current_goal_pos = 0;

	void UART_send(uint8_t u8_data);

	// trenble() 變數
	uint32_t last_tremble_ms = 0;
	bool is_moving = false;
	const int WAIT_MOVING_MS = 3000;
};

extern SERVO ServoElevatorR, ServoElevatorL;
extern SERVO ServoTriggerR, ServoTriggerL;

#endif /* SERVO_HPP_ */
