/*
 * motor.hpp
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
   * @param pwmTimer PWM 輸出定時器
   * @param pwmChannel PWM 輸出定時器通道
   * @param dirPort 方向 GPIO 端口
   * @param dirPin 方向 GPIO 引腳
   */
  DC(TIM_HandleTypeDef* pwmTimer, uint32_t pwmChannel, GPIO_TypeDef* dirPort, uint16_t dirPin)
    :_pwmTimer(pwmTimer), _pwmChannel(pwmChannel), _dirPort(dirPort), _dirPin(dirPin){}


  /**
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
   * @param dir 旋轉方向
   */
  DC(TIM_HandleTypeDef* pwmTimer, uint32_t pwmChannel, GPIO_TypeDef* dirPort, uint16_t dirPin,
    TIM_HandleTypeDef* encTimer, float kp, float ki, float kd,
    float res, float ratio, float interval , bool dir)
    :_pwmTimer(pwmTimer), _pwmChannel(pwmChannel), _dirPort(dirPort), _dirPin(dirPin),
     _encTimer(encTimer), _kp(kp), _ki(ki), _kd(kd),
     _encoder_res(res), _sr_ratio(ratio), _interval(interval), _direction(dir){}


  
  void init(void);

  void open_loop_pwm_output(uint16_t duty, GPIO_PinState set);
  void close_loop_pwm_output(void);

  void set_target_wheel_speed(float speed);
  void updateCurrentWheelSpeed(void);
  void updateTargetPWM(void);

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



private:
// REQUIRED PARAMS //
  TIM_HandleTypeDef* _pwmTimer = nullptr; 
  uint32_t _pwmChannel = 0; 
  GPIO_TypeDef* _dirPort = nullptr;
  uint16_t _dirPin = 0;

// OPTIONAL PARAMS (PID Ctrl) //
  TIM_HandleTypeDef* _encTimer = nullptr;
  float _kp=0.f, _ki=0.f, _kd=0.f;  /* PID */
  float _encoder_res=0.f;   /* Encoder resolution */
  float _sr_ratio=0.f;   /* Gear ratio */
  float _interval=0.f;   /* Time interval */ 
  float _error=0.f, _integral=0.f, _error_last=0.f;   /* Error parameters */ 
  bool _direction=false; /* Motor Spin Direction */

// STATE PARAMS (Update Automatically) //
  float _previous_error=0.f;   /* Previous error */
  float _current_wheel_speed=0.f;    /* Current wheel speed (rps). */
  float _target_wheel_speed=0.f;   /* Target wheel speed (rps). Given by Chassis. */
  int16_t _current_pulse=0;  /* Current pulse. Given by Interrupt. */
  float _target_PWM=0.f;  /* PWM target output. Fed to Interrupt. */

  static constexpr float PWM_SCALE = 1000.f;
};


extern DC DC_ChassisR;
extern DC DC_ChassisL;

extern DC DC_SwivelL;
extern DC DC_SwivelR;

extern DC DC_LauncherR1;
extern DC DC_LauncherR2;
extern DC DC_LauncherL1;
extern DC DC_LauncherL2;
