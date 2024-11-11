/*
 * servo.cpp
 *
 *  Created on: Oct 20, 2024
 *      Author: pomelo925
 */

#include <servo.hpp>

extern TIM_HandleTypeDef htim24;

extern UART_HandleTypeDef huart3;

SERVO ServoElevatorR(&htim24,1);
SERVO ServoElevatorL(&htim24,3);


/**
 * @brief 開環控制，須進中斷。
 */
void SERVO::open_loop_step(void){
  // 設定 CCR 值
  this->_deg_to_ccr();

  // 輸出 PWM
  switch(this->_channel){
    case 1:
      __HAL_TIM_SET_COMPARE(this->_TIMx, TIM_CHANNEL_1, this->_goal_ccr);
      break;
    case 2:
      __HAL_TIM_SET_COMPARE(this->_TIMx, TIM_CHANNEL_2, this->_goal_ccr);
      break;
    case 3:
      __HAL_TIM_SET_COMPARE(this->_TIMx, TIM_CHANNEL_3, this->_goal_ccr);
      break;
    case 4:
      __HAL_TIM_SET_COMPARE(this->_TIMx, TIM_CHANNEL_4, this->_goal_ccr);
      break;

    default:
      break;
  }

  return;
}


/**
 * @brief 將角度轉換為 CCR 值
 */
void SERVO::_deg_to_ccr(void){
  if (_goal_deg < _MIN_DEG) _goal_deg = _MIN_DEG;
  if (_goal_deg > _MAX_DEG) _goal_deg = _MAX_DEG;

  _goal_ccr = static_cast<uint16_t>((_goal_deg * 1000) / _MAX_DEG);

  return;
}


/**
 * @brief 透過 UART 通訊，傳送資料。
 * @param u8_data 資料。
 */
void SERVO::UART_send(uint8_t u8_data) {
	uint8_t *u8_pointer = &u8_data;
	HAL_UART_Transmit(&huart3, u8_pointer, 1, 100);

	this->_checksum_calc += u8_data;
}


/**
 * @brief 透過 UART 通訊，傳送位置與時間。
 * 
 * @param Position 位置
 * @param Time 時間
 */
void SERVO::UART_send_pos(uint16_t Position, uint16_t Time) {
	this->_checksum_calc = 0;
	UART_send(0x80 + _id);    //header mark & broadcast ID
	UART_send(0x83);              //header mark & command code
	UART_send(0x05);              //total data length
	UART_send((Position / 256) & 0x7F);  //Servo Pos_H
	UART_send(Position % 256);           //Servo Pos_L
	UART_send((Time / 256) & 0x7F); //Servo Time_H
	UART_send(Time % 256);          //Servo Time_L
	UART_send(this->_checksum_calc);     //data length (one servo with time and speed)
}
