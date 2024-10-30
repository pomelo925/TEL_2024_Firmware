/*
 * remote.cpp
 *
 *  Created on: Oct 16, 2024
 *      Author: pomelo925
 */

#include <remote.hpp>
#include <global.hpp>

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
        case 1:
            Global.Chassis_X_Speed = REMOTE::joystick_mapping(this->ppmHigh[0], -1.f, 1.f);
            Global.Chassis_Theta_Speed = REMOTE::joystick_mapping(this->ppmHigh[1], -1.f, 1.f);
            Global.Swivel_L_Speed = REMOTE::joystick_mapping(this->ppmHigh[3], -1.f, 1.f);
            Global.Swivel_R_Speed = REMOTE::joystick_mapping(this->ppmHigh[2], -1.f, 1.f);
            break;
        
        case 2:            
            Global.launcher_L1_DUTY = REMOTE::joystick_mapping(this->ppmHigh[1], -100, 100);
            Global.launcher_L2_DUTY = REMOTE::joystick_mapping(this->ppmHigh[1], -100, 100);
            Global.launcher_R1_DUTY = REMOTE::joystick_mapping(this->ppmHigh[3], -100, 100);
            Global.launcher_R2_DUTY = REMOTE::joystick_mapping(this->ppmHigh[3], -100, 100);
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
    if(Global.swa == _SWITCH_UP && Global.swb == _SWITCH_UP) return 1;
    if(Global.swa == _SWITCH_UP && Global.swb == _SWITCH_MIDDLE) return 2;
    if(Global.swa == _SWITCH_UP && Global.swb == _SWITCH_DOWN) return 3;

    if(Global.swa == _SWITCH_DOWN && Global.swb == _SWITCH_UP) return 4;
    if(Global.swa == _SWITCH_DOWN && Global.swb == _SWITCH_MIDDLE) return 5;
    if(Global.swa == _SWITCH_DOWN && Global.swb == _SWITCH_DOWN) return 6;

    return 0;
}



/** 
 * @brief 將 PPM 資料傳給 Global 資料。
 */
void REMOTE::ppmHigh_to_global(){
    Global.swa = REMOTE::switch_mapping(this->ppmHigh[4]);
    Global.swb = REMOTE::switch_mapping(this->ppmHigh[5]);
    Global.swc = REMOTE::switch_mapping(this->ppmHigh[6]);
    Global.swd = REMOTE::switch_mapping(this->ppmHigh[7]);

    Global.mode = REMOTE::switch_mode_mapping();

    REMOTE::global_update();
    return;
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
    }

    if(Remote.count%2) __HAL_TIM_SET_COUNTER(&htim17, 0);

    return;
}


