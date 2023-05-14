/*
  DenoiseComparison.cpp

  Copyright (c) 2023, Andrew Fernie

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

#include "DenoiseComparison.h"

DenoiseComparison::DenoiseComparison(uint16_t period, float value, float tolerance)
{
    tolerance_value = tolerance;
    comparison_value = value;
    period_required = period;
    period_condition_met = 0;  
}

DenoiseComparison::~DenoiseComparison()
{
}

void DenoiseComparison::Reset()
{
    period_condition_met = 0;
}

uint8_t DenoiseComparison::Equals(float value){
    if (abs(value - comparison_value) < tolerance_value){
        period_condition_met++;
    } else {
        period_condition_met = 0;
    }
    if (period_condition_met >= period_required){
        return true;
    } else {
        return false;
    }
}

uint8_t DenoiseComparison::GreaterThan(float value)
{
    if (value > comparison_value) {
        period_condition_met++;
    }
    else {
        period_condition_met = 0;
    }
    if (period_condition_met >= period_required) {
        return true;
    }
    else {
        return false;
    }
}

uint8_t DenoiseComparison::LessThan(float value)
{
    if (value < comparison_value) {
        period_condition_met++;
    }
    else {
        period_condition_met = 0;
    }
    if (period_condition_met >= period_required) {
        return true;
    }
    else {
        return false;
    }
}