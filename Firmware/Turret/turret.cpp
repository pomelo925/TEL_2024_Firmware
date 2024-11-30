/*
 * turret.cpp
 *
 *  Created on: Oct 16, 2024
 *      Author: pomelo925
 */

#include "stm32h7xx_hal.h"

#include <turret.hpp>
#include <stepper.hpp>
#include <servo.hpp>
#include <dc.hpp>


TURRET Turret;

uint32_t TURRET::_init_ms = 0;

/**
 * @brief 初始化
 */
void TURRET::init(void){
    Stepper_R.set_goal_pos(500);
    while(TURRET::_init_ms < 1500){};  // 等待 1.5 秒

    ServoTriggerR.UART_send_pos(1150, 100, true);
    while(TURRET::_init_ms < 2500){};  // 等待 1 秒

    Stepper_R.set_goal_pos(-13000);
    while(TURRET::_init_ms < 6000){};  // 等待 3.5 秒

    Stepper_R.set_goal_pos(6000);
    return;
}


/**
 * @brief 根據預設模式，使 SWIVEL 及 LAUNCHER 以特定位置和速度運行。
 * 
 * @param device 左砲台或右砲台
 * @param mode 不同目標點的模式
 */
void TURRET::operate_with_default_mode(uint8_t device, uint8_t mode){
	const uint8_t max_duty = 100;
//	const uint8_t max_duty = 50;

    // LEFT
    if(device == _LEFT){
        this->_current_mode_left = mode;

        switch (mode){
        case _LEFT_TOP:
            DC_SwivelL.set_target_pos_adc(20);
            ServoElevatorL.set_goal_deg(50);
        	DC_LauncherL1.set_duty(max_duty);
        	DC_LauncherL2.set_duty(max_duty);
            break;
        
        case _RIGHT_TOP:
            DC_SwivelL.set_target_pos_adc(20);
            ServoElevatorL.set_goal_deg(50);
        	DC_LauncherL1.set_duty(max_duty);
        	DC_LauncherL2.set_duty(max_duty);
            break;

        case _LEFT_MIDDLE:
            DC_SwivelL.set_target_pos_adc(20);
            ServoElevatorL.set_goal_deg(50);
        	DC_LauncherL1.set_duty(max_duty);
        	DC_LauncherL2.set_duty(max_duty);
            break;

        case _RIGHT_MIDDLE:
            DC_SwivelL.set_target_pos_adc(20);
            DC_SwivelL.set_target_pos_adc(20);
            ServoElevatorL.set_goal_deg(50);
        	DC_LauncherL1.set_duty(max_duty);
        	DC_LauncherL2.set_duty(max_duty);
            break;
        
        case _LEFT_BOTTOM:
            DC_SwivelL.set_target_pos_adc(20);
            ServoElevatorL.set_goal_deg(50);
        	DC_LauncherL1.set_duty(max_duty);
        	DC_LauncherL2.set_duty(max_duty);
            break;

        case _RIGHT_BOTTOM:
            DC_SwivelL.set_target_pos_adc(20);
            ServoElevatorL.set_goal_deg(50);
        	DC_LauncherL1.set_duty(max_duty);
        	DC_LauncherL2.set_duty(max_duty);
            break;

        default:
            // ServoElevatorL.set_goal_deg(0);
        	// DC_LauncherL1.set_duty(0);
        	// DC_LauncherL2.set_duty(0);
            break;
        }
    }

    // RIGHT
    else if(device == _RIGHT){
        this->_current_mode_right = mode;
        
        switch (mode){
        case _LEFT_TOP:
            DC_SwivelR.set_target_pos_adc(20);
            ServoElevatorR.set_goal_deg(50);
        	DC_LauncherR1.set_duty(max_duty);
        	DC_LauncherR2.set_duty(max_duty);
            break;
        
        case _RIGHT_TOP:
            DC_SwivelR.set_target_pos_adc(20);
            ServoElevatorR.set_goal_deg(50);
        	DC_LauncherR1.set_duty(max_duty);
        	DC_LauncherR2.set_duty(max_duty);
            break;

        case _LEFT_MIDDLE:
            DC_SwivelR.set_target_pos_adc(20);
            ServoElevatorR.set_goal_deg(50);
        	DC_LauncherR1.set_duty(max_duty);
        	DC_LauncherR2.set_duty(max_duty);
            break;

        case _RIGHT_MIDDLE:
            DC_SwivelR.set_target_pos_adc(20);
            ServoElevatorR.set_goal_deg(50);
        	DC_LauncherR1.set_duty(max_duty);
        	DC_LauncherR2.set_duty(max_duty);
            break;
        
        case _LEFT_BOTTOM:
            DC_SwivelR.set_target_pos_adc(20);
            ServoElevatorR.set_goal_deg(50);
        	DC_LauncherR1.set_duty(max_duty);
        	DC_LauncherR2.set_duty(max_duty);
            break;

        case _RIGHT_BOTTOM:
            DC_SwivelR.set_target_pos_adc(20);
            ServoElevatorR.set_goal_deg(50);
        	DC_LauncherR1.set_duty(max_duty);
        	DC_LauncherR2.set_duty(max_duty);
            break;

        default:
            // ServoElevatorR.set_goal_deg(0);
        	// DC_LauncherR1.set_duty(0);
        	// DC_LauncherR2.set_duty(0);
            break;
        }
    }
    return;
}


/**
 * @brief 左砲台狀態機，發射&填彈
 */
void TURRET::_execute_shoot_and_reload_L(void){
    switch (this->_step_L){
    case 0:
        ServoTriggerL.UART_send_pos(1100, 4000, true);
        this->_step_L = 1;
        this->_ms_L = 0;
        break;
    
    case 1:
        if (this->_ms_L >= 1000) { // 等待 1 秒
            ServoTriggerL.UART_send_pos(1250, 500, true);
            this->_step_L = 2;
            this->_ms_L = 0;
        }
        break;
    
    case 2:
        if (this->_ms_L >= 1000) { // 等待 1 秒
            ServoTriggerL.UART_send_pos(950, 500, true);
            Stepper_L.set_goal_pos(-5500);
            this->_step_L = 3;
            this->_ms_L = 0;
        }
        break;

    case 3:
        if (this->_ms_L >= 1000) { // 等待 1 秒
        	Stepper_L.set_goal_pos(3000);
            ServoTriggerL.UART_send_pos(1100, 500, true);
            this->_step_L = 0;
            this->_ms_L = 0;
            this->_work_unit_done_L = true;
        }
        break;
    
    default:
        break;

    }
    return;
}


/**
 * @brief 右砲台狀態機，發射&填彈
 */
void TURRET::_execute_shoot_and_reload_R(void){
    switch (this->_step_R){
    case 0:
        ServoTriggerR.UART_send_pos(1150, 100, true);
        this->_step_R = 1;
        this->_ms_R = 0;
        break;

    case 1:
        if (this->_ms_R >= 300) { // 等待 0.3 秒
            ServoTriggerR.UART_send_pos(1250, 1, true);
            this->_step_R = 2;
            this->_ms_R = 0;
        }
        break;

    case 2:
        if (this->_ms_R >= 500) { // 等待 0.5 秒
            ServoTriggerR.UART_send_pos(950, 100, true);
            this->_step_R = 3;
            this->_ms_R = 0;
        }
        break;
    

    case 3:
        if (this->_ms_R >= 500) { // 等待 0.5 秒
            Stepper_R.set_goal_pos(-29000);
            this->_step_R = 4;
            this->_ms_R = 0;
        }
        break;
    
    case 4:
        if (this->_ms_R >= 1000) { // 等待 1 秒
            Stepper_R.set_goal_pos(9000);
            this->_step_R = 5;
            this->_ms_R = 0;
        }
        break;

    case 5:
        if (this->_ms_R >= 1000) { // 等待 1 秒
        	ServoTriggerR.UART_send_pos(1150, 100, true);
            this->_step_R = 6;
            this->_ms_R = 0;
        }

    case 6:
        if (this->_ms_R >= 500) { // 等待 0.5 秒
        	Stepper_R.set_goal_pos(-9000);
            this->_step_R = 0;
            this->_ms_R = 0;
            this->_work_unit_done_R = true;
        }
        break;
    
    default:
        break;
    
    }
    return;
}


/**
 * @brief 砲台發射&填彈
 * @param device 左砲台或右砲台
 * @param trigger_once 觸發一次
 */
void TURRET::shoot_and_reload(uint8_t device, bool trigger_once){
    // 改變工作狀態
    if(trigger_once){
        if(device == _LEFT) {
            this->_working_L = true;
            this->_work_unit_done_L = false;
        }
        if(device == _RIGHT) {
            this->_working_R = true;
            this->_work_unit_done_R = false;
        }
    }

    // 檢查是否有工作
    if(!this->_working_L && !this->_working_R) return;

    // 有工作，執行狀態機
    if (device == _LEFT && this->_working_L){
        if(!this->_work_unit_done_L) this->_execute_shoot_and_reload_L();
        else {
            this->_step_L = 0;
            this->_ms_L = 0;
            this->_working_L = false;
        }
        return;
    }

    if (device == _RIGHT && this->_working_R){
        if(!this->_work_unit_done_R) this->_execute_shoot_and_reload_R();
        else {
            this->_step_R = 0;
            this->_ms_R = 0;
            this->_working_R = false;
        }
        return;
    }

    return;
}


/**
 * @brief 細調砲台位置
 * @param device 左砲台或右砲台
 * @param swivel_duty 旋轉占空比（%）
 * @param elevation_angle 仰角角度（°）
 */
void TURRET::fine_tune(uint8_t device, float swivel_duty, uint32_t elevation_angle){
    // 最快每秒執行一次
    static uint32_t last_ms = 0;
    if(_fine_tune_ms - last_ms < 1000) return;
    last_ms = _fine_tune_ms;

    const uint32_t swivel_unit_step = 5;
    const uint32_t elevation_unit_step = 2;

    if(device == _LEFT){
        // swivel
        if(abs(swivel_duty)>50) DC_SwivelL.add_target_pos_adc(swivel_unit_step);

        // elevation
        if(elevation_angle>50) ServoElevatorL.add_goal_deg(elevation_unit_step);
    }
    
    else if(device == _RIGHT){
        // swivel
    	if(abs(swivel_duty)>50) DC_SwivelR.add_target_pos_adc(swivel_unit_step);

        // elevation
        if(elevation_angle>50) ServoElevatorR.add_goal_deg(elevation_unit_step);
    }
    
    return;
}



/**
 * @brief 更新發射填彈的外部時鐘計算
 */
void TURRET::update_timer(void){
    if(!this->_work_unit_done_L) this->_ms_L++;
    if(!this->_work_unit_done_R) this->_ms_R++;
    _fine_tune_ms++;
    _init_ms++;
    return;
}
