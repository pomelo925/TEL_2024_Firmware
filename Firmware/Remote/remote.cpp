/*
 * remote.cpp
 *
 *  Created on: Oct 16, 2024
 *      Author: pomelo925
 */

#include <remote.hpp>
#include <chassis.hpp>
#include <global.hpp>
#include <turret.hpp>
#include <dc.hpp>

extern TIM_HandleTypeDef htim17;
extern DMA_HandleTypeDef hdma_tim17_ch1;

REMOTE Remote;

/** 
 * @brief 初始化 DMA 通道。
 */
void REMOTE::init(){
    __HAL_DMA_ENABLE_IT(&hdma_tim17_ch1, DMA_IT_TC);
	HAL_TIM_IC_Start_DMA(&htim17, TIM_CHANNEL_1, this->ppmRaw, 18);
    
    return;
}


/** 
 * @brief 根據模式執行。
 */
void REMOTE::mode_execute(void){
    switch(this->_mode){
        // swa = up, swd = up => 手動底盤模式，CH1~2粗調，CH3~4精調
        case 1: {
        	DC::freeze_launcher();
            this->_under_case2 = false;

            if( fabs(this->ppmHigh[0]-_PPM_JOYSTICK_DUTY_DEFAULT) > 100 ||
                fabs(this->ppmHigh[1]-_PPM_JOYSTICK_DUTY_DEFAULT) > 100){
            	Chassis.setSpeed(REMOTE::joystick_mapping(this->ppmHigh[1], 1.2f, -1.2f),
            					REMOTE::joystick_mapping(this->ppmHigh[0], 1.f, -1.f));
				break;
            }

        	Chassis.setSpeed(REMOTE::joystick_mapping(this->ppmHigh[3], 0.2f, -0.2f),
        					REMOTE::joystick_mapping(this->ppmHigh[2], 0.5f, -0.5f));
            break;
        }

        // swa = up, swd = down => 自動砲台模式，CH1~2左砲，CH3~4右砲，swb、swc改變就發射        
        case 2: {
            // 先判斷左砲狀態
            switch(REMOTE::joystick_state(_LEFT)){
                // 特定位置、速度
                case _LEFT_TOP:
                    Turret.operate_with_default_mode(_LEFT, _LEFT_TOP); break;
                case _RIGHT_TOP:
                    Turret.operate_with_default_mode(_LEFT, _RIGHT_TOP); break;
                case _LEFT_MIDDLE:
                    Turret.operate_with_default_mode(_LEFT, _LEFT_MIDDLE); break;
                case _RIGHT_MIDDLE:
                    Turret.operate_with_default_mode(_LEFT, _RIGHT_MIDDLE); break;
                case _LEFT_BOTTOM: 
                    Turret.operate_with_default_mode(_LEFT, _LEFT_BOTTOM); break;
                case _RIGHT_BOTTOM:
                    Turret.operate_with_default_mode(_LEFT, _RIGHT_BOTTOM); break;
                
                // 待機狀態
                case _MIDDLE_TOP:
                    Turret.operate_with_default_mode(_LEFT, _MIDDLE_TOP); break;
                case _MIDDLE_MIDDLE:
                    Turret.operate_with_default_mode(_LEFT, _MIDDLE_MIDDLE); break;
                case _MIDDLE_BOTTOM:
                    Turret.operate_with_default_mode(_LEFT, _MIDDLE_BOTTOM); break;

                default:
                    break;
            }

            // 再判斷右砲狀態 
            switch(REMOTE::joystick_state(_RIGHT)){
                // 特定位置、速度
                case _LEFT_TOP:
                    Turret.operate_with_default_mode(_RIGHT, _LEFT_TOP); break;
                case _RIGHT_TOP:
                    Turret.operate_with_default_mode(_RIGHT, _RIGHT_TOP); break;
                case _LEFT_MIDDLE:
                    Turret.operate_with_default_mode(_RIGHT, _LEFT_MIDDLE); break;
                case _RIGHT_MIDDLE:
                    Turret.operate_with_default_mode(_RIGHT, _RIGHT_MIDDLE); break;
                case _LEFT_BOTTOM: 
                    Turret.operate_with_default_mode(_RIGHT, _LEFT_BOTTOM); break;
                case _RIGHT_BOTTOM:
                    Turret.operate_with_default_mode(_RIGHT, _RIGHT_BOTTOM); break;
                                    
                // 待機狀態
                case _MIDDLE_TOP:
                    Turret.operate_with_default_mode(_RIGHT, _MIDDLE_TOP); break;
                case _MIDDLE_MIDDLE:
                    Turret.operate_with_default_mode(_RIGHT, _MIDDLE_MIDDLE); break;
                case _MIDDLE_BOTTOM:
                    Turret.operate_with_default_mode(_RIGHT, _MIDDLE_BOTTOM); break;

                default:
                    break;
            }
            
            // 進入此模式後，先更新當前 swb、swc 狀態
            if(!this->_under_case2) {
                this->swb_state_last = this->_swb;
                this->swc_state_last = this->_swc;
                this->_under_case2 = true;
            }

            // 如果 swb 狀態改變，則發射
            const bool trigger_once_left = ( this->_swb != this->swb_state_last );
            Turret.shoot_and_reload(_LEFT, trigger_once_left);

            this->swb_state_last = this->_swb;
            
            
            // 如果 swc 狀態改變，則發射
            const bool trigger_once_right = ( this->_swc != this->swc_state_last );
            Turret.shoot_and_reload(_RIGHT, trigger_once_right);

            this->swc_state_last = this->_swc;
    
            break;
        }
        

        // swa = down, swd = up => 自動底盤模式，搖桿 swa ~ swd 選模式
        case 3: {
        	DC::freeze_launcher();
        	this->_under_case2 = false;

            // 開頭：左，結束：左
            if(this->_swb == _SWITCH_UP && this->_swc == _SWITCH_UP) Chassis.moveTo(20.f, 20.f);
            // 開頭：左，結束：中
            if(this->_swb == _SWITCH_UP && this->_swc == _SWITCH_MIDDLE) Chassis.moveTo(20.f, -20.f);
            // 開頭：左，結束：右
            if(this->_swb == _SWITCH_UP && this->_swc == _SWITCH_DOWN) Chassis.moveTo(-20.f, 20.f);

            // 開頭：右，結束：左
            if(this->_swb == _SWITCH_DOWN && this->_swc == _SWITCH_UP) Chassis.moveTo(-20.f, -20.f);
            // 開頭：右，結束：中
            if(this->_swb == _SWITCH_DOWN && this->_swc == _SWITCH_MIDDLE) Chassis.moveTo(-20.f, 20.f);
            // 開頭：右，結束：右
            if(this->_swb == _SWITCH_DOWN && this->_swc == _SWITCH_DOWN) Chassis.moveTo(20.f, -20.f);

            break;
        }

        // swa = down, swd = down => 手動砲台模式，微調左右砲位置
        case 4: {
        	this->_under_case2 = false;

            Turret.fine_tune(_LEFT, REMOTE::joystick_mapping(this->ppmHigh[0], -1.f, 1.f), REMOTE::joystick_mapping(this->ppmHigh[1], -1.f, 1.f));
            Turret.fine_tune(_RIGHT, REMOTE::joystick_mapping(this->ppmHigh[2], -1.f, 1.f), REMOTE::joystick_mapping(this->ppmHigh[3], -1.f, 1.f));

            break;
        }

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
    if(this->_swa == _SWITCH_UP && this->_swd == _SWITCH_UP) return 1;
    if(this->_swa == _SWITCH_UP && this->_swd == _SWITCH_DOWN) return 2;
    if(this->_swa == _SWITCH_DOWN && this->_swd == _SWITCH_UP) return 3;
    if(this->_swa == _SWITCH_DOWN && this->_swd == _SWITCH_DOWN) return 4;

    return 0;
}



/** 
 * @brief 將 PPM 資料傳給 Global 資料。
 */
void REMOTE::ppm_high_to_switch_and_mode(void){
    this->_swa = REMOTE::switch_mapping(this->ppmHigh[4]);
    this->_swb = REMOTE::switch_mapping(this->ppmHigh[5]);
    this->_swc = REMOTE::switch_mapping(this->ppmHigh[6]);
    this->_swd = REMOTE::switch_mapping(this->ppmHigh[7]);

    this->_mode = REMOTE::switch_mode_mapping();
    return;
}

/**
 * @brief Switch 撥桿狀態。
 */
uint8_t REMOTE::joystick_state(uint8_t dir){
    const static int LOW = 1200, HIGH = 1800;

    // 檢查左搖桿
    if(dir == _LEFT){
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
    else if(dir == _RIGHT){
        if(this->ppmHigh[2] < LOW){
            if(this->ppmHigh[3] > HIGH) return this->_LEFT_TOP;
            if(this->ppmHigh[3] < LOW) return this->_LEFT_BOTTOM;
            return this->_LEFT_MIDDLE;
        }

        if(this->ppmHigh[2] > HIGH){
            if(this->ppmHigh[3] > HIGH) return this->_RIGHT_TOP;
            if(this->ppmHigh[3] < LOW) return this->_RIGHT_BOTTOM;
            return this->_RIGHT_MIDDLE;
        }

        if(this->ppmHigh[3] > HIGH) return this->_MIDDLE_TOP;
        if(this->ppmHigh[3] < LOW) return this->_MIDDLE_BOTTOM;
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

	// 使遙控器左右搖桿的通道邏輯相同
	uint16_t temp = ppmHigh[2];
	ppmHigh[2] = ppmHigh[3];
	ppmHigh[3] = temp;

    return; 
}


/**
 * @brief DMA 中斷處理函式
 *  記得在 stm32h7xx_it.c 刪除函數，否則會有 First Defined Here 的錯誤。
 */
extern "C" void DMA1_Stream0_IRQHandler(void) {
    HAL_DMA_IRQHandler(&hdma_tim17_ch1);

    Remote.count++;

    if(Remote.count%10){
		Remote.ppm_raw_to_cnt();
		Remote.ppm_cnt_to_high();
		Remote.ppm_high_to_switch_and_mode();
        Remote.mode_execute();
    }

    if(Remote.count%2) __HAL_TIM_SET_COUNTER(&htim17, 0);

    return;
}
