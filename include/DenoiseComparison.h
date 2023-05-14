/*
  DenoiseComparison.h

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

#pragma once

#include "Arduino.h"
#include <stdint.h>

class DenoiseComparison {
   public:
    DenoiseComparison(uint16_t period, float value, float tolerance=0.0);
    ~DenoiseComparison();
    void Reset();

    uint8_t Equals(float value);
    uint8_t GreaterThan(float value);
    uint8_t LessThan(float value);

   private:
    // DenoiseComparison variables
    float tolerance_value;
    float comparison_value;
    uint16_t period_condition_met;   // iteration counts
    uint16_t period_required;
};
