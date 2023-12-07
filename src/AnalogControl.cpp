
/*
  AnalogControl.cpp

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

#include "AnalogControl.h"

AnalogControl::AnalogControl(
    uint8_t pin_a)
{
    gpio_pin_a = pin_a;
    zero_a = 0;
    value_int_a = 0;
    scale_a = 1.0;
    deadzone = 0.0;
}

AnalogControl::~AnalogControl()
{
}

/**
 * Initializes the analog control by setting the pin mode to INPUT and calibrate its zero point.
 */
void AnalogControl::Init()
{
    pinMode(gpio_pin_a, INPUT);
    CalibrateZero();
}

void AnalogControl::SetDeadzone(float dz)
{
    deadzone = dz;
};

float AnalogControl::GetDeadzone()
{
    return deadzone;
};

void AnalogControl::SetScale(float scale)
{
    scale_a = scale;
};

float AnalogControl::GetScale()
{
    return scale_a;
};

uint8_t AnalogControl::Update()
{
    // make sure the pin is valid
    if (gpio_pin_a == 0) {
        return 1;
    }

    // read the AnalogControl
    value_int_a = analogRead(gpio_pin_a);
    return 0;
}

uint8_t AnalogControl::CalibrateZero()
{
    // make sure the pin is valid
    if (gpio_pin_a == 0) {
        return 1;
    }

    // calibrate the AnalogControl mid point
    int in_a = 0;
    for (int i = 0; i < 100; i++) {
        Update();
        in_a += value_int_a;
        delay(10);
    }

    zero_a = in_a / 100;
    value_int_a = zero_a;

    return 0;
}

uint8_t AnalogControl::CalibrateScale()
{
    // make sure the pins are valid
    if (gpio_pin_a == 0) {
        return 1;
    }

    // calibrate the AnalogControl scale
    int max_value_a = -10000;
    int min_value_a = 10000;

    long timestart = millis();

    while (millis() - timestart < 5000) {
        Update();
        if (value_int_a > max_value_a) {
            max_value_a = value_int_a;
        }
        else if (value_int_a < min_value_a) {
            min_value_a = value_int_a;
        }
    }
    scale_a = 2.0 / (float)(max_value_a - min_value_a) * 4095.0;
    return 0;
}

/**
 * Returns the current value of the analog control.
 * @return The current value of the analog control.
 */
float AnalogControl::Value()
{
    float value = (value_int_a - zero_a) * scale_a / 4095.0;
    if (value > deadzone) {
        value = min(max((float)0.0, value - deadzone), (float)1.0);
    }
    else if (value < -deadzone) {
        value = max((float)-1.0, min((float)0.0, value + deadzone));
    }
    else {
        value = 0.0;
    }
    return value;
};

int AnalogControl::RawValue()
{
    return value_int_a;
};