
/*
  Joystick.cpp

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

#include "Joystick.h"

Joystick::Joystick(
    uint8_t pin_x,
    uint8_t pin_y)
{
    gpio_pin_x = pin_x;
    gpio_pin_y = pin_y;
    zero_x = 0;
    zero_y = 0;
    value_int_x = 0;
    value_int_y = 0;
    scale_x = 1.0;
    scale_y = 1.0;
    deadzone = 0.0;
}

Joystick::~Joystick()
{
}

void Joystick::Init()
{
    
    pinMode(gpio_pin_x, INPUT);
    pinMode(gpio_pin_y, INPUT);
    CalibrateZeroAll();
}

void Joystick::SetDeadzone(float dz)
{
    deadzone = dz;
};

float Joystick::GetDeadzone()
{
    return deadzone;
};

void Joystick::SetScale(uint8_t axis, float scale)
{
    if (axis == 0) {
        scale_x = scale;
    }
    else {
        scale_y = scale;
    }
};

float Joystick::GetScale(uint8_t axis)
{
    if (axis == 0) {
        return scale_x;
    }
    else {
        return scale_y;
    }
};

uint8_t Joystick::Update()
{
    // make sure the pins are valid
    if (gpio_pin_x == 0 || gpio_pin_y == 0) {
        return 1;
    }

    // read the joystick
    value_int_x = analogRead(gpio_pin_x);
    value_int_y = analogRead(gpio_pin_y);
    return 0;
}

uint8_t Joystick::CalibrateZeroAll()
{
    uint8_t status = 0;
    
    if(CalibrateZero(0) != 0) {
        status = 1;
    }
    if (CalibrateZero(1) != 0) {
        status = 1;
    }

    return status;
}

uint8_t Joystick::CalibrateZero(uint8_t axis)
{
    // make sure the pins are valid
    if (((axis == 0) && (gpio_pin_x == 0)) || ((axis == 1) && (gpio_pin_y == 0))) {
        return 1;
    }

    // calibrate the joystick mid point
    int in_x = 0;
    int in_y = 0;
    for (int i = 0; i < 100; i++) {
        Update();
        in_x += value_int_x;
        in_y += value_int_y;
        delay(10);
    }

    if (axis == 0) {
        zero_x = in_x / 100;
        value_int_x = zero_x;
    }
    else {
        zero_y = in_y / 100;
        value_int_y = zero_y;
    }

    return 0;
}

uint8_t Joystick::CalibrateScale()
{
    // make sure the pins are valid
    if ((gpio_pin_x == 0) || (gpio_pin_y == 0)) {
        return 1;
    }

    // calibrate the joystick scale
    int max_value_x = -10000;
    int min_value_x = 10000;
    int max_value_y = -10000;
    int min_value_y = 10000;

    long timestart = millis();

    while (millis() - timestart < 5000) {
        Update();
        if (value_int_x > max_value_x) {
            max_value_x = value_int_x;
        }
        else if (value_int_x < min_value_x) {
            min_value_x = value_int_x;
        }
        if (value_int_y > max_value_y) {
            max_value_y = value_int_y;
        }
        else if (value_int_y < min_value_y) {
            min_value_y = value_int_y;
        }
    }
    scale_x = 2.0 / (float)(max_value_x - min_value_x) * 4095.0;
    scale_y = 2.0 / (float)(max_value_y - min_value_y) * 4095.0;
    return 0;
}

float Joystick::x()
{
    float value = (value_int_x - zero_x) * scale_x / 4095.0;
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

int Joystick::RawX(){
    return value_int_x;
};

int Joystick::RawY(){
    return value_int_y;
};

float Joystick::y()
{
    float value = (value_int_y - zero_y) * scale_y / 4095.0;
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
