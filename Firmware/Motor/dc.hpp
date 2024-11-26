/*
 * dc.hpp
 *
 *  Created on: Oct 16, 2024
 *      Author: pomelo925
 */

#pragma once

#include "stm32h7xx.h"
#include "stm32h7xx_hal.h"

class DC{
public:
  /**
   * 適用開迴路控制直流馬達
   * @param pwmTimer PWM 輸出定時器
   * @param pwmChannel PWM 輸出定時器通道
   * @param dirPort 方向 GPIO 端口
   * @param dirPin 方向 GPIO 引腳
   */
  DC(TIM_HandleTypeDef* pwmTimer, uint32_t pwmChannel, GPIO_TypeDef* dirPort, uint16_t dirPin)
    :_pwmTimer(pwmTimer), _pwmChannel(pwmChannel), _dirPort(dirPort), _dirPin(dirPin){}


  /**
   * 適用內建閉迴路之直流馬達，編碼器為 ADC 訊號
   * @param pwmTimer PWM 輸出定時器
   * @param pwmChannel PWM 輸出定時器通道
   * @param dirPort 方向 GPIO 端口
   * @param dirPin 方向 GPIO 引腳
   * @param breakPort 煞車 GPIO 端口
   * @param breakPin 煞車 GPIO 引腳
   */

  DC(TIM_HandleTypeDef* pwmTimer, uint32_t pwmChannel, GPIO_TypeDef* dirPort, uint16_t dirPin,
	  GPIO_TypeDef* breakPort, uint16_t breakPin)
    :_pwmTimer(pwmTimer), _pwmChannel(pwmChannel),  _dirPort(dirPort), _dirPin(dirPin),
	    _breakPort(breakPort), _breakPin(breakPin){
  }


  /**
   * 適用閉迴路控制直流馬達
   * @param pwmTimer PWM 輸出定時器
   * @param pwmChannel PWM 輸出定時器通道
   * @param dirPort 方向 GPIO 端口
   * @param dirPin 方向 GPIO 引腳
   * @param encTimer 編碼器定時器
   * @param kp 比例增益
   * @param ki 積分增益
   * @param kd 微分增益
   * @param res 編碼器解析度
   * @param ratio 齒輪比
   * @param interval 時間間隔
   */
  DC(TIM_HandleTypeDef* pwmTimer, uint32_t pwmChannel, GPIO_TypeDef* dirPort, uint16_t dirPin,
    TIM_HandleTypeDef* encTimer, float kp, float ki, float kd,
    float res, float ratio, float interval)
    :_pwmTimer(pwmTimer), _pwmChannel(pwmChannel), _dirPort(dirPort), _dirPin(dirPin),
     _encTimer(encTimer), _kp(kp), _ki(ki), _kd(kd),
     _encoder_res(res), _sr_ratio(ratio), _interval(interval){}

  
  static void freeze_launcher(void);
  void init(void);
  
  void open_loop_pwm_output(void);
  void close_loop_pwm_output(void);
  void close_loop_pos(float pos);

  void set_target_wheel_speed(float speed);
  void update_current_wheel_speed(void);
  void update_target_PWM(void);

  TIM_HandleTypeDef* getEncTimer(void) const {return _encTimer;}
  TIM_HandleTypeDef* getPwmTimer(void) const {return _pwmTimer;}
  uint32_t getPwmChannel(void) const {return _pwmChannel;}
  GPIO_TypeDef* getDirPort(void) const {return _dirPort;}
  uint16_t getDirPin(void) const {return _dirPin;}
  GPIO_PinState getDir(void) const {
    return _direction ? GPIO_PIN_SET : GPIO_PIN_RESET;
  }
  float get_current_wheel_speed(void) const {return _current_wheel_speed;}
  float get_target_pwm(void) const {return _target_PWM;}
  void set_duty(float duty){_duty = duty;}


private:
// REQUIRED PARAMS //
  TIM_HandleTypeDef* _pwmTimer = nullptr; 
  uint32_t _pwmChannel = 999;
  GPIO_TypeDef* _dirPort = nullptr;
  uint16_t _dirPin = 999;

// OPTIONAL PARAMS //
  GPIO_TypeDef* _encPort = nullptr;
  uint16_t _encPin = 999;
  TIM_HandleTypeDef* _encTimer = nullptr;
  uint32_t _encChannel = 999;
  GPIO_TypeDef* _breakPort = nullptr;
  uint16_t _breakPin = 999;

  float _kp=0.f, _ki=0.f, _kd=0.f;  /* PID */
  float _encoder_res=0.f;   /* Encoder resolution */
  float _sr_ratio=0.f;   /* Gear ratio */
  float _interval=0.f;   /* Time interval */ 
  float _error=0.f, _integral=0.f, _error_last=0.f;   /* Error parameters */ 
  bool _direction=false; /* Motor Spin Direction */
  float _duty=0.f;

// STATE PARAMS (Update Automatically) //
  float _previous_error=0.f;   /* Previous error */
  float _current_wheel_speed=0.f;    /* Current wheel speed (rps). */
  float _target_wheel_speed=0.f;   /* Target wheel speed (rps). Given by Chassis. */
  int16_t _current_pulse=0;  /* Current pulse. Given by Interrupt. */
  float _target_PWM=0.f;  /* PWM target output. Fed to Interrupt. */

  static constexpr float PWM_SCALE = 999.f;
  
  // ADC
  static uint32_t _dc_swivel_adc[2];
};


extern DC DC_ChassisR;
extern DC DC_ChassisL;

extern DC DC_SwivelL;
extern DC DC_SwivelR;

extern DC DC_LauncherR1;
extern DC DC_LauncherR2;
extern DC DC_LauncherL1;
extern DC DC_LauncherL2;
