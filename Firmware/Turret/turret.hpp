/*
 * turret.hpp
 *
 *  Created on: Oct 16, 2024
 *      Author: pomelo925
 */

#pragma once

#include "stm32h7xx.h"

class TURRET{
public:
    void global_default_mode(uint8_t device, int mode);

private:
    int _current_mode_left = 0;
    int _current_mode_right = 0;
};

extern TURRET Turret;
