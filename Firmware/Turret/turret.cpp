/*
 * turret.cpp
 *
 *  Created on: Oct 16, 2024
 *      Author: pomelo925
 */

#include <turret.hpp>
#include <stepper.hpp>
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
            DC_SwivelL.close_loop_pos(0);
            DC_LauncherL1.open_loop_pwm_output(0);
            DC_LauncherL2.open_loop_pwm_output(0);
            
            break;
        
        case 2:
            break;

        case 4:
            break;

        case 5:
            break;
        
        case 7:
            break;

        case 8:
            break;

        default:
            break;
        }
    }

    // RIGHT
    else if(device == _RIGHT){
        this->_current_mode_right = mode;
        
        switch (mode){
        case 1:
            DC_SwivelR.close_loop_pos(0);
            DC_LauncherR1.open_loop_pwm_output(0);
            DC_LauncherR2.open_loop_pwm_output(0);
            
            break;
        
        case 2:
            break;

        case 4:
            break;

        case 5:
            break;
        
        case 7:
            break;

        case 8:
            break;

        default:
            break;
        }
    }
    return;
}

void TURRET::shoot(int device){
    if(device == _LEFT){
        // trigger

    }
    
    else if(device == _RIGHT){
        // trigger
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


void TURRET::reload(uint8_t device){
    if(device == _LEFT) Stepper_L.set_goal_pos(-5500);
    else if(device == _RIGHT) Stepper_R.set_goal_pos(-5500);
    return;
}
