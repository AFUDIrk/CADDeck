#pragma once
#include "CADDeck.h"
#include "joystick.h"
#include "AnalogControl.h"
#include "PCF857X.h"
#include "DenoiseComparison.h"


const uint8_t PCF857X_SCL = 19;
const uint8_t PCF857X_SDA = 18;
const uint8_t PCF857X_INT = 27;
const uint8_t PCF857X_ADDR = 0x20;

extern PCF857X pcf857X;

extern Joystick joystick;
extern AnalogControl zoomControl;
extern AnalogControl rotateControl;

const uint8_t JOYSTICK_X = 32;
const uint8_t JOYSTICK_Y = 33;
extern int8_t joystick_mode_pins[4];
extern int8_t mode_select_button_pin;

const uint8_t ZOOM_PIN = 35;
const uint8_t ROTATE_PIN = 34;

const uint16_t CALIBRATION_TIME_MS = 10000;

// The buttons are on the pcf857X encoder. pins "Pn" are the PCF857X pin numbers
#ifdef NEW_PINS
const uint8_t BUTTON_0 = P0;
const uint8_t BUTTON_1 = P3;
const uint8_t BUTTON_2 = P1;
const uint8_t BUTTON_3 = P4;
const uint8_t BUTTON_4 = P2;
const uint8_t BUTTON_5 = P5;
const uint8_t BUTTON_6 = P10;
const uint8_t BUTTON_7 = P8;
const uint8_t BUTTON_8 = P9;
const uint8_t BUTTON_9 = P12;
const uint8_t BUTTON_10 = P11;
#else
const uint8_t BUTTON_0 = P14;
const uint8_t BUTTON_1 = P8;
const uint8_t BUTTON_2 = P9;
const uint8_t BUTTON_3 = P0;
const uint8_t BUTTON_4 = P1;
const uint8_t BUTTON_5 = P12;
const uint8_t BUTTON_6 = P13;
const uint8_t BUTTON_7 = P7;
const uint8_t BUTTON_8 = P15;
const uint8_t BUTTON_9 = P10;
const uint8_t BUTTON_10 = P11;
#endif

extern PCF857X::DigitalInput pcf857X_inputs;
extern const uint8_t HWButton_Pins[];

void init_io();
void update_io();
uint8_t pan_button();
uint8_t tilt_button();
uint8_t zoom_button();
uint8_t rotate_button();
uint8_t set_mouse_buttons();
uint8_t check_joystick_mode();
uint8_t get_hwbutton(uint8_t button);
uint8_t get_pcf857X_bit(PCF857X::DigitalInput dinputs, uint8_t pin);
void calibrate_scale_analog_controls();
void calibrate_zero_analog_controls();
void set_rotate_mode(uint8_t cadapp);
void set_move_mode(uint8_t cadapp);
void set_pantilt_mode(uint8_t cadapp);

extern uint8_t current_joystick_mode;
extern uint8_t previous_joystick_mode;

enum JoystickControlMode {
    JOYSTICK_CONTROL_MODE_MOUSE = 0,
    JOYSTICK_CONTROL_MODE_PANTILT,
    JOYSTICK_CONTROL_MODE_ROTATE,
    JOYSTICK_CONTROL_MODE_MOVE
};