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
    Stepper_L.set_goal_pos(1000);
    while(TURRET::_init_ms < 2000){};  // 等待 2 秒

    ServoTriggerR.UART_send_pos(1150, 100);
    ServoTriggerL.UART_send_pos(1900, 100);
    while(TURRET::_init_ms < 5000){};  // 等待 3 秒

    Stepper_R.set_goal_pos(-13000);
    Stepper_L.set_goal_pos(-26000);
    while(TURRET::_init_ms < 13000){};  // 等待 8 秒

    Stepper_R.set_goal_pos(6000);
    Stepper_L.set_goal_pos(12000);
    return;
}


/**
 * @brief 運行除錯模式
 * 
 */
void TURRET::operate_with_debug_mode(void){
    DC_LauncherL1.set_duty(-_DC_LAUNCHER_DUTY_L);
    DC_LauncherL2.set_duty(-_DC_LAUNCHER_DUTY_L);
    DC_LauncherR1.set_duty(_DC_LAUNCHER_DUTY_R);
    DC_LauncherR2.set_duty(_DC_LAUNCHER_DUTY_R);

    ServoElevatorL.set_goal_deg(_DEBUG_MODE_ELEVATOR_L);
    ServoElevatorR.set_goal_deg(_DEBUG_MODE_ELEVATOR_R);

    return;
}


/**
 * @brief 根據預設模式，使 SWIVEL 及 LAUNCHER 以特定位置和速度運行。
 * 
 * @param device 左砲台或右砲台
 * @param mode 不同目標點的模式
 */
void TURRET::operate_with_default_mode(uint8_t device, uint8_t mode){
	const uint8_t max_duty = 50;

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
        DC_LauncherR1.set_duty(max_duty);
        DC_LauncherR2.set_duty(max_duty);
        
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
        ServoTriggerL.UART_send_pos(1900, 10);
        this->_step_L = 1;
        this->_ms_L = 0;
        break;

    case 1:
        if (this->_ms_L >= 300) { // 等待 0.3 秒
            ServoTriggerL.UART_send_pos(1800, 1);
            this->_step_L = 2;
            this->_ms_L = 0;
        }
        break;

    case 2:
        if (this->_ms_L >= 300) { // 等待 0.3 秒
            ServoTriggerL.UART_send_pos(2110, 100);
            this->_step_L = 3;
            this->_ms_L = 0;
        }
        break;
    

    case 3:
        if (this->_ms_L >= 300) { // 等待 0.3 秒
            Stepper_L.set_goal_pos(-11600);
            this->_step_L = 4;
            this->_ms_L = 0;
        }
        break;
    
    case 4:
        if (this->_ms_L >= 3200) { // 等待 3.2 秒
            Stepper_L.set_goal_pos(10000);
            this->_step_L = 5;
            this->_ms_L = 0;
        }
        break;

    case 5:
        if (this->_ms_L >= 3000) { // 等待 3 秒
            ServoTriggerL.UART_send_pos(2040, 500);
            this->_step_L = 6;
            this->_ms_L = 0;
        }
        break;

    case 6:
        if (this->_ms_L >= 600) { // 等待 0.6 秒
            ServoTriggerL.UART_send_pos(1970, 500);
            this->_step_L = 7;
            this->_ms_L = 0;
        }
        break;

    case 7:
        if (this->_ms_L >= 600) { // 等待 0.6 秒
            ServoTriggerL.UART_send_pos(1900, 500);
            this->_step_L = 8;
            this->_ms_L = 0;
        }
        break;
        
    case 8:
        // 第九次發射，步進馬達不再往前
        if (this->_shoot_and_reload_count_L == 8) break;

        if (this->_ms_L >= 3000) { // 等待 3 秒
            Stepper_L.set_goal_pos(-10000);
            this->_step_L = 0;
            this->_ms_L = 0;
            this->_work_unit_done_L = true;
            this->_shoot_and_reload_count_L++;
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
        ServoTriggerR.UART_send_pos(1150, 10);
        this->_step_R = 1;
        this->_ms_R = 0;
        break;

    case 1:
        if (this->_ms_R >= 300) { // 等待 0.3 秒
            ServoTriggerR.UART_send_pos(1250, 1);
            this->_step_R = 2;
            this->_ms_R = 0;
        }
        break;

    case 2:
        if (this->_ms_R >= 300) { // 等待 0.3 秒
            ServoTriggerR.UART_send_pos(970, 100);
            this->_step_R = 3;
            this->_ms_R = 0;
        }
        break;
    

    case 3:
        if (this->_ms_R >= 300) { // 等待 0.3 秒
            Stepper_R.set_goal_pos(-5800);
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
            ServoTriggerR.UART_send_pos(1000, 2500);
            this->_step_R = 6;
            this->_ms_R = 0;
        }
        break;
    
    case 6:
        if (this->_ms_R >= 600) { // 等待 0.6 秒
            ServoTriggerR.UART_send_pos(1070, 500);
            this->_step_R = 7;
            this->_ms_R = 0;
        }
        break;

    case 7:
        if (this->_ms_R >= 600) { // 等待 0.6 秒
            ServoTriggerR.UART_send_pos(1150, 500);
            this->_step_R = 8;
            this->_ms_R = 0;
        }
        break;

    case 8:
        // 第九次發射，步進馬達不再往前
        if (this->_shoot_and_reload_count_R == 8) break;

        if (this->_ms_R >= 3000) { // 等待 3 秒
            Stepper_R.set_goal_pos(-5000);
            this->_step_R = 0;
            this->_ms_R = 0;
            this->_work_unit_done_R = true;
            this->_shoot_and_reload_count_R++;
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
void TURRET::fine_tune_swivel_elevation(uint8_t device, float swivel_duty, float elevation_angle){
    const float trigger_duty = 10.f;  // 觸發動作的占空比

    const float swivel_working_duty_L = 75;  // 轉盤的工作占空比
    const float swivel_working_duty_R = 75; 

    const uint8_t elevation_unit_step = 2;  // 仰角每次調整的角度

    if(device == _LEFT){
        /** Swivel **/ 
        if(abs(swivel_duty) < trigger_duty) DC_SwivelL.set_duty(100);

        else if(swivel_duty > trigger_duty) DC_SwivelL.set_duty(-swivel_working_duty_L);
        else DC_SwivelL.set_duty(swivel_working_duty_L);

        /** Elevation **/ 
        // 每秒執行一次
        static uint32_t fine_tune_elevation_ms_last_L = 0;
        if(this->_fine_tune_elevation_ms_L - fine_tune_elevation_ms_last_L < 200) return;
        fine_tune_elevation_ms_last_L = this->_fine_tune_elevation_ms_L;

        // elevation
        if(elevation_angle > trigger_duty) Turret.tune_debug_mode_elevator_deg(_LEFT, elevation_unit_step);
        if(elevation_angle < -trigger_duty) Turret.tune_debug_mode_elevator_deg(_LEFT, -elevation_unit_step);
    }
    
    else if(device == _RIGHT){
        /** Swivel **/ 
        if(abs(swivel_duty) < trigger_duty) DC_SwivelR.set_duty(100);

        else if(swivel_duty > trigger_duty) DC_SwivelR.set_duty(swivel_working_duty_R);
        else DC_SwivelR.set_duty(-swivel_working_duty_R);

        /** Elevation **/ 
        // 每秒執行一次
        static uint32_t fine_tune_elevation_ms_last_R = 0;
        if(this->_fine_tune_elevation_ms_R - fine_tune_elevation_ms_last_R < 200) return;
        fine_tune_elevation_ms_last_R = this->_fine_tune_elevation_ms_R;

        if(elevation_angle > trigger_duty) Turret.tune_debug_mode_elevator_deg(_RIGHT, elevation_unit_step);
        if(elevation_angle < -trigger_duty) Turret.tune_debug_mode_elevator_deg(_RIGHT, -elevation_unit_step);
    }

    return;
}


/**
 * @brief 細調發射器轉速
 * @param device 左砲台或右砲台
 * @param launcher_duty 發射器轉速占空比（%）
 * @param stepper_angle 步進馬達角度（°）
 */
void TURRET::fine_tune_launcher_stepper(uint8_t device, float launcher_duty, float stepper_angle){
    const float trigger_duty = 10.f;  // 觸發動作的占空比

    const uint32_t launcher_unit_step = 5;  // 發射器每次調整的轉速

    const int step_unit_L = 1600;  // 步進馬達每次調整的角度
    const int step_unit_R = 800;


    if(abs(launcher_duty) < trigger_duty && abs(stepper_angle) < trigger_duty) return;

    if(device == _LEFT){
        // 每秒執行一次
        static uint32_t fine_tune_launcher_stepper_ms_last_L = 0;
        if(this->_fine_tune_launcher_stepper_ms_L - fine_tune_launcher_stepper_ms_last_L < 200)  return;
        fine_tune_launcher_stepper_ms_last_L = this->_fine_tune_launcher_stepper_ms_L;

        /** DC Launcher **/
        if(launcher_duty > trigger_duty) this->_DC_LAUNCHER_DUTY_L += launcher_unit_step;
        else this->_DC_LAUNCHER_DUTY_L -= launcher_unit_step;

        if(this->_DC_LAUNCHER_DUTY_L < 0) this->_DC_LAUNCHER_DUTY_L = 0;
        if(this->_DC_LAUNCHER_DUTY_L > 100) this->_DC_LAUNCHER_DUTY_L = 100;

        DC_LauncherL1.set_duty(-this->_DC_LAUNCHER_DUTY_L);
        DC_LauncherL2.set_duty(-this->_DC_LAUNCHER_DUTY_L);

        /** Stepper **/
        if(stepper_angle > trigger_duty) Stepper_L.set_goal_pos(step_unit_L);
        if(stepper_angle < -trigger_duty) Stepper_L.set_goal_pos(-step_unit_L);
    }
    else if(device == _RIGHT){
        // 每秒執行一次
        static uint32_t fine_tune_launcher_stepper_ms_last_R = 0;
        if(this->_fine_tune_launcher_stepper_ms_R - fine_tune_launcher_stepper_ms_last_R < 200)  return;
        fine_tune_launcher_stepper_ms_last_R = this->_fine_tune_launcher_stepper_ms_R;

        /** DC Launcher **/
        if(launcher_duty > trigger_duty) this->_DC_LAUNCHER_DUTY_R += launcher_unit_step;
        else this->_DC_LAUNCHER_DUTY_R -= launcher_unit_step;

        if(this->_DC_LAUNCHER_DUTY_R < 0) this->_DC_LAUNCHER_DUTY_R = 0;
        if(this->_DC_LAUNCHER_DUTY_R > 100) this->_DC_LAUNCHER_DUTY_R = 100;

        DC_LauncherR1.set_duty(this->_DC_LAUNCHER_DUTY_R);
        DC_LauncherR2.set_duty(this->_DC_LAUNCHER_DUTY_R);

        /** Stepper **/
        if(stepper_angle > trigger_duty) Stepper_R.set_goal_pos(-step_unit_R);
        if(stepper_angle < -trigger_duty) Stepper_R.set_goal_pos(step_unit_R);
    }
    return;
}


/**
 * @brief 微調除錯模式的仰角
 * 
 * @param device 左砲台或右砲台
 * @param step 步數
 */
void TURRET::tune_debug_mode_elevator_deg(uint8_t device, uint32_t step){
    if(device == _LEFT){
        _DEBUG_MODE_ELEVATOR_L += step;
        if(_DEBUG_MODE_ELEVATOR_L > 98) _DEBUG_MODE_ELEVATOR_L = 98;
        if(_DEBUG_MODE_ELEVATOR_L < 2) _DEBUG_MODE_ELEVATOR_L = 2;
        ServoElevatorL.set_goal_deg(_DEBUG_MODE_ELEVATOR_L);
    }
    
    else if(device == _RIGHT){
        _DEBUG_MODE_ELEVATOR_R += step;
        if(_DEBUG_MODE_ELEVATOR_R > 98) _DEBUG_MODE_ELEVATOR_R = 98;
        if(_DEBUG_MODE_ELEVATOR_R < 2) _DEBUG_MODE_ELEVATOR_R = 2;
        ServoElevatorR.set_goal_deg(_DEBUG_MODE_ELEVATOR_R);
    }
    return;
}


/**
 * @brief 微調預設模式的仰角
 * 
 * @param device 左砲台或右砲台
 * @param step 步數
 */
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
    this->_init_ms++;
    this->_ms_L++;
    this->_ms_R++;
    this->_fine_tune_launcher_stepper_ms_L++;
    this->_fine_tune_launcher_stepper_ms_R++;
    this->_fine_tune_elevation_ms_L++;
    this->_fine_tune_elevation_ms_R++;
    return;
}