/*
 * turret.cpp
 *
 *  Created on: Oct 16, 2024
 *      Author: pomelo925
 */

#include <turret.hpp>
#include <stepper.hpp>
#include <servo.hpp>
#include <dc.hpp>

TURRET Turret;

/**
 * @brief 根據預設模式，使 SWIVEL 及 LAUNCHER 以特定位置和速度運行。
 * 
 * @param device 左砲台或右砲台
 * @param mode 不同目標點的模式
 */
void TURRET::operate_with_default_mode(uint8_t device, uint8_t mode){
    // LEFT
    if(device == _LEFT){
        this->_current_mode_left = mode;

        switch (mode){
        case _LEFT_TOP:
            ServoElevatorL.set_goal_deg(50);
        	DC_LauncherL1.set_duty(100);
        	DC_LauncherL2.set_duty(100);
            break;
        
        case _RIGHT_TOP:
            ServoElevatorL.set_goal_deg(50);
        	DC_LauncherL1.set_duty(100);
        	DC_LauncherL2.set_duty(100);
            break;

        case _LEFT_MIDDLE:
            ServoElevatorL.set_goal_deg(50);
        	DC_LauncherL1.set_duty(100);
        	DC_LauncherL2.set_duty(100);
            break;

        case _RIGHT_MIDDLE:
            ServoElevatorL.set_goal_deg(50);
        	DC_LauncherL1.set_duty(100);
        	DC_LauncherL2.set_duty(100);
            break;
        
        case _LEFT_BOTTOM:
            ServoElevatorL.set_goal_deg(50);
        	DC_LauncherL1.set_duty(100);
        	DC_LauncherL2.set_duty(100);
            break;

        case _RIGHT_BOTTOM:
            ServoElevatorL.set_goal_deg(50);
        	DC_LauncherL1.set_duty(100);
        	DC_LauncherL2.set_duty(100);
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
            ServoElevatorR.set_goal_deg(50);
        	DC_LauncherR1.set_duty(100);
        	DC_LauncherR2.set_duty(100);
            break;
        
        case _RIGHT_TOP:
            ServoElevatorR.set_goal_deg(50);
        	DC_LauncherR1.set_duty(100);
        	DC_LauncherR2.set_duty(100);
            break;

        case _LEFT_MIDDLE:
            ServoElevatorR.set_goal_deg(50);
        	DC_LauncherR1.set_duty(100);
        	DC_LauncherR2.set_duty(100);
            break;

        case _RIGHT_MIDDLE:
            ServoElevatorR.set_goal_deg(50);
        	DC_LauncherR1.set_duty(100);
        	DC_LauncherR2.set_duty(100);
            break;
        
        case _LEFT_BOTTOM:
            ServoElevatorR.set_goal_deg(50);
        	DC_LauncherR1.set_duty(100);
        	DC_LauncherR2.set_duty(100);
            break;

        case _RIGHT_BOTTOM:
            ServoElevatorR.set_goal_deg(50);
        	DC_LauncherR1.set_duty(100);
        	DC_LauncherR2.set_duty(100);
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
        ServoTriggerL.UART_send_pos(1100,4000);
        this->_step_L = 1;
        this->_ms_L = 0;
        break;
    
    case 1:
        if (this->_ms_L >= 1000) { // 等待 1 秒
            ServoTriggerL.UART_send_pos(1250, 500);
            this->_step_L = 2;
            this->_ms_L = 0;
        }
        break;
    
    case 2:
        if (this->_ms_L >= 1000) { // 等待 1 秒
            ServoTriggerL.UART_send_pos(950, 500);
            Stepper_L.set_goal_pos(-5500);
            this->_step_L = 3;
            this->_ms_L = 0;
        }
        break;

    case 3:
        if (this->_ms_L >= 1000) { // 等待 1 秒
        	Stepper_L.set_goal_pos(3000);
            ServoTriggerL.UART_send_pos(1100, 500);
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
        ServoTriggerR.UART_send_pos(1050,500);
        this->_step_R = 1;
        this->_ms_R = 0;
        break;

    case 1:
        if (this->_ms_R >= 1000) { // 等待 1 秒
            ServoTriggerR.UART_send_pos(1300, 500);
            this->_step_R = 2;
            this->_ms_R = 0;
        }
        break;

    case 2:
        if (this->_ms_R >= 1000) { // 等待 1 秒
            ServoTriggerR.UART_send_pos(950, 500);
            Stepper_R.set_goal_pos(-5500);
            this->_step_R = 3;
            this->_ms_R = 0;
        }
        break;
    

    case 3:
        if (this->_ms_R >= 1000) { // 等待 1 秒
            ServoTriggerR.UART_send_pos(1050, 500);
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
 */
void TURRET::fine_tune(uint8_t device, float swivel_speed, float elevation_speed){
    if(device == _LEFT){
        // swivel
        DC_SwivelL.set_target_wheel_speed(swivel_speed);
        // elevation
    }
    
    else if(device == _RIGHT){
        // swivel
        // elevation
    }
    
    return;
}


/**
 * @brief 更新發射填彈的外部時鐘計算
 */
void TURRET::update_shoot_and_reload_timer(void){
    if(!this->_work_unit_done_L) this->_ms_L++;
    if(!this->_work_unit_done_R) this->_ms_R++;
    return;
}
