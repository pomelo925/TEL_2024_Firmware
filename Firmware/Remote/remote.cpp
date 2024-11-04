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
            _case2_flag = false;

            Global.Chassis_X_Speed = REMOTE::joystick_mapping(this->ppmHigh[0], -1.f, 1.f);
            Global.Chassis_Theta_Speed = REMOTE::joystick_mapping(this->ppmHigh[1], -1.f, 1.f);
            Global.Chassis_X_Speed = REMOTE::joystick_mapping(this->ppmHigh[0], -0.2f, 0.2f);
            Global.Chassis_Theta_Speed = REMOTE::joystick_mapping(this->ppmHigh[1], -0.2f, 0.2f);
            break;
        
        // swa = up, swd = down => 自動砲台模式，CH1~2左砲，CH3~4右砲，swb、swc改變就發射        
        case 2:
            // 先判斷左砲狀態
            switch(REMOTE::joystick_state(_LEFT)){
                case _LEFT_TOP:
                    Turret.operate_with_default_mode(_LEFT, 1); break;
                case _RIGHT_TOP:
                    Turret.operate_with_default_mode(_LEFT, 2); break;
                case _LEFT_MIDDLE:
                    Turret.operate_with_default_mode(_LEFT, 4); break;
                case _RIGHT_MIDDLE:
                    Turret.operate_with_default_mode(_LEFT, 5); break;
                case _LEFT_BOTTOM: 
                    Turret.operate_with_default_mode(_LEFT, 7); break;
                case _RIGHT_BOTTOM:
                    Turret.operate_with_default_mode(_LEFT, 8); break;
                default:
                    break;
            }

            // 再判斷右砲狀態 
            switch(REMOTE::joystick_state(_RIGHT)){
                case _LEFT_TOP:
                    Turret.operate_with_default_mode(_RIGHT, 2); break;
                case _RIGHT_TOP:
                    Turret.operate_with_default_mode(_RIGHT, 2); break;
                case _LEFT_MIDDLE:
                    Turret.operate_with_default_mode(_RIGHT, 4); break;
                case _RIGHT_MIDDLE:
                    Turret.operate_with_default_mode(_RIGHT, 5); break;
                case _LEFT_BOTTOM: 
                    Turret.operate_with_default_mode(_RIGHT, 7); break;
                case _RIGHT_BOTTOM:
                    Turret.operate_with_default_mode(_RIGHT, 8); break;
                default:
                    break;
            }
            
            // 進入此模式後，先更新當前 swb、swc 狀態
            static uint8_t swb_state_last = 0, swc_state_last = 0;
            if(!_case2_flag) {
                swb_state_last = Global.swb;
                swc_state_last = Global.swc;
                _case2_flag = true;
            }
            // 如果 swb、swc 狀態改變，則發射
            if(Global.swb != swb_state_last){
                Turret.shoot(_LEFT);
                Turret.reload(_LEFT);
                swb_state_last = Global.swb;
            }
            if(Global.swc != swc_state_last){
                Turret.shoot(_RIGHT);
                Turret.reload(_RIGHT);
                swc_state_last = Global.swc;
            }
    
            break;
        
        // swa = down, swd = up => 自動底盤模式，搖桿 swa ~ swd 選模式
        case 3:
            _case2_flag = false;

            // 開頭：左，結束：左
            if(Global.swb == _SWITCH_UP && Global.swc == _SWITCH_UP) Chassis.moveTo(20.f, 20.f);
            // 開頭：左，結束：中
            if(Global.swb == _SWITCH_UP && Global.swc == _SWITCH_MIDDLE) Chassis.moveTo(20.f, -20.f);
            // 開頭：左，結束：右
            if(Global.swb == _SWITCH_UP && Global.swc == _SWITCH_DOWN) Chassis.moveTo(-20.f, 20.f);

            // 開頭：右，結束：左
            if(Global.swb == _SWITCH_DOWN && Global.swc == _SWITCH_UP) Chassis.moveTo(-20.f, -20.f);
            // 開頭：右，結束：中
            if(Global.swb == _SWITCH_DOWN && Global.swc == _SWITCH_MIDDLE) Chassis.moveTo(-20.f, 20.f);
            // 開頭：右，結束：右
            if(Global.swb == _SWITCH_DOWN && Global.swc == _SWITCH_DOWN) Chassis.moveTo(20.f, -20.f);

            break;

        // swa = down, swd = down => 手動砲台模式，微調左右砲位置
        case 4:
            _case2_flag = false;

            Turret.fine_tune(_LEFT, REMOTE::joystick_mapping(this->ppmHigh[0], -1.f, 1.f), REMOTE::joystick_mapping(this->ppmHigh[1], -1.f, 1.f));
            Turret.fine_tune(_RIGHT, REMOTE::joystick_mapping(this->ppmHigh[2], -1.f, 1.f), REMOTE::joystick_mapping(this->ppmHigh[3], -1.f, 1.f));

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

	// 使遙控器左右搖桿的通道邏輯相同
	uint16_t temp = ppmHigh[2];
	ppmHigh[2] = ppmHigh[3];
	ppmHigh[3] = temp;

    return; 
}


/**
 * @brief DMA 中斷處理函式
 *  記得在 stm32h7xx_it.c 註解函數，否則會有 First Defined Here 的錯誤。
 */
extern "C" void DMA1_Stream0_IRQHandler(void) {
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


