/*
 * remote.cpp
 *
 *  Created on: Oct 16, 2024
 *      Author: pomelo925
 */

#include <remote.hpp>
#include <global.hpp>
#include <turret.hpp>

extern TIM_HandleTypeDef htim17;
extern DMA_HandleTypeDef hdma_tim17_ch1;

REMOTE Remote;

/** 
 * @brief 初始化 DMA 通道。
 */
void REMOTE::init(){
    __HAL_DMA_ENABLE_IT(&hdma_tim17_ch1, DMA_IT_TC);
	HAL_TIM_IC_Start_DMA(&htim17, TIM_CHANNEL_1, this->ppmRaw, 18);
}


/** 
 * @brief Global 更新函數，根據 Global.mode。
 */
void REMOTE::global_update(void){
    switch(Global.mode){
        // swa = up, swd = up => 手動底盤模式，CH1~2粗調，CH3~4精調
        case 1:
            Global.Chassis_X_Speed = REMOTE::joystick_mapping(this->ppmHigh[0], -1.f, 1.f);
            Global.Chassis_Theta_Speed = REMOTE::joystick_mapping(this->ppmHigh[1], -1.f, 1.f);
            Global.Chassis_X_Speed = REMOTE::joystick_mapping(this->ppmHigh[0], -0.2f, 0.2f);
            Global.Chassis_Theta_Speed = REMOTE::joystick_mapping(this->ppmHigh[1], -0.2f, 0.2f);
            break;
        
        // swa = up, swd = down => 自動砲台模式，CH1~2左砲，CH3~4右砲
        case 2:
            switch(REMOTE::joystick_state(_LEFT)){
                case _LEFT_TOP:
                    Turret.global_default_mode(_LEFT, 1); break;
                case _RIGHT_TOP:
                    Turret.global_default_mode(_LEFT, 2); break;
                case _LEFT_MIDDLE:
                    Turret.global_default_mode(_LEFT, 4); break;
                case _RIGHT_MIDDLE:
                    Turret.global_default_mode(_LEFT, 5); break;
                case _LEFT_BOTTOM: 
                    Turret.global_default_mode(_LEFT, 7); break;
                case _RIGHT_BOTTOM:
                    Turret.global_default_mode(_LEFT, 8); break;
                default:
                    break;
            }

            switch(REMOTE::joystick_state(_RIGHT)){
                case _LEFT_TOP:
                    Turret.global_default_mode(_RIGHT, 2); break;
                case _RIGHT_TOP:
                    Turret.global_default_mode(_RIGHT, 2); break;
                case _LEFT_MIDDLE:
                    Turret.global_default_mode(_RIGHT, 4); break;
                case _RIGHT_MIDDLE:
                    Turret.global_default_mode(_RIGHT, 5); break;
                case _LEFT_BOTTOM: 
                    Turret.global_default_mode(_RIGHT, 7); break;
                case _RIGHT_BOTTOM:
                    Turret.global_default_mode(_RIGHT, 8); break;
                default:
                    break;
            }

            break;
        
        // swa = down, swd = up => 
        case 3:

            break;

        // swa = down, swd = down => 手動砲台模式，微調左右砲位置
        case 4:

            break;


        default:
            break;
    }
    return;
}


/** 
 * @brief PPM 映射函數，Joystick 類比搖桿。
 */
float REMOTE::joystick_mapping(uint16_t ppmHigh, float output_min, float output_max){
    if(ppmHigh < _PPM_JOYSTICK_DUTY_MIN) return output_min;
    if(ppmHigh > _PPM_JOYSTICK_DUTY_MAX) return output_max;
    return output_min + (output_max - output_min) * (ppmHigh - _PPM_JOYSTICK_DUTY_MIN) / (_PPM_JOYSTICK_DUTY_MAX - _PPM_JOYSTICK_DUTY_MIN);
}


/** 
 * @brief PPM 映射函數，Switch 撥桿。
 */
uint8_t REMOTE::switch_mapping(uint16_t ppmHigh){
    if(ppmHigh < _PPM_SWITCH_DUTY_MIN) return _SWITCH_UP;
    if(ppmHigh > _PPM_SWITCH_DUTY_MAX) return _SWITCH_DOWN;
    return _SWITCH_MIDDLE;
}


/** 
 * @brief Switch 撥桿與模式映射的函數。
 */
uint8_t REMOTE::switch_mode_mapping(void){
    if(Global.swa == _SWITCH_UP && Global.swd == _SWITCH_UP) return 1;
    if(Global.swa == _SWITCH_UP && Global.swd == _SWITCH_DOWN) return 2;
    if(Global.swa == _SWITCH_DOWN && Global.swd == _SWITCH_UP) return 3;
    if(Global.swa == _SWITCH_DOWN && Global.swd == _SWITCH_DOWN) return 4;

    return 0;
}



/** 
 * @brief 將 PPM 資料傳給 Global 資料。
 */
void REMOTE::ppmHigh_to_global(void){
    Global.swa = REMOTE::switch_mapping(this->ppmHigh[4]);
    Global.swb = REMOTE::switch_mapping(this->ppmHigh[5]);
    Global.swc = REMOTE::switch_mapping(this->ppmHigh[6]);
    Global.swd = REMOTE::switch_mapping(this->ppmHigh[7]);

    Global.mode = REMOTE::switch_mode_mapping();
    return;
}

/**
 * @brief Switch 撥桿狀態。
 */
uint8_t REMOTE::joystick_state(uint8_t dir){
    const static int LOW = 1200, HIGH = 1800;

    // 檢查左搖桿
    if(dir == this->_LEFT){
        if(this->ppmHigh[0] < LOW){
            if(this->ppmHigh[1] > HIGH) return this->_LEFT_TOP;
            if(this->ppmHigh[1] < LOW) return this->_LEFT_BOTTOM;
            return this->_LEFT_MIDDLE;
        }
        
        if(this->ppmHigh[0] > HIGH){
            if(this->ppmHigh[1] > HIGH) return this->_RIGHT_TOP;
            if(this->ppmHigh[1] < LOW) return this->_RIGHT_BOTTOM;
            return this->_RIGHT_MIDDLE;
        }

        if(this->ppmHigh[1] > HIGH) return this->_MIDDLE_TOP;
        if(this->ppmHigh[1] < LOW) return this->_MIDDLE_BOTTOM;
        return this->_MIDDLE_MIDDLE;
    }

    // 檢查右搖桿
    if(dir == this->_RIGHT){
        if(this->ppmHigh[3] < LOW){
            if(this->ppmHigh[2] > HIGH) return this->_LEFT_TOP;
            if(this->ppmHigh[2] < LOW) return this->_LEFT_BOTTOM;
            return this->_LEFT_MIDDLE;
        }

        if(this->ppmHigh[3] > HIGH){
            if(this->ppmHigh[2] > HIGH) return this->_RIGHT_TOP;
            if(this->ppmHigh[2] < LOW) return this->_RIGHT_BOTTOM;
            return this->_RIGHT_MIDDLE;
        }

        if(this->ppmHigh[2] > HIGH) return this->_MIDDLE_TOP;
        if(this->ppmHigh[2] < LOW) return this->_MIDDLE_BOTTOM;
        return this->_MIDDLE_MIDDLE;
    }

    return 0;
}


/**
 * @brief 紀錄時間戳順序正確的值。
 */
void REMOTE::ppm_raw_to_cnt(void) {
	// 低電平間隔 9.6ms
	// 低電平間隔 0.4ms，高電平持續 0.6~1.6ms，0.6/1.1/1.6
	// 壓差 1.64V
	// 完整週期 ~ 20ms

    // 1. 先找到起始脈衝的上升緣
    int start = 0;
    for(int i=0; i<17; i++){  
        if(this->ppmRaw[i+1] - this->ppmRaw[i] > this->_PPM_INTERVAL) start = i+1;
    } 

    // 2. 確保後續9個脈衝的時間戳連續
    for (int i=0; i<9; i++) {
        int currentIndex = (start + i) % 18;
        int nextIndex = (start + i + 1) % 18;
        if (this->ppmRaw[nextIndex] < this->ppmRaw[currentIndex]) {
            start += 9;
            if (start >= 18) start -= 18;
            break;
        }
    }


    // 3. ppmRaw 傳至 ppmCnt
    int cntIndex = 0;    // 紀錄儲存數量
    for(int i=start; i<18; i++){
        this->ppmCnt[cntIndex] = this->ppmRaw[i];
        cntIndex++;
        // ppmCnt 只能存儲 9 個值
        if(cntIndex >= 9) break; 
    }
    if(cntIndex < 9){     // 如果 ppmCnt 未滿，則從 ppmRaw[0] 繼續賦值
        for(int i=0; i<9-cntIndex; i++) this->ppmCnt[cntIndex+i] = this->ppmRaw[i];
    }
    
    return;
}


/**
 * @brief 將 Buffer 中的脈衝寬度轉換為 High 值。
 * @param cnt TIMER CNT 值，對應上升緣時刻
 * @return 對應的 High 值
 */
void REMOTE::ppm_cnt_to_high(void) {
	for(int i=0; i<8; i++)
		this->ppmHigh[i] = this->ppmCnt[i+1] - this->ppmCnt[i];
    return; 
}



extern "C" void DMA1_Stream0_IRQHandler(void) {
    // 調用 HAL 庫的中斷處理函式
    HAL_DMA_IRQHandler(&hdma_tim17_ch1);

    Remote.count++;

    if(Remote.count%10){
		Remote.ppm_raw_to_cnt();
		Remote.ppm_cnt_to_high();
		Remote.ppmHigh_to_global();
        Remote.global_update();
    }

    if(Remote.count%2) __HAL_TIM_SET_COUNTER(&htim17, 0);

    return;
}


