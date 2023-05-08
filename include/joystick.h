// define a new clasa for joystick

/*
  Joystick.h
  
  Copyright (c) 2022, Andrew Fernie

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.
  
  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.
  
  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#pragma once

#include "Arduino.h"
#include <stdint.h>

class Joystick {
   public:
    Joystick(uint8_t pin_x, uint8_t pin_y);
    ~Joystick();
    void Init();
    uint8_t Update();
    uint8_t CalibrateZeroAll();
    uint8_t CalibrateScale();
    uint8_t CalibrateZero(uint8_t axis);
    void SetDeadzone(float deadzone);
    float GetDeadzone();
    void SetScale(uint8_t axis, float scale);
    float GetScale(uint8_t axis);
    float x();
    float y();
    int RawX();
    int RawY();

   private:
    // joystick variables
    uint8_t gpio_pin_x;
    uint8_t gpio_pin_y;
    int zero_x;
    int zero_y;
    int value_int_x;
    int value_int_y;
    float scale_x;
    float scale_y;
    float deadzone;
};
