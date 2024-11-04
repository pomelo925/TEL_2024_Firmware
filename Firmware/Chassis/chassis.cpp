/*
 * chassis.cpp
 *
 *  Created on: Oct 18, 2024
 *      Author: pomelo925
 */

#include <chassis.hpp>
#include <dc.hpp>

CHASSIS Chassis;


/**
 * @brief 底盤運作，追目標車速，並回饋當前車速 (m/s)(rad/s)。
 * @param void
 */
void CHASSIS::run(float GLOBAL_x, float GLOBAL_theta){
  this->TARGET_SPEED.x = GLOBAL_x;
  this->TARGET_SPEED.theta = GLOBAL_theta;

  CHASSIS::update_target_wheel_speed();
  CHASSIS::update_current_chassis_speed();
  return;
}


/**
 * @brief 更新目標輪速 (rad/s)。
 * @param void
 */
void CHASSIS::update_target_wheel_speed(void){
  const float x = this->TARGET_SPEED.x; 
  const float theta = this->TARGET_SPEED.theta; 
  DC_ChassisL.set_target_wheel_speed((2*x + theta*_wheel_distance) / (2*_wheel_radius));
  DC_ChassisR.set_target_wheel_speed((2*x - theta*_wheel_distance) / (2*_wheel_radius));
  return;
}


/**
 * @brief 更新當前車速 (m/s)(rad/s)。
 * @param void
 */
void CHASSIS::update_current_chassis_speed(void){
  CURRENT_SPEED.x = (DC_ChassisR.get_current_wheel_speed() + DC_ChassisL.get_current_wheel_speed()) * _wheel_radius / 2;
  CURRENT_SPEED.theta = (DC_ChassisR.get_current_wheel_speed() - DC_ChassisL.get_current_wheel_speed()) * _wheel_radius / _wheel_distance;
  return;
}


/**
 * @brief 移動至目標點。
 * 
 * @param x X 座標，車頭方向為正。
 * @param y y 座標，車頭右方為正。
 */
void CHASSIS::moveTo(float x, float y){
  return;
}
