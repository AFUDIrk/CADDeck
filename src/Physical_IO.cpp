#include "Physical_IO.h"

Joystick joystick(JOYSTICK_X, JOYSTICK_Y);

PCF857X pcf857X(PCF857X_ADDR);  // Note that the SCL and SDA pins have already been defined as part of
                                   // the touch screen setup.


long encoder1Value = 0;
long lastEncoder1Value = 0;
bool encoder1Changed = false;

uint8_t current_joystick_mode = JoystickMode_None;
uint8_t previous_joystick_mode = JoystickMode_None;
long millis_last_joystick_move = 0;

int8_t joystick_mode_pins[4] = {0, 0, 0, 0};
const uint8_t HWButton_Pins[NUM_HW_BUTTONS] = {BUTTON_1L, BUTTON_2L, BUTTON_3L, BUTTON_4L, BUTTON_5L, BUTTON_1R, BUTTON_2R, BUTTON_3R, BUTTON_4R, BUTTON_5R, ENC_1_SW};

PCF857X::DigitalInput pcf857X_inputs;

// Encoder encoder1(ENC_1_A, ENC_1_B);
//  Setup a RotaryEncoder with 2 steps per latch for the 2 signal input pins:
RotaryEncoder encoder1(ENC_1_A, ENC_1_B, RotaryEncoder::LatchMode::TWO03);

void encoder1_ISR();

void init_io()
{
    MSG_INFOLN("[INFO] Init joystick");
    joystick.Init();

    joystick.SetDeadzone(cadconfig.joy_deadzone);
    joystick.SetScale(0, cadconfig.joy_scale_x);
    joystick.SetScale(1, cadconfig.joy_scale_y);


    MSG_INFOLN("[INFO] Init pcf857X pins");
    // All pins are inputs. Note that the PCF857X library doesn't support pullups, so
    // mode has to be INPUT or OUTPUT, not INPUT_PULLUP.
    for (int i = 0; i < 16; i++) {
        pcf857X.pinMode(i, INPUT);
    }

    pcf857X.begin(false);  // false so as not to start the I2C bus (already done by the touch controller)

    attachInterrupt(ENC_1_A, encoder1_ISR, CHANGE);
    attachInterrupt(ENC_1_B, encoder1_ISR, CHANGE);
}    

void encoder1_ISR()
{
    noInterrupts();
    encoder1.tick();
    interrupts();
}

void update_io()
{
    joystick.Update();
    pcf857X_inputs = pcf857X.digitalReadAll();

    encoder1Value = (encoder1.getPosition())/2; // 2 steps per latch
    if(encoder1Value != lastEncoder1Value){
        encoder1Changed = true;
    }
    else{
        encoder1Changed = false;
    }

    if (Keyboard.isConnected()) {
        if (set_mouse_buttons() != JoystickMode_None || encoder1Changed) {
            Mouse.move(joystick.x() * cadconfig.joy_sensitivity, joystick.y() * cadconfig.joy_sensitivity, (encoder1Value - lastEncoder1Value) * cadconfig.thumbwheel_sensitivity);
        }
    }
    else {
        MSG_WARNLN("[DEBUG] BLE Mouse not connected");
    }
    lastEncoder1Value = encoder1Value;
}

// Handle the joystick driving the mouse
uint8_t set_mouse_buttons()
{
    current_joystick_mode = check_joystick_mode();

    if (current_joystick_mode != previous_joystick_mode) {

        MSG_DEBUG1("[DEBUG] Joystick mode changed to ", current_joystick_mode);
        switch (current_joystick_mode) {
            case JoystickMode_None:
                KeyboardMouseAction(17, 11, 0);  // Release all mouse buttons
                KeyboardMouseAction(5, 9, 0);
                break;

            case JoystickMode_Pan:
                for (int i = 0; i < 3; i++) {
                    if (cadprogramconfig[cadconfig.current_program].pan[i].action != 0) {
                        KeyboardMouseAction(cadprogramconfig[cadconfig.current_program].pan[i].action,
                                            cadprogramconfig[cadconfig.current_program].pan[i].value,
                                            cadprogramconfig[cadconfig.current_program].pan[i].symbol);
                    }
                }
                break;

            case JoystickMode_Rotate:
                for (int i = 0; i < 3; i++) {
                    if (cadprogramconfig[cadconfig.current_program].rotate[i].action != 0) {
                        KeyboardMouseAction(cadprogramconfig[cadconfig.current_program].rotate[i].action,
                                            cadprogramconfig[cadconfig.current_program].rotate[i].value,
                                            cadprogramconfig[cadconfig.current_program].rotate[i].symbol);
                    }
                }
                break;

            case JoystickMode_Zoom:
                for (int i = 0; i < 3; i++) {
                    if (cadprogramconfig[cadconfig.current_program].zoom[i].action != 0) {
                        KeyboardMouseAction(cadprogramconfig[cadconfig.current_program].zoom[i].action,
                                            cadprogramconfig[cadconfig.current_program].zoom[i].value,
                                            cadprogramconfig[cadconfig.current_program].zoom[i].symbol);
                    }
                }
                break;

            case JoystickMode_Mouse:
                KeyboardMouseAction(17, 11, 0);  // Release all mouse buttons
                KeyboardMouseAction(5, 9, 0);
                break;
                
            default:
                break;
        }
    }
    previous_joystick_mode = current_joystick_mode;
    return current_joystick_mode;
}

uint8_t check_joystick_mode()
{
    uint8_t mode = 0;

    if (fabs(joystick.x()) < 0.001 && fabs(joystick.y()) < 0.001) {
        if (millis() - millis_last_joystick_move > cadconfig.joy_steady_time) {
            mode = JoystickMode_None;
        }
        else {
            mode = previous_joystick_mode;
        }
    }
    else if (pan_button()) {
        mode = JoystickMode_Pan;
        millis_last_joystick_move = millis();
    }
    else if (rotate_button()) {
        mode = JoystickMode_Rotate;
        millis_last_joystick_move = millis();
    }
    else if (zoom_button()) {
        mode = JoystickMode_Zoom;
        millis_last_joystick_move = millis();
    }
    else {
        mode = cadprogramconfig[cadconfig.current_program].default_joystick_mode;
        millis_last_joystick_move = millis();
    }

    return mode;
}

uint8_t pan_button()
{
    uint8_t state;
    uint8_t pin = joystick_mode_pins[JoystickMode_Pan];

    state = !get_pcf857X_bit(pcf857X_inputs, pin);

    return state;
}

uint8_t rotate_button()
{
    uint8_t state;
    uint8_t pin = joystick_mode_pins[JoystickMode_Rotate];

    state = !get_pcf857X_bit(pcf857X_inputs, pin);

    return state;
}

uint8_t zoom_button()
{
    uint8_t state;
    uint8_t pin = joystick_mode_pins[JoystickMode_Zoom];

    state = !get_pcf857X_bit(pcf857X_inputs, pin);

    return state;
}

uint8_t get_hwbutton(uint8_t button)
{
    uint8_t state;
    if (button >= 0 && button < NUM_HW_BUTTONS) {
        state = !get_pcf857X_bit(pcf857X_inputs,HWButton_Pins[button]);
    }
    else {
        state = 0;
    }
    return state;
}

uint8_t get_pcf857X_bit(PCF857X::DigitalInput dinputs, uint8_t pin)
{
    uint8_t state;
    switch (pin) {
        case P0:
            state = dinputs.p0;
            break;
        case P1:
            state = dinputs.p1;
            break;
        case P2:
            state = dinputs.p2;
            break;
        case P3:
            state = dinputs.p3;
            break;
        case P4:
            state = dinputs.p4;
            break;
        case P5:
            state = dinputs.p5;
            break;
        case P6:
            state = dinputs.p6;
            break;
        case P7:
            state = dinputs.p7;
            break;
        case P8:
            state = dinputs.p8;
            break;
        case P9:
            state = dinputs.p9;
            break;
        case P10:
            state = dinputs.p10;
            break;
        case P11:
            state = dinputs.p11;
            break;
        case P12:
            state = dinputs.p12;
            break;
        case P13:
            state = dinputs.p13;
            break;
        case P14:
            state = dinputs.p14;
            break;
        case P15:
            state = dinputs.p15;
            break;
        default:
            state = 0;
            break;
    }
    return state;
}