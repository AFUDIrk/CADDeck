#pragma once
#include <RotaryEncoder.h>
#include "CADDeck.h"
#include "joystick.h"
#include "PCF857X.h"

const uint8_t PCF857X_SCL = 19;
const uint8_t PCF857X_SDA = 18;
const uint8_t PCF857X_INT = 27;
const uint8_t PCF857X_ADDR = 0x20;

extern PCF857X pcf857X;

extern long encoder1Value;
extern bool encoder1Changed;

extern Joystick joystick;

#define JOYSTICK_ENABLE
#ifdef JOYSTICK_ENABLE
const uint8_t JOYSTICK_X = 32;
const uint8_t JOYSTICK_Y = 33;
#endif
extern int8_t joystick_mode_pins[4];


// The encoders are on direct digital inputs
const uint8_t ENC_1_A = 25;
const uint8_t ENC_1_B = 26;

// const uint8_t ENC_2_A = P3;
// const uint8_t ENC_2_B = P4;

// The buttons are on the pcf857X encoder. pins "Pn" are the PCF857X pin numbers
const uint8_t BUTTON_JOY_1 = P0;
const uint8_t BUTTON_JOY_2 = P1;
const uint8_t BUTTON_JOY_3 = P2;
const uint8_t BUTTON_JOY_4 = P3;

const uint8_t BUTTON_1 = P8;
const uint8_t BUTTON_2 = P9;
const uint8_t BUTTON_3 = P10;
const uint8_t BUTTON_4 = P11;

const uint8_t ENC_1_SW = P14;

extern PCF857X::DigitalInput pcf857X_inputs;
extern const uint8_t HWButton_Pins[];

void init_io();
void update_io();
uint8_t pan_button();
uint8_t rotate_button();
uint8_t zoom_button();
uint8_t set_mouse_buttons();
uint8_t check_joystick_mode();
uint8_t get_hwbutton(uint8_t button);
uint8_t get_pcf857X_bit(PCF857X::DigitalInput dinputs, uint8_t pin);

extern uint8_t current_joystick_mode;
extern uint8_t previous_joystick_mode;