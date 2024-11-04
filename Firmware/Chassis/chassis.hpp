/*
 * chassis.hpp
 *
 *  Created on: Oct 18, 2024
 *      Author: pomelo925
 */

#pragma once

class CHASSIS{
public:
  void run(float GLOBAL_x, float GLOBAL_theta);
  void moveTo(float x, float y);
  
private:
  struct SpeedVector{ 
    float x = 0.f;
    float theta = 0.f;
  };

  SpeedVector CURRENT_SPEED, TARGET_SPEED;

  /* Kinectics Arguments (mks)*/
  const float _wheel_distance = 0.2f;
  const float _wheel_radius = 0.05f;

  /* functions */
  void update_target_wheel_speed(void);
  void update_current_chassis_speed(void);
};

extern CHASSIS Chassis;
