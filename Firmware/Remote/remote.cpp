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
 * @brief 將 PPM 資料傳給 Global 資料。
 */
void REMOTE::ppmValue_to_global(){
     GLOBAL::Chassis_X_Speed = this->ppmValue[0];
     GLOBAL::Chassis_Theta_Speed = this->ppmValue[1];
     //...

    return;
}


/**
 * @brief 紀錄時間戳順序正確的值。
 */
void REMOTE::ppm_raw_to_cnt(void) {
    // PPM 訊號間隔判定，超過 PPM_INTERVAL 視為新週期。
    const uint32_t PPM_INTERVAL = 6000;

    // 1. 先找到起始脈衝的上升緣
    int start = 0;
    for(int i=0; i<17; i++){  
        if(ppmRaw[i+1] - ppmRaw[i] > PPM_INTERVAL) start = i+1;
    } 

    // 2. 確保後續9個脈衝的時間戳連續
    for (int i=0; i<9; i++) {
        int currentIndex = (start + i) % 18;
        int nextIndex = (start + i + 1) % 18;
        if (ppmRaw[nextIndex] < ppmRaw[currentIndex]) {
            start += 9;
            if (start >= 18) start -= 18;
            break;
        }
    }


    // 3. ppmRaw 傳至 ppmCnt
    int cntIndex = 0;    // 紀錄儲存數量
    for(int i=start; i<18; i++){
        ppmCnt[cntIndex] = ppmRaw[i];
        cntIndex++;
        // ppmCnt 只能存儲 9 個值
        if(cntIndex >= 9) break; 
    }
    if(cntIndex < 9){     // 如果 ppmCnt 未滿，則從 ppmRaw[0] 繼續賦值
        for(int i=0; i<9-cntIndex; i++) ppmCnt[cntIndex+i] = ppmRaw[i];
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


/**
 * @brief 將 Buffer 中的 High 值轉換為 Value。
 * @param highIndex 高值索引
 * @return 對應的 Value
 */
void REMOTE::ppm_high_to_value(void) {
    // TODO: 實現 High 到 Value 的轉換邏輯
    return;
}



// 低電平間隔 9.6ms
// 低電平間隔 0.4ms，高電平持續 0.6~1.6ms，0.6/1.1/1.6
// 壓差 1.64V
// 完整週期 ~ 20ms

extern "C" void DMA1_Stream0_IRQHandler(void) {
    // 調用 HAL 庫的中斷處理函式
    HAL_DMA_IRQHandler(&hdma_tim17_ch1);

    Remote.count++;

    Remote.ppm_raw_to_cnt();
    Remote.ppm_cnt_to_high();
    Remote.ppm_high_to_value();

    if(Remote.count%2)__HAL_TIM_SET_COUNTER(&htim17, 0);

    return;
}


