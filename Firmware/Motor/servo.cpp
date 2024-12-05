/*
 * servo.cpp
 *
 *  Created on: Oct 20, 2024
 *      Author: pomelo925
 */

#include <servo.hpp>

/* TYPE 1 SERVO */
extern TIM_HandleTypeDef htim1;

SERVO ServoElevatorR(&htim1, TIM_CHANNEL_2);
SERVO ServoElevatorL(&htim1, TIM_CHANNEL_3);


/* TYPE 2 SERVO */
extern UART_HandleTypeDef huart6;
extern UART_HandleTypeDef huart10;

SERVO ServoTriggerR(&huart6, 1);
SERVO ServoTriggerL(&huart10, 3);


void SERVO::init(void){
	HAL_TIM_PWM_Start(ServoElevatorR._TIMx, ServoElevatorR._channel);
	HAL_TIM_PWM_Start(ServoElevatorL._TIMx, ServoElevatorL._channel);

	ServoTriggerL.UART_send_pos(2120, 200);
	ServoTriggerR.UART_send_pos(950, 200);

	ServoElevatorL.set_goal_deg(98);
	ServoElevatorR.set_goal_deg(98);
}


/**
 * @brief 開環控制，須進中斷。
 */
void SERVO::open_loop_step(void){
  // 設定 CCR 值
  this->_deg_to_ccr();

  __HAL_TIM_SET_COMPARE(this->_TIMx, this->_channel, this->_goal_ccr);

  return;
}


/**
 * @brief 將角度轉換為 CCR 值
 */
void SERVO::_deg_to_ccr(void) {
  // 確保輸入範圍在 [0, _MAX_DEG]
  if (this->_goal_deg > _MAX_DEG) this->_goal_deg = static_cast<uint16_t>(_MAX_DEG);
  else if (this->_goal_deg < 0) this->_goal_deg = 0;

  // 計算 _goal_ccr，對應到 _CYCLE_MS 的 42.8571% ~ 100%
  const float lower_bound = 0.428571f; // 42.8571%
  const float upper_bound = 1.0f;      // 100%
  this->_goal_ccr = static_cast<uint32_t>(
      this->_CYCLE_MS * (lower_bound +
      (static_cast<float>(this->_goal_deg) / _MAX_DEG) * (upper_bound - lower_bound))
  );

  return;
}


/**
 * @brief 透過 UART 通訊，傳送資料。
 * @param u8_data 資料。
 */
void SERVO::UART_send(uint8_t u8_data) {
	uint8_t *u8_pointer = &u8_data;
	HAL_UART_Transmit(_huart, u8_pointer, 1, 100);

	this->_checksum_calc += u8_data;
}


/**
 * @brief 透過 UART 通訊，傳送位置與時間。
 * 
 * @param Position 位置
 * @param Time 時間
 */
void SERVO::UART_send_pos(uint16_t position, uint16_t Time) {
	this->_checksum_calc = 0;
	UART_send(0x80 + _id);    //header mark & broadcast ID
	UART_send(0x83);              //header mark & command code
	UART_send(0x05);              //total data length
	UART_send((position / 256) & 0x7F);  //Servo Pos_H
	UART_send(position % 256);           //Servo Pos_L
	UART_send((Time / 256) & 0x7F); //Servo Time_H
	UART_send(Time % 256);          //Servo Time_L
	UART_send(this->_checksum_calc);     //data length (one servo with time and speed)

	this->is_moving = true;
	this->_current_goal_pos = position;
	return;
}


/**
 * @brief 透過 UART 通訊，傳送位置與時間。
 * 
 * @param Position 位置
 * @param Time 時間
 */
void SERVO::UART_send_pos_tremble(uint16_t position, uint16_t Time) {
	this->_checksum_calc = 0;
	UART_send(0x80 + _id);    //header mark & broadcast ID
	UART_send(0x83);              //header mark & command code
	UART_send(0x05);              //total data length
	UART_send((position / 256) & 0x7F);  //Servo Pos_H
	UART_send(position % 256);           //Servo Pos_L
	UART_send((Time / 256) & 0x7F); //Servo Time_H
	UART_send(Time % 256);          //Servo Time_L
	UART_send(this->_checksum_calc);     //data length (one servo with time and speed)

	return;
}



/**
 * @brief 顫抖，週期 20ms，震幅 pos +- 10
 */
void SERVO::tremble(void){
	// 週期 200 ms
	const uint8_t TREMBLE_PERIOD_MS = 200;
	if(HAL_GetTick() - this->last_tremble_ms < TREMBLE_PERIOD_MS) return;

	// 先檢查是否在移動，超過 3 秒才 tremble
	if(this->is_moving && HAL_GetTick() - this->last_tremble_ms < this->WAIT_MOVING_MS) return;
	this->is_moving = false;
	this->last_tremble_ms = HAL_GetTick();

	/* 顫抖 */
	// 產生 -5 ~ 5 的隨機數
	const int16_t pos = rand() % 11 -5;
	SERVO::UART_send_pos_tremble(this->_current_goal_pos + pos, 1);

	return;
}
