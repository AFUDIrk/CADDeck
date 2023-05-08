// define a new clasa for AnalogControl

/*
  AnalogControl.h

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

class AnalogControl {
   public:
    AnalogControl(uint8_t pin_a);
    ~AnalogControl();
    void Init();
    uint8_t Update();
    uint8_t CalibrateScale();
    uint8_t CalibrateZero();
    void SetDeadzone(float deadzone);
    float GetDeadzone();
    void SetScale(float scale);
    float GetScale();
    float Value();
    int RawValue();

   private:
    // AnalogControl variables
    uint8_t gpio_pin_a;
    int zero_a;
    int value_int_a;
    float scale_a;
    float deadzone;
};
