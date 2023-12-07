#include "Physical_IO.h"

Joystick joystick(JOYSTICK_X, JOYSTICK_Y);
AnalogControl zoomControl(ZOOM_PIN);
AnalogControl rotateControl(ROTATE_PIN);

DenoiseComparison zoom_comparison(5, 0.0, 0.01);
DenoiseComparison rotate_comparison(5, 0.0, 0.01);
DenoiseComparison joy_x_comparison(5, 0.0, 0.01);
DenoiseComparison joy_y_comparison(5, 0.0, 0.01);

PCF857X pcf857X(PCF857X_ADDR);  // Note that the SCL and SDA pins have already been defined as part of
                                // the touch screen setup.

uint8_t current_joystick_mode = JoystickModeNone;
uint8_t previous_joystick_mode = JoystickModeNone;
long millis_last_joystick_move = 0;

int8_t joystick_mode_pins[4] = {BUTTON_1, BUTTON_2, BUTTON_3, BUTTON_4};
const uint8_t HWButton_Pins[NUM_HW_BUTTONS] = {BUTTON_0, BUTTON_1, BUTTON_2, BUTTON_3, BUTTON_4, BUTTON_5, BUTTON_6, BUTTON_7, BUTTON_8, BUTTON_9, BUTTON_10};
int8_t mode_select_button_pin = BUTTON_0;

uint8_t control_mode = JOYSTICK_CONTROL_MODE_MOUSE;
uint8_t last_control_mode = JOYSTICK_CONTROL_MODE_MOUSE;

PCF857X::DigitalInput pcf857X_inputs;

Button2 joystickModeButton;

byte joystickModeButtonHandler()
{
    return get_hwbutton(mode_select_button_pin);
}

void init_io()
{
    MSG_INFOLN("[INFO] Init joystick");
    joystick.Init();
    joystick.SetDeadzone(cadconfig.joy_deadzone);
    joystick.SetScale(0, cadconfig.joy_scale_x);
    joystick.SetScale(1, cadconfig.joy_scale_y);

    zoomControl.Init();
    zoomControl.SetDeadzone(cadconfig.zoom_deadzone);
    zoomControl.SetScale(cadconfig.zoom_scale);

    rotateControl.Init();
    rotateControl.SetDeadzone(cadconfig.rotate_deadzone);
    rotateControl.SetScale(cadconfig.rotate_scale);

    MSG_INFOLN("[INFO] Init pcf857X pins");
    // All pins are inputs. Note that the PCF857X library doesn't support pullups, so
    // mode has to be INPUT or OUTPUT, not INPUT_PULLUP.
    for (int i = 0; i < 16; i++) {
        pcf857X.pinMode(i, INPUT);
    }

    set_pantilt_mode(cadconfig.current_program);

    joystickModeButton.setButtonStateFunction(joystickModeButtonHandler);
    joystickModeButton.setClickHandler(joystickModeButtonClick);
    joystickModeButton.setDoubleClickHandler(joystickModeButtonDoubleClick);
    joystickModeButton.begin(VIRTUAL_PIN);

    pcf857X.begin(false);  // false so as not to start the I2C bus (already done by the touch controller)
}

void set_move_mode(uint8_t cadapp)
{
    switch (cadapp) {
        case CADApp_SolidWorks:
            KeyboardMouseAction(Action_MouseButton, MouseButton_RLRM, 0);  // Release all mouse buttons
            KeyboardMouseAction(Action_Option, OptionKey_ReleaseAll, 0);   // Release all keys
            KeyboardMouseAction(Action_Option, OptionKey_LCtrl, 0);        // Set left ctrl key
            KeyboardMouseAction(Action_MouseButton, MouseButton_PM, 0);    // Set middle mouse button
            break;

        case CADApp_Fusion360:
            KeyboardMouseAction(Action_MouseButton, MouseButton_RLRM, 0);  // Release all mouse buttons
            KeyboardMouseAction(Action_Option, OptionKey_ReleaseAll, 0);   // Release all keys
            KeyboardMouseAction(Action_MouseButton, MouseButton_PM, 0);    // Set middle mouse button
            break;

        case CADApp_Blender:
            KeyboardMouseAction(Action_MouseButton, MouseButton_RLRM, 0);  // Release all mouse buttons
            KeyboardMouseAction(Action_Option, OptionKey_ReleaseAll, 0);   // Release all keys
            KeyboardMouseAction(Action_Option, OptionKey_LShift, 0);       // Set left shift key
            KeyboardMouseAction(Action_MouseButton, MouseButton_PM, 0);    // Set middle mouse button
            break;

        case CADApp_FreeCAD:
            KeyboardMouseAction(Action_MouseButton, MouseButton_RLRM, 0);  // Release all mouse buttons
            KeyboardMouseAction(Action_Option, OptionKey_ReleaseAll, 0);   // Release all keys
            KeyboardMouseAction(Action_MouseButton, MouseButton_PM, 0);    // Set middle mouse button
            break;

        case CADApp_AC3D:
            KeyboardMouseAction(Action_MouseButton, MouseButton_RLRM, 0);  // Release all mouse buttons
            KeyboardMouseAction(Action_Option, OptionKey_ReleaseAll, 0);   // Release all keys
            KeyboardMouseAction(Action_MouseButton, MouseButton_PM, 0);    // Set middle mouse button
            break;

        default:
            KeyboardMouseAction(Action_MouseButton, MouseButton_RLRM, 0);  // Release all mouse buttons
            KeyboardMouseAction(Action_Option, OptionKey_ReleaseAll, 0);   // Release all keys
            break;
    }
    control_mode = JOYSTICK_CONTROL_MODE_MOVE;
}

void set_rotate_mode(uint8_t cadapp)
{
    switch (cadapp) {
        case CADApp_SolidWorks:
            KeyboardMouseAction(Action_MouseButton, MouseButton_RLRM, 0);  // Release all mouse buttons
            KeyboardMouseAction(Action_Option, OptionKey_ReleaseAll, 0);   // Release all keys
            KeyboardMouseAction(Action_MouseButton, MouseButton_PM, 0);    // Set middle mouse button
            break;

        case CADApp_Fusion360:
            KeyboardMouseAction(Action_MouseButton, MouseButton_RLRM, 0);  // Release all mouse buttons
            KeyboardMouseAction(Action_Option, OptionKey_ReleaseAll, 0);   // Release all keys
            KeyboardMouseAction(Action_Option, OptionKey_LShift, 0);       // Set left shift key
            KeyboardMouseAction(Action_MouseButton, MouseButton_PM, 0);    // Set middle mouse button
            break;

        case CADApp_Blender:
            KeyboardMouseAction(Action_MouseButton, MouseButton_RLRM, 0);  // Release all mouse buttons
            KeyboardMouseAction(Action_Option, OptionKey_ReleaseAll, 0);   // Release all keys
            KeyboardMouseAction(Action_MouseButton, MouseButton_PM, 0);    // Set middle mouse button
            break;

        case CADApp_FreeCAD:
            KeyboardMouseAction(Action_MouseButton, MouseButton_RLRM, 0);  // Release all mouse buttons
            KeyboardMouseAction(Action_Option, OptionKey_ReleaseAll, 0);   // Release all keys
            KeyboardMouseAction(Action_MouseButton, MouseButton_PM, 0);    // Set middle mouse button
            KeyboardMouseAction(Action_MouseButton, MouseButton_PR, 0);    // Set right mouse button (must set middle mouse button then right mouse button for FreeCAD)
            break;

        case CADApp_AC3D:
            KeyboardMouseAction(Action_MouseButton, MouseButton_RLRM, 0);  // Release all mouse buttons
            KeyboardMouseAction(Action_Option, OptionKey_ReleaseAll, 0);   // Release all keys
            KeyboardMouseAction(Action_Option, OptionKey_LAlt, 0);         // Set left alt key
            KeyboardMouseAction(Action_MouseButton, MouseButton_PL, 0);    // Set left mouse button
            break;

        default:
            KeyboardMouseAction(Action_MouseButton, MouseButton_RLRM, 0);  // Release all mouse buttons
            KeyboardMouseAction(Action_Option, OptionKey_ReleaseAll, 0);   // Release all keys
            break;
    }

    control_mode = JOYSTICK_CONTROL_MODE_ROTATE;
}

void set_pantilt_mode(uint8_t cadapp)
{
    switch (cadapp) {
        case CADApp_SolidWorks:
            KeyboardMouseAction(Action_MouseButton, MouseButton_RLRM, 0);  // Release all mouse buttons
            KeyboardMouseAction(Action_Option, OptionKey_ReleaseAll, 0);   // Release all keys
            KeyboardMouseAction(Action_MouseButton, MouseButton_PM, 0);    // Set middle mouse button
            break;

        case CADApp_Fusion360:
            KeyboardMouseAction(Action_MouseButton, MouseButton_RLRM, 0);  // Release all mouse buttons
            KeyboardMouseAction(Action_Option, OptionKey_ReleaseAll, 0);   // Release all keys
            KeyboardMouseAction(Action_Option, OptionKey_LShift, 0);       // Set left shift key
            KeyboardMouseAction(Action_MouseButton, MouseButton_PM, 0);    // Set middle mouse button
            break;

        case CADApp_Blender:
            KeyboardMouseAction(Action_MouseButton, MouseButton_RLRM, 0);  // Release all mouse buttons
            KeyboardMouseAction(Action_Option, OptionKey_ReleaseAll, 0);   // Release all keys
            KeyboardMouseAction(Action_MouseButton, MouseButton_PM, 0);    // Set middle mouse button
            break;

        case CADApp_FreeCAD:
            KeyboardMouseAction(Action_MouseButton, MouseButton_RLRM, 0);  // Release all mouse buttons
            KeyboardMouseAction(Action_Option, OptionKey_ReleaseAll, 0);   // Release all keys
            KeyboardMouseAction(Action_MouseButton, MouseButton_PM, 0);    // Set middle mouse button
            KeyboardMouseAction(Action_MouseButton, MouseButton_PR, 0);    // Set right mouse button (must set middle mouse button then right mouse button for FreeCAD)
            break;

        case CADApp_AC3D:
            KeyboardMouseAction(Action_MouseButton, MouseButton_RLRM, 0);  // Release all mouse buttons
            KeyboardMouseAction(Action_Option, OptionKey_ReleaseAll, 0);   // Release all keys
            KeyboardMouseAction(Action_Option, OptionKey_LAlt, 0);         // Set left alt key
            KeyboardMouseAction(Action_MouseButton, MouseButton_PL, 0);    // Set left mouse button
            break;

        default:
            KeyboardMouseAction(Action_MouseButton, MouseButton_RLRM, 0);  // Release all mouse buttons
            KeyboardMouseAction(Action_Option, OptionKey_ReleaseAll, 0);   // Release all keys
            break;
    }

    control_mode = JOYSTICK_CONTROL_MODE_PANTILT;
}

void set_mouse_mode()
{
    KeyboardMouseAction(Action_MouseButton, MouseButton_RLRM, 0);  // Release all mouse buttons
    KeyboardMouseAction(Action_Option, OptionKey_ReleaseAll, 0);   // Release all keys
    control_mode = JOYSTICK_CONTROL_MODE_MOUSE;
}

void joystickModeButtonClick(Button2& btn)
{
    if (control_mode != JOYSTICK_CONTROL_MODE_MOUSE) {
        set_mouse_mode();
    }
    KeyboardMouseAction(Action_MouseButton, MouseButton_PL, 0);  // Set left mouse button
    KeyboardMouseAction(Action_MouseButton, MouseButton_RL, 0);  // Release left mouse button
}

void joystickModeButtonDoubleClick(Button2& btn)
{
    if (control_mode != JOYSTICK_CONTROL_MODE_MOUSE) {
        set_mouse_mode();
    }
    KeyboardMouseAction(Action_MouseButton, MouseButton_PL, 0);  // Set left mouse button
    KeyboardMouseAction(Action_MouseButton, MouseButton_RL, 0);  // Release left mouse button
    KeyboardMouseAction(Action_MouseButton, MouseButton_PL, 0);  // Set left mouse button
    KeyboardMouseAction(Action_MouseButton, MouseButton_RL, 0);  // Release left mouse button
}

void update_io()
{
    joystick.Update();
    zoomControl.Update();
    rotateControl.Update();
    pcf857X_inputs = pcf857X.digitalReadAll();
    joystickModeButton.loop();

    uint8_t hw_button0_set = !get_hwbutton(mode_select_button_pin);

    uint8_t joystick_x_steadyzero = joy_x_comparison.Equals(joystick.x());
    uint8_t joystick_y_steadyzero = joy_y_comparison.Equals(joystick.y());
    uint8_t rotate_knob_steadyzero = rotate_comparison.Equals(rotateControl.Value());
    uint8_t zoom_knob_steadyzero = zoom_comparison.Equals(zoomControl.Value());
    uint8_t all_control_steadyzero = joystick_x_steadyzero && joystick_y_steadyzero && rotate_knob_steadyzero && zoom_knob_steadyzero;

    if (!cadconfig.spacemouse_enable) {
        if (Keyboard.isConnected()) {
#if (LOG_MSG_JOYSTICK_MODE > 0)
            if (!get_pcf857X_bit(pcf857X_inputs, BUTTON_10)) {
                char msg[30];
                // sprintf(msg, "[DBG] %d,%d,%d,%d,%d", all_control_steadyzero, joystick_x_steadyzero,
                //         joystick_y_steadyzero, rotate_knob_steadyzero, zoom_knob_steadyzero);
                sprintf(msg, "[DBG] %d", joystickModeButton.isPressed());
                MSG_PORT.println(msg);
            }

#endif
            if (!all_control_steadyzero) {
                switch (cadconfig.current_program) {
                    case CADApp_SolidWorks:
                    case CADApp_Fusion360:
                    case CADApp_FreeCAD:
                    case CADApp_AC3D:
                        if (hw_button0_set) {
                            if (!joystick_x_steadyzero || !joystick_y_steadyzero || !zoom_knob_steadyzero) {
                                if (control_mode != JOYSTICK_CONTROL_MODE_MOVE) {
                                    set_move_mode(cadconfig.current_program);
                                }
                                Mouse.move(joystick.x() * cadconfig.joy_sensitivity, joystick.y() * cadconfig.joy_sensitivity, zoomControl.Value() * cadconfig.zoom_sensitivity);
                            }
                        }
                        else {
                            if (!zoom_knob_steadyzero || !rotate_knob_steadyzero) {
                                if (control_mode != JOYSTICK_CONTROL_MODE_ROTATE) {
                                    set_rotate_mode(cadconfig.current_program);
                                }
                                Mouse.move(rotateControl.Value() * cadconfig.rotate_sensitivity, zoomControl.Value() * cadconfig.rotate_sensitivity, 0);
                            }
                            else if (!joystick_x_steadyzero || !joystick_y_steadyzero) {
                                if (control_mode != JOYSTICK_CONTROL_MODE_MOUSE) {
                                    set_mouse_mode();
                                }
                                Mouse.move(joystick.x() * cadconfig.mouse_sensitivity, joystick.y() * cadconfig.mouse_sensitivity, 0.0);
                            }
                        }
                        break;

                    case CADApp_Blender:

                        if (hw_button0_set) {
                            if (control_mode != JOYSTICK_CONTROL_MODE_MOVE) {
                                set_move_mode(cadconfig.current_program);
                            }

                            if (!joystick_x_steadyzero || !joystick_y_steadyzero || !zoom_knob_steadyzero) {
                                Mouse.move(joystick.x() * cadconfig.joy_sensitivity, joystick.y() * cadconfig.joy_sensitivity, 0.0);
                            }
                        }
                        else {
                            if (!rotate_knob_steadyzero) {
                                if (control_mode != JOYSTICK_CONTROL_MODE_ROTATE) {
                                    set_rotate_mode(cadconfig.current_program);
                                }
                                Mouse.move(rotateControl.Value() * cadconfig.rotate_sensitivity, 0.0, 0.0);
                            }
                            else if (!joystick_x_steadyzero || !joystick_y_steadyzero) {
                                if (control_mode != JOYSTICK_CONTROL_MODE_PANTILT) {
                                    set_pantilt_mode(cadconfig.current_program);
                                }
                                Mouse.move(joystick.x() * cadconfig.joy_sensitivity, joystick.y() * cadconfig.joy_sensitivity, 0.0);
                            }
                        }

                        break;

                    default:
                        break;
                }
            }
            else {
                if (all_control_steadyzero && (control_mode != JOYSTICK_CONTROL_MODE_MOUSE)) {
                    set_mouse_mode();
                }
            }

            last_control_mode = control_mode;
        }

        else {
            MSG_WARNLN("[WARN] BLE Mouse not connected");
        }
    }

    else {
        uint16_t x, y, z, a, b, c;
        if (hw_button0_set) {
            x = joystick.x() * 2047.0 + 32768;
            y = joystick.y() * 2047.0 + 32768;
            z = zoomControl.Value() * 2047.0 + 32768;
            a = 32768;
            b = 32768;
            c = rotateControl.Value() * 2047.0 + 32768;
        }
        else {
            x = 32768;
            y = 32768;
            z = zoomControl.Value() * 2047.0 + 32768;
            a = joystick.x() * 2047.0 + 32768;
            b = joystick.y() * 2047.0 + 32768;
            c = rotateControl.Value() * 2047.0 + 32768;
        }
        spaceMouse.SendViewpointDataPacket(x, y, z, a, b, c);
    }
}

void calibrate_scale_analog_controls()
{
    // calibrate the analog controls
    int max_value_x = -10000;
    int min_value_x = 10000;
    int max_value_y = -10000;
    int min_value_y = 10000;
    int max_value_r = -10000;
    int min_value_r = 10000;
    int max_value_s = -10000;
    int min_value_s = 10000;

    long timestart = millis();

    while (millis() - timestart < CALIBRATION_TIME_MS) {
        joystick.Update();
        int value_int_x = joystick.RawX();
        int value_int_y = joystick.RawY();

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

        zoomControl.Update();
        int value_int_r = rotateControl.RawValue();
        if (value_int_r > max_value_r) {
            max_value_r = value_int_r;
        }
        else if (value_int_r < min_value_r) {
            min_value_r = value_int_r;
        }

        rotateControl.Update();
        int value_int_s = zoomControl.RawValue();
        if (value_int_s > max_value_s) {
            max_value_s = value_int_s;
        }
        else if (value_int_s < min_value_s) {
            min_value_s = value_int_s;
        }
    }
    float scale_x = 2.0 / (float)(max_value_x - min_value_x) * 4095.0;
    float scale_y = 2.0 / (float)(max_value_y - min_value_y) * 4095.0;
    float scale_r = 2.0 / (float)(max_value_r - min_value_r) * 4095.0;
    float scale_s = 2.0 / (float)(max_value_s - min_value_s) * 4095.0;

    joystick.SetScale(0, scale_x);
    joystick.SetScale(1, scale_y);
    rotateControl.SetScale(scale_r);
    zoomControl.SetScale(scale_s);
}

void calibrate_zero_analog_controls()
{
    joystick.CalibrateZeroAll();
    zoomControl.CalibrateZero();
    rotateControl.CalibrateZero();
}

uint8_t get_hwbutton(uint8_t button)
{
    uint8_t state;
    if (button >= 0 && button < NUM_HW_BUTTONS) {
        state = !get_pcf857X_bit(pcf857X_inputs, HWButton_Pins[button]);
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