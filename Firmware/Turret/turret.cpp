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
    Stepper_L.set_goal_pos(500);
    while(TURRET::_init_ms < 800){};  // 等待 0.8 秒

    ServoTriggerR.UART_send_pos(1150, 100, true);
    ServoTriggerL.UART_send_pos(2010, 100, true);
    while(TURRET::_init_ms < 2500){};  // 等待 1 秒

    Stepper_R.set_goal_pos(-13000);
    Stepper_L.set_goal_pos(-13000);
    while(TURRET::_init_ms < 6000){};  // 等待 3.5 秒

    Stepper_R.set_goal_pos(6000);
    Stepper_L.set_goal_pos(6000);
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

    // LEFT
    if(device == _LEFT){
        this->_current_mode_left = mode;

        DC_LauncherL1.set_duty(-max_duty);
        DC_LauncherL2.set_duty(-max_duty);

        switch (mode){
        case _LEFT_TOP:
            _CURRENT_LAUNCH_MODE_L = _LEFT_TOP;
            ServoElevatorL.set_goal_deg(_LEFT_TOP_ELEVATOR_L);
            // DC_SwivelL.set_target_pos_adc(20);
            break;
        
        case _RIGHT_TOP:
            _CURRENT_LAUNCH_MODE_L = _RIGHT_TOP;
            ServoElevatorL.set_goal_deg(_RIGHT_TOP_ELEVATOR_L);
            // DC_SwivelL.set_target_pos_adc(20);
            break;

        case _LEFT_MIDDLE:
            _CURRENT_LAUNCH_MODE_L = _LEFT_MIDDLE;
            ServoElevatorL.set_goal_deg(_LEFT_MIDDLE_ELEVATOR_L);
            // DC_SwivelL.set_target_pos_adc(20);
            break;

        case _RIGHT_MIDDLE:
            _CURRENT_LAUNCH_MODE_L = _RIGHT_MIDDLE;
            ServoElevatorL.set_goal_deg(_RIGHT_MIDDLE_ELEVATOR_L);
            // DC_SwivelL.set_target_pos_adc(20);
            break;
        
        case _LEFT_BOTTOM:
            _CURRENT_LAUNCH_MODE_L = _LEFT_BOTTOM;
            ServoElevatorL.set_goal_deg(_LEFT_BOTTOM_ELEVATOR_L);
            // DC_SwivelL.set_target_pos_adc(20);
            break;

        case _RIGHT_BOTTOM:
            _CURRENT_LAUNCH_MODE_L = _RIGHT_BOTTOM;
            ServoElevatorL.set_goal_deg(_RIGHT_BOTTOM_ELEVATOR_L);
            // DC_SwivelL.set_target_pos_adc(20);
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
//        DC_LauncherR1.set_duty(max_duty);
//        DC_LauncherR2.set_duty(max_duty);
        
        switch (mode){
        case _LEFT_TOP:
            _CURRENT_LAUNCH_MODE_R = _LEFT_TOP;
            ServoElevatorR.set_goal_deg(_LEFT_TOP_ELEVATOR_R);
            // DC_SwivelR.set_target_pos_adc(20);
            break;
        
        case _RIGHT_TOP:
            _CURRENT_LAUNCH_MODE_R = _RIGHT_TOP;
            ServoElevatorR.set_goal_deg(_RIGHT_TOP_ELEVATOR_R);
            // DC_SwivelR.set_target_pos_adc(20);
            break;

        case _LEFT_MIDDLE:
            _CURRENT_LAUNCH_MODE_R = _LEFT_MIDDLE;
            ServoElevatorR.set_goal_deg(_LEFT_MIDDLE_ELEVATOR_R);
            // DC_SwivelR.set_target_pos_adc(20);
            break;

        case _RIGHT_MIDDLE:
            _CURRENT_LAUNCH_MODE_R = _RIGHT_MIDDLE;
            ServoElevatorR.set_goal_deg(_RIGHT_MIDDLE_ELEVATOR_R);
            // DC_SwivelR.set_target_pos_adc(20);
            break;
        
        case _LEFT_BOTTOM:
            _CURRENT_LAUNCH_MODE_R = _LEFT_BOTTOM;
            ServoElevatorR.set_goal_deg(_LEFT_BOTTOM_ELEVATOR_R);
            // DC_SwivelR.set_target_pos_adc(20);
            break;

        case _RIGHT_BOTTOM:
            _CURRENT_LAUNCH_MODE_R = _RIGHT_BOTTOM;
            ServoElevatorR.set_goal_deg(_RIGHT_BOTTOM_ELEVATOR_R);
            // DC_SwivelR.set_target_pos_adc(20);
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
        ServoTriggerL.UART_send_pos(1870, 1000, true);
        this->_step_L = 1;
        this->_ms_L = 0;
        break;

    case 1:
        if (this->_ms_L >= 300) { // 等待 0.3 秒
            ServoTriggerL.UART_send_pos(1800, 1, true);
            this->_step_L = 2;
            this->_ms_L = 0;
        }
        break;

    case 2:
        if (this->_ms_L >= 300) { // 等待 0.3 秒
            ServoTriggerL.UART_send_pos(2110, 100, true);
            this->_step_L = 3;
            this->_ms_L = 0;
        }
        break;
    

    case 3:
        if (this->_ms_L >= 300) { // 等待 0.3 秒
            Stepper_L.set_goal_pos(-6000);
            this->_step_L = 4;
            this->_ms_L = 0;
        }
        break;
    
    case 4:
        if (this->_ms_L >= 1800) { // 等待 1.8 秒
            Stepper_L.set_goal_pos(5000);
            this->_step_L = 5;
            this->_ms_L = 0;
        }
        break;

    case 5:
        if (this->_ms_L >= 1600) { // 等待 1.6 秒
            ServoTriggerL.UART_send_pos(1870, 1000, true);
        	Stepper_L.set_goal_pos(-5000);
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
        ServoTriggerR.UART_send_pos(1150, 1000, true);
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
        if (this->_ms_R >= 300) { // 等待 0.3 秒
            ServoTriggerR.UART_send_pos(950, 100, true);
            this->_step_R = 3;
            this->_ms_R = 0;
        }
        break;
    

    case 3:
        if (this->_ms_R >= 300) { // 等待 0.3 秒
            Stepper_R.set_goal_pos(-6000);
            this->_step_R = 4;
            this->_ms_R = 0;
        }
        break;
    
    case 4:
        if (this->_ms_R >= 1800) { // 等待 1.8 秒
            Stepper_R.set_goal_pos(5000);
            this->_step_R = 5;
            this->_ms_R = 0;
        }
        break;

    case 5:
        if (this->_ms_R >= 1600) { // 等待 1.6 秒
            ServoTriggerR.UART_send_pos(1150, 1000, true);
        	Stepper_R.set_goal_pos(-5000);
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
void TURRET::fine_tune(uint8_t device, float swivel_duty, float elevation_angle){
//    const uint32_t swivel_unit_step = 5;
    const uint32_t elevation_unit_step = 2;

    const float swivel_working_duty_L = 60;
    const float swivel_working_duty_R = 80;

    if(device == _LEFT){
        // swivel
        // if(swivel_duty>50.f) DC_SwivelL.add_target_pos_adc(swivel_unit_step);
        // if(swivel_duty<-50.f) DC_SwivelL.add_target_pos_adc(-swivel_unit_step);

        // 直控
        if(abs(swivel_duty)<50.f) DC_SwivelL.set_duty(100);
        else if(swivel_duty>50.f) DC_SwivelL.set_duty(-swivel_working_duty_L);
        else DC_SwivelL.set_duty(swivel_working_duty_L);

        // 每秒執行一次
        static uint32_t last_ms_L = 0;
        if(_fine_tune_ms_L - last_ms_L < 500)  return;
        last_ms_L = _fine_tune_ms_L;

        // elevation
        if(elevation_angle>50.f) Turret.tune_default_mode_elevator_deg(_LEFT, elevation_unit_step);
        if(elevation_angle<-50.f) Turret.tune_default_mode_elevator_deg(_LEFT, -elevation_unit_step);
    }
    
    else if(device == _RIGHT){
        // swivel
    	// if(swivel_duty>50.f) DC_SwivelR.add_target_pos_adc(swivel_unit_step);
        // if(swivel_duty<-50.f) DC_SwivelR.add_target_pos_adc(-swivel_unit_step);

        // 直控
        if(abs(swivel_duty)<50.f) DC_SwivelR.set_duty(100);
        else if(swivel_duty>50.f) DC_SwivelR.set_duty(swivel_working_duty_R);
        else DC_SwivelR.set_duty(-swivel_working_duty_R);

        // 每秒執行一次
        static uint32_t last_ms_R = 0;
        if(_fine_tune_ms_R - last_ms_R < 500) return;
        last_ms_R = _fine_tune_ms_R;

        // elevation
        if(elevation_angle>50.f) Turret.tune_default_mode_elevator_deg(_RIGHT, elevation_unit_step);
        if(elevation_angle<-50.f) Turret.tune_default_mode_elevator_deg(_RIGHT, -elevation_unit_step);
    }

    return;
}


void TURRET::tune_default_mode_elevator_deg(uint8_t device, uint32_t step){
    if(device == _LEFT){
        switch (_CURRENT_LAUNCH_MODE_L){
        case _LEFT_TOP:
            _LEFT_TOP_ELEVATOR_L += step;
            ServoElevatorL.set_goal_deg(_LEFT_TOP_ELEVATOR_R);
            break;
        
        case _RIGHT_TOP:
            _RIGHT_TOP_ELEVATOR_L += step;
            ServoElevatorL.set_goal_deg(_RIGHT_TOP_ELEVATOR_L);
            break;

        case _LEFT_MIDDLE:
            _LEFT_MIDDLE_ELEVATOR_L += step;
            ServoElevatorL.set_goal_deg(_LEFT_MIDDLE_ELEVATOR_L);
            break;

        case _RIGHT_MIDDLE:
            _RIGHT_MIDDLE_ELEVATOR_L += step;
            ServoElevatorL.set_goal_deg(_RIGHT_MIDDLE_ELEVATOR_L);
            break;
        
        case _LEFT_BOTTOM:
            _LEFT_BOTTOM_ELEVATOR_L += step;
            ServoElevatorL.set_goal_deg(_LEFT_BOTTOM_ELEVATOR_L);
            break;

        case _RIGHT_BOTTOM:
            _RIGHT_BOTTOM_ELEVATOR_L += step;
            ServoElevatorL.set_goal_deg(_RIGHT_BOTTOM_ELEVATOR_L);
            break;

        default:
            break;
        }
    }
    else if(device == _RIGHT){
        switch (_CURRENT_LAUNCH_MODE_R){
        case _LEFT_TOP:
            _LEFT_TOP_ELEVATOR_R += step;
            ServoElevatorR.set_goal_deg(_LEFT_TOP_ELEVATOR_R);
            break;
        
        case _RIGHT_TOP:
            _RIGHT_TOP_ELEVATOR_R += step;
            ServoElevatorR.set_goal_deg(_RIGHT_TOP_ELEVATOR_R);
            break;

        case _LEFT_MIDDLE:
            _LEFT_MIDDLE_ELEVATOR_R += step;
            ServoElevatorR.set_goal_deg(_LEFT_MIDDLE_ELEVATOR_R);
            break;

        case _RIGHT_MIDDLE:
            _RIGHT_MIDDLE_ELEVATOR_R += step;
            ServoElevatorR.set_goal_deg(_RIGHT_MIDDLE_ELEVATOR_R);
            break;
        
        case _LEFT_BOTTOM:
            _LEFT_BOTTOM_ELEVATOR_R += step;
            ServoElevatorR.set_goal_deg(_LEFT_BOTTOM_ELEVATOR_R);
            break;

        case _RIGHT_BOTTOM:
            _RIGHT_BOTTOM_ELEVATOR_R += step;
            ServoElevatorR.set_goal_deg(_RIGHT_BOTTOM_ELEVATOR_R);
            break;

        default:
            break;
        }
    }

    return;
}


/**
 * @brief 更新發射填彈的外部時鐘計算
 */
void TURRET::update_timer(void){
    this->_ms_L++; this->_ms_R++;
    this->_fine_tune_ms_L++; this->_fine_tune_ms_R++;
    this->_init_ms++;
    return;
}
