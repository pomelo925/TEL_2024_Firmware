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
void TURRET::operate_with_default_mode(uint8_t device, int mode){
    // LEFT
    if(device == _LEFT){
        this->_current_mode_left = mode;

        switch (mode){
        case 1:
        	DC_LauncherL1.set_duty(30);
        	DC_LauncherL2.set_duty(30);
            break;
        
        case 2:
        	DC_LauncherL1.set_duty(30);
        	DC_LauncherL2.set_duty(30);
            break;

        case 4:
        	DC_LauncherL1.set_duty(30);
        	DC_LauncherL2.set_duty(30);
            break;

        case 5:
        	DC_LauncherL1.set_duty(30);
        	DC_LauncherL2.set_duty(30);
            break;
        
        case 7:
        	DC_LauncherL1.set_duty(30);
        	DC_LauncherL2.set_duty(30);
            break;

        case 8:
        	DC_LauncherL1.set_duty(30);
        	DC_LauncherL2.set_duty(30);
            break;

        default:
        	DC_LauncherL1.set_duty(0);
        	DC_LauncherL2.set_duty(0);
            break;
        }
    }

    // RIGHT
    else if(device == _RIGHT){
        this->_current_mode_right = mode;
        
        switch (mode){
        case 1:
        	DC_LauncherR1.set_duty(30);
        	DC_LauncherR2.set_duty(30);
            break;
        
        case 2:
        	DC_LauncherR1.set_duty(30);
        	DC_LauncherR2.set_duty(30);
            break;

        case 4:
        	DC_LauncherR1.set_duty(30);
        	DC_LauncherR2.set_duty(30);
            break;

        case 5:
        	DC_LauncherR1.set_duty(30);
        	DC_LauncherR2.set_duty(30);
            break;
        
        case 7:
        	DC_LauncherR1.set_duty(30);
        	DC_LauncherR2.set_duty(30);
            break;

        case 8:
        	DC_LauncherR1.set_duty(30);
        	DC_LauncherR2.set_duty(30);
            break;

        default:
        	DC_LauncherR1.set_duty(0);
        	DC_LauncherR2.set_duty(0);
            break;
        }
    }
    return;
}

void TURRET::shoot_and_reload(int device){
    if(device == _LEFT){
    	ServoTriggerL.UART_send_pos(1050,4000);
    	ServoTriggerL.UART_send_pos(1300,4000);
    	ServoTriggerL.UART_send_pos(950,4000);
    	Stepper_L.set_goal_pos(-5500);
    	ServoTriggerL.UART_send_pos(1050,4000);
    }
    
    else if(device == _RIGHT){
    	ServoTriggerR.UART_send_pos(1050,4000);
    	ServoTriggerR.UART_send_pos(1300,4000);
    	ServoTriggerR.UART_send_pos(950,4000);
    	Stepper_R.set_goal_pos(-5500);
    	ServoTriggerR.UART_send_pos(1050,4000);
    }

    return;
}

void TURRET::fine_tune(uint8_t device, float swivel_speed, float elevation_speed){
    if(device == _LEFT){
        // swivel
        // elevation
    }
    
    else if(device == _RIGHT){
        // swivel
        // elevation
    }
    
    return;
}
