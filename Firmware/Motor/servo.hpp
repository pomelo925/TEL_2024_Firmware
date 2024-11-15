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
	SERVO(TIM_HandleTypeDef* TIMx, uint8_t channel):
		_TIMx(TIMx), _channel(channel){};

	void open_loop_step(void);
	void set_goal_deg(uint8_t goal_deg){_goal_deg = goal_deg;};


/* TYPE 2 SERVO */
	SERVO(UART_HandleTypeDef* huart, uint16_t id):
		_huart(huart), _id(id){
	};

	void UART_send_pos(uint16_t Position, uint16_t Time);


private:
/* TYPE 1 SERVO */
	void _deg_to_ccr(void);

	TIM_HandleTypeDef* _TIMx = nullptr;
	uint8_t _channel = 0;

	uint8_t _goal_deg = 0;
	uint16_t _goal_ccr = 0;

	const uint8_t _MIN_DEG = 0;
	const uint8_t _MAX_DEG = 180;
	const int _CYCLE_MS = 1000;


/* TYPE 2 SERVO */
	UART_HandleTypeDef* _huart;
	uint16_t _id = 0;
	int _checksum_calc = 0;

	void UART_send(uint8_t u8_data);
};

extern SERVO ServoElevatorR, ServoElevatorL;
extern SERVO ServoTriggerR, ServoTriggerL;

#endif /* SERVO_HPP_ */
