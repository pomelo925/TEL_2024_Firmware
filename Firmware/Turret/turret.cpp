/*
 * turret.cpp
 *
 *  Created on: Oct 16, 2024
 *      Author: pomelo925
 */

#include <turret.hpp>
#include <dc.hpp>

TURRET Turret;

void TURRET::global_default_mode(uint8_t device, int mode){
    // LEFT
    if(device == 3){
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
    else if(device == 4){
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