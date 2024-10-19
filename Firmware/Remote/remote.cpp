/*
 * remote.cpp
 *
 *  Created on: Oct 16, 2024
 *      Author: pomelo925
 */

#include <remote.hpp>

REMOTE Remote;

/** 
 * @brief 初始化 DMA 通道。
 */
void REMOTE::init(){
	HAL_TIM_IC_Start_DMA(&htim16, TIM_CHANNEL_1, ppmBuffer, 8);
}


/** 
 * @brief 循環函數。
 */
void REMOTE::loop(){
	while(1){

	}
}


/** 
 * @brief 處理 DMA 轉移的 PPM 資料。
 */
void REMOTE::DMA_transfer_PPMData(){
    if (ppmBuffer != nullptr) {
        ppmData.ch1 = static_cast<uint16_t>(ppmBuffer[0]);
        ppmData.ch2 = static_cast<uint16_t>(ppmBuffer[1]);
        ppmData.ch3 = static_cast<uint16_t>(ppmBuffer[2]);
        ppmData.ch4 = static_cast<uint16_t>(ppmBuffer[3]);
        ppmData.ch5 = static_cast<uint16_t>(ppmBuffer[4]);
        ppmData.ch6 = static_cast<uint16_t>(ppmBuffer[5]);
        ppmData.ch7 = static_cast<uint16_t>(ppmBuffer[6]);
        ppmData.ch8 = static_cast<uint16_t>(ppmBuffer[7]);
    }
}


/** 
 * @brief TIM16 硬體中斷處理函數。捕獲數據觸發中斷。
 */
void TIM16_IRQHandler(){
    HAL_TIM_IRQHandler(&htim16);
    Remote.DMA_transfer_PPMData();
}