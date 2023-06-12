#include "Action.h"
#include "SaveConfig.h"
#include "BleCombo.h"

/**
* @brief This function takes an int as an "action" and "value". It uses
         a switch statement to determine which type of action to do.
         e.g. write, print, press. If an action requires a char, you
         can pass the pointer to that char through the parameter "symbol"
*
* @param action int
* @param value int
* @param symbol char *
*
* @return none
*
* @note Case 11 is used for special functions, none bleKeyboard related.
*/
void KeyboardMouseAction(int action, int value, char *symbol, uint8_t hwbutton_index)
{
//    MSG_INFO("[DEBUG] Action received: ");
//    MSG_INFO3(action, value, symbol, hwbutton_index);
    int callingPageNum;
    
    switch (action) {
        case Action_NoAction:
            // No Action
            break;
        case Action_Delay:  // Delay
            delay(value);
            break;
        case Action_TabArrow:  // Send TAB ARROW etc
            switch (value) {
                case 1:
                    Keyboard.write(KEY_UP_ARROW);
                    break;
                case 2:
                    Keyboard.write(KEY_DOWN_ARROW);
                    break;
                case 3:
                    Keyboard.write(KEY_LEFT_ARROW);
                    break;
                case 4:
                    Keyboard.write(KEY_RIGHT_ARROW);
                    break;
                case 5:
                    Keyboard.write(KEY_BACKSPACE);
                    break;
                case 6:
                    Keyboard.write(KEY_TAB);
                    break;
                case 7:
                    Keyboard.write(KEY_RETURN);
                    break;
                case 8:
                    Keyboard.write(KEY_PAGE_UP);
                    break;
                case 9:
                    Keyboard.write(KEY_PAGE_DOWN);
                    break;
                case 10:
                    Keyboard.write(KEY_DELETE);
                    break;
                case 11:
                    Keyboard.write(KEY_PRTSC);
                    break;
                case 12:
                    Keyboard.write(KEY_ESC);
                    break;
                case 13:
                    Keyboard.write(KEY_HOME);
                    break;
                case 14:
                    Keyboard.write(KEY_END);
                    break;

                default:
                    // if nothing matches do nothing
                    break;
            }
            break;
        case Action_Media:  // Send Media Key
            switch (value) {
                case 1:
                    Keyboard.write(KEY_MEDIA_MUTE);
                    break;
                case 2:
                    Keyboard.write(KEY_MEDIA_VOLUME_DOWN);
                    break;
                case 3:
                    Keyboard.write(KEY_MEDIA_VOLUME_UP);
                    break;
                case 4:
                    Keyboard.write(KEY_MEDIA_PLAY_PAUSE);
                    break;
                case 5:
                    Keyboard.write(KEY_MEDIA_STOP);
                    break;
                case 6:
                    Keyboard.write(KEY_MEDIA_NEXT_TRACK);
                    break;
                case 7:
                    Keyboard.write(KEY_MEDIA_PREVIOUS_TRACK);
                    break;
                case 8:
                    Keyboard.write(KEY_MEDIA_WWW_HOME);
                    break;
                case 9:
                    Keyboard.write(KEY_MEDIA_LOCAL_MACHINE_BROWSER);
                    break;
                case 10:
                    Keyboard.write(KEY_MEDIA_CALCULATOR);
                    break;
                case 11:
                    Keyboard.write(KEY_MEDIA_WWW_BOOKMARKS);
                    break;
                case 12:
                    Keyboard.write(KEY_MEDIA_WWW_SEARCH);
                    break;
                case 13:
                    Keyboard.write(KEY_MEDIA_WWW_STOP);
                    break;
                case 14:
                    Keyboard.write(KEY_MEDIA_WWW_BACK);
                    break;
                case 15:
                    Keyboard.write(KEY_MEDIA_CONSUMER_CONTROL_CONFIGURATION);
                    break;
                case 16:
                    Keyboard.write(KEY_MEDIA_EMAIL_READER);
                    break;
                default:
                    // if nothing matches do nothing
                    break;
            }
            break;
        case Action_Char:  // Send Character
            Keyboard.print(symbol);
            break;
        case Action_Option:  // Option Keys
            switch (value) {
                case 1:
                    Keyboard.press(KEY_LEFT_CTRL);
                    break;
                case 2:
                    Keyboard.press(KEY_LEFT_SHIFT);
                    break;
                case 3:
                    Keyboard.press(KEY_LEFT_ALT);
                    break;
                case 4:
                    Keyboard.press(KEY_LEFT_GUI);
                    break;
                case 5:
                    Keyboard.press(KEY_RIGHT_CTRL);
                    break;
                case 6:
                    Keyboard.press(KEY_RIGHT_SHIFT);
                    break;
                case 7:
                    Keyboard.press(KEY_RIGHT_ALT);
                    break;
                case 8:
                    Keyboard.press(KEY_RIGHT_GUI);
                    break;
                case 9:
                    Keyboard.releaseAll();
                    break;
                default:
                    // if nothing matches do nothing
                    break;
            }
            break;
        case Action_FnKey:  // Function Keys
            switch (value) {
                case 1:
                    Keyboard.press(KEY_F1);
                    break;
                case 2:
                    Keyboard.press(KEY_F2);
                    break;
                case 3:
                    Keyboard.press(KEY_F3);
                    break;
                case 4:
                    Keyboard.press(KEY_F4);
                    break;
                case 5:
                    Keyboard.press(KEY_F5);
                    break;
                case 6:
                    Keyboard.press(KEY_F6);
                    break;
                case 7:
                    Keyboard.press(KEY_F7);
                    break;
                case 8:
                    Keyboard.press(KEY_F8);
                    break;
                case 9:
                    Keyboard.press(KEY_F9);
                    break;
                case 10:
                    Keyboard.press(KEY_F10);
                    break;
                case 11:
                    Keyboard.press(KEY_F11);
                    break;
                case 12:
                    Keyboard.press(KEY_F12);
                    break;
                case 13:
                    Keyboard.press(KEY_F13);
                    break;
                case 14:
                    Keyboard.press(KEY_F14);
                    break;
                case 15:
                    Keyboard.press(KEY_F15);
                    break;
                case 16:
                    Keyboard.press(KEY_F16);
                    break;
                case 17:
                    Keyboard.press(KEY_F17);
                    break;
                case 18:
                    Keyboard.press(KEY_F18);
                    break;
                case 19:
                    Keyboard.press(KEY_F19);
                    break;
                case 20:
                    Keyboard.press(KEY_F20);
                    break;
                case 21:
                    Keyboard.press(KEY_F21);
                    break;
                case 22:
                    Keyboard.press(KEY_F22);
                    break;
                case 23:
                    Keyboard.press(KEY_F23);
                    break;
                case 24:
                    Keyboard.press(KEY_F24);
                    break;
                default:
                    // if nothing matches do nothing
                    break;
            }
            break;
        case Action_Number:  // Send Number
            Keyboard.print(value);
            break;
        case Action_SpecialChar:  // Send Special Character
            Keyboard.print(symbol);
            break;
        case Action_Combos:  // Combos
            switch (value) {
                case 1:
                    Keyboard.press(KEY_LEFT_CTRL);
                    Keyboard.press(KEY_LEFT_SHIFT);
                    break;
                case 2:
                    Keyboard.press(KEY_LEFT_ALT);
                    Keyboard.press(KEY_LEFT_SHIFT);
                    break;
                case 3:
                    Keyboard.press(KEY_LEFT_GUI);
                    Keyboard.press(KEY_LEFT_SHIFT);
                    break;
                case 4:
                    Keyboard.press(KEY_LEFT_CTRL);
                    Keyboard.press(KEY_LEFT_GUI);
                    break;
                case 5:
                    Keyboard.press(KEY_LEFT_ALT);
                    Keyboard.press(KEY_LEFT_GUI);
                    break;
                case 6:
                    Keyboard.press(KEY_LEFT_CTRL);
                    Keyboard.press(KEY_LEFT_ALT);
                    break;
                case 7:
                    Keyboard.press(KEY_LEFT_CTRL);
                    Keyboard.press(KEY_LEFT_ALT);
                    Keyboard.press(KEY_LEFT_GUI);
                    break;
                case 8:
                    Keyboard.press(KEY_RIGHT_CTRL);
                    Keyboard.press(KEY_RIGHT_SHIFT);
                    break;
                case 9:
                    Keyboard.press(KEY_RIGHT_ALT);
                    Keyboard.press(KEY_RIGHT_SHIFT);
                    break;
                case 10:
                    Keyboard.press(KEY_RIGHT_GUI);
                    Keyboard.press(KEY_RIGHT_SHIFT);
                    break;
                case 11:
                    Keyboard.press(KEY_RIGHT_CTRL);
                    Keyboard.press(KEY_RIGHT_GUI);
                    break;
                case 12:
                    Keyboard.press(KEY_RIGHT_ALT);
                    Keyboard.press(KEY_RIGHT_GUI);
                    break;
                case 13:
                    Keyboard.press(KEY_RIGHT_CTRL);
                    Keyboard.press(KEY_RIGHT_ALT);
                    break;
                case 14:
                    Keyboard.press(KEY_RIGHT_CTRL);
                    Keyboard.press(KEY_RIGHT_ALT);
                    Keyboard.press(KEY_RIGHT_GUI);
                    break;
            }
            break;
        case Action_Helpers:  // Helpers
            switch (value) {
                case 1:
                    if (generalconfig.modifier1 != 0) {
                        Keyboard.press(generalconfig.modifier1);
                    }
                    if (generalconfig.modifier2 != 0) {
                        Keyboard.press(generalconfig.modifier2);
                    }
                    if (generalconfig.modifier3 != 0) {
                        Keyboard.press(generalconfig.modifier3);
                    }
                    Keyboard.press(KEY_F1);
                    Keyboard.releaseAll();
                    delay(generalconfig.helperdelay);
                    break;
                case 2:
                    if (generalconfig.modifier1 != 0) {
                        Keyboard.press(generalconfig.modifier1);
                    }
                    if (generalconfig.modifier2 != 0) {
                        Keyboard.press(generalconfig.modifier2);
                    }
                    if (generalconfig.modifier3 != 0) {
                        Keyboard.press(generalconfig.modifier3);
                    }
                    Keyboard.press(KEY_F2);
                    Keyboard.releaseAll();
                    delay(generalconfig.helperdelay);
                    break;
                case 3:
                    if (generalconfig.modifier1 != 0) {
                        Keyboard.press(generalconfig.modifier1);
                    }
                    if (generalconfig.modifier2 != 0) {
                        Keyboard.press(generalconfig.modifier2);
                    }
                    if (generalconfig.modifier3 != 0) {
                        Keyboard.press(generalconfig.modifier3);
                    }
                    Keyboard.press(KEY_F3);
                    Keyboard.releaseAll();
                    delay(generalconfig.helperdelay);
                    break;
                case 4:
                    if (generalconfig.modifier1 != 0) {
                        Keyboard.press(generalconfig.modifier1);
                    }
                    if (generalconfig.modifier2 != 0) {
                        Keyboard.press(generalconfig.modifier2);
                    }
                    if (generalconfig.modifier3 != 0) {
                        Keyboard.press(generalconfig.modifier3);
                    }
                    Keyboard.press(KEY_F4);
                    Keyboard.releaseAll();
                    delay(generalconfig.helperdelay);
                    break;
                case 5:
                    if (generalconfig.modifier1 != 0) {
                        Keyboard.press(generalconfig.modifier1);
                    }
                    if (generalconfig.modifier2 != 0) {
                        Keyboard.press(generalconfig.modifier2);
                    }
                    if (generalconfig.modifier3 != 0) {
                        Keyboard.press(generalconfig.modifier3);
                    }
                    Keyboard.press(KEY_F5);
                    Keyboard.releaseAll();
                    delay(generalconfig.helperdelay);
                    break;
                case 6:
                    if (generalconfig.modifier1 != 0) {
                        Keyboard.press(generalconfig.modifier1);
                    }
                    if (generalconfig.modifier2 != 0) {
                        Keyboard.press(generalconfig.modifier2);
                    }
                    if (generalconfig.modifier3 != 0) {
                        Keyboard.press(generalconfig.modifier3);
                    }
                    Keyboard.press(KEY_F6);
                    Keyboard.releaseAll();
                    delay(generalconfig.helperdelay);
                    break;
                case 7:
                    if (generalconfig.modifier1 != 0) {
                        Keyboard.press(generalconfig.modifier1);
                    }
                    if (generalconfig.modifier2 != 0) {
                        Keyboard.press(generalconfig.modifier2);
                    }
                    if (generalconfig.modifier3 != 0) {
                        Keyboard.press(generalconfig.modifier3);
                    }
                    Keyboard.press(KEY_F7);
                    Keyboard.releaseAll();
                    delay(generalconfig.helperdelay);
                    break;
                case 8:
                    if (generalconfig.modifier1 != 0) {
                        Keyboard.press(generalconfig.modifier1);
                    }
                    if (generalconfig.modifier2 != 0) {
                        Keyboard.press(generalconfig.modifier2);
                    }
                    if (generalconfig.modifier3 != 0) {
                        Keyboard.press(generalconfig.modifier3);
                    }
                    Keyboard.press(KEY_F8);
                    Keyboard.releaseAll();
                    delay(generalconfig.helperdelay);
                    break;
                case 9:
                    if (generalconfig.modifier1 != 0) {
                        Keyboard.press(generalconfig.modifier1);
                    }
                    if (generalconfig.modifier2 != 0) {
                        Keyboard.press(generalconfig.modifier2);
                    }
                    if (generalconfig.modifier3 != 0) {
                        Keyboard.press(generalconfig.modifier3);
                    }
                    Keyboard.press(KEY_F9);
                    Keyboard.releaseAll();
                    delay(generalconfig.helperdelay);
                    break;
                case 10:
                    if (generalconfig.modifier1 != 0) {
                        Keyboard.press(generalconfig.modifier1);
                    }
                    if (generalconfig.modifier2 != 0) {
                        Keyboard.press(generalconfig.modifier2);
                    }
                    if (generalconfig.modifier3 != 0) {
                        Keyboard.press(generalconfig.modifier3);
                    }
                    Keyboard.press(KEY_F10);
                    Keyboard.releaseAll();
                    delay(generalconfig.helperdelay);
                    break;
                case 11:
                    if (generalconfig.modifier1 != 0) {
                        Keyboard.press(generalconfig.modifier1);
                    }
                    if (generalconfig.modifier2 != 0) {
                        Keyboard.press(generalconfig.modifier2);
                    }
                    if (generalconfig.modifier3 != 0) {
                        Keyboard.press(generalconfig.modifier3);
                    }
                    Keyboard.press(KEY_F11);
                    Keyboard.releaseAll();
                    delay(generalconfig.helperdelay);
                    break;
            }
            break;


        case Action_SpecialFn:  // Special functions
            switch (value) {
                case SpecialFn_ConfigMode:  // Enter config mode

                    configmode();

                    break;
                case SpecialFn_DisplayBrightnessDown:  // Display Brightness Down
                    if (ledBrightness > 25) {
                        ledBrightness = ledBrightness - 25;
                        ledcWrite(0, ledBrightness);
                        savedStates.putInt("ledBrightness", ledBrightness);
                        MSG_INFOLN("[INFO] Brightness down.");
                    }
                    break;
                case SpecialFn_DisplayBrightnessUp:  // Display Brightness Up
                    if (ledBrightness < 230) {
                        ledBrightness = ledBrightness + 25;
                        ledcWrite(0, ledBrightness);
                        savedStates.putInt("ledBrightness", ledBrightness);
                        MSG_INFOLN("[INFO] Brightness up.");
                    }
                    break;
                case SpecialFn_SleepEnable:  // Sleep Enabled
                    if (generalconfig.sleepenable) {
                        generalconfig.sleepenable = false;
                        MSG_INFOLN("[INFO] Sleep disabled.");
                    }
                    else {
                        generalconfig.sleepenable = true;
                        Interval = generalconfig.sleeptimer * MIN_TO_MS;
                        MSG_INFOLN("[INFO] Sleep enabled.");
                        MSG_INFO("[INFO] Timer set to: ");
                        MSG_INFOLN(generalconfig.sleeptimer);
                    }
                    break;

                case SpecialFn_InfoPage:
                    callingPageNum = pageNum;
                    pageNum = SPECIAL_PAGE_INFO;
                    pageHistoryStack.push(pageNum);
                    if (generalconfig.usbcommsenable) {
                        char usbData[40];
                        snprintf(usbData, sizeof(usbData), "{NewPage, %s , %s}", menu[callingPageNum].name, "Info");
                        Serial.println(usbData);
                    }
                    break;

                case SpecialFn_HomePage:
                    callingPageNum = pageNum;
                    pageNum = 0;
                    pageHistoryStack.push(pageNum);
                    if (generalconfig.usbcommsenable) {
                        char usbData[40];
                        snprintf(usbData, sizeof(usbData), "{NewPage, %s , %s}", menu[callingPageNum].name, "Home");
                        Serial.println(usbData);
                    }
                    drawKeypad();
                    break;

                case SpecialFn_SaveConfig:
                    saveCurrentConfigGeneral();
                    break;

                case SpecialFn_USBComm:  // USB Comms Enable/Disable
                    if (generalconfig.usbcommsenable) {
                        generalconfig.usbcommsenable = false;
                        MSG_INFOLN("[INFO] USB Comms Disabled.");
                    }
                    else {
                        generalconfig.usbcommsenable = true;
                        MSG_INFOLN("[INFO] USB Comms Enabled.");
                    }
                    break;

                case SpecialFn_IOMonitor:
                    callingPageNum = pageNum;
                    pageNum = SPECIAL_PAGE_IO_MONITOR;
                    pageHistoryStack.push(pageNum);
                    if (generalconfig.usbcommsenable) {
                        char usbData[40];
                        snprintf(usbData, sizeof(usbData), "{NewPage, %s , %s}", menu[callingPageNum].name, "IO Monitor");
                        Serial.println(usbData);
                    }

                    break;
				case SpecialFn_GPIO_Toggle:
                    generalconfig.gpio_pin_mode = !generalconfig.gpio_pin_mode;
                    digitalWrite(generalconfig.gpio_pin, generalconfig.gpio_pin_mode);
                    MSG_INFOLN("[INFO] Toggle GPIO.");
                    break;
                case SpecialFn_GPIO_Off:
                    generalconfig.gpio_pin_mode = LOW;
                    digitalWrite(generalconfig.gpio_pin, generalconfig.gpio_pin_mode);
                    MSG_INFOLN("[INFO] Set GPIO off.");
                    break;
                case SpecialFn_GPIO_On:
                    generalconfig.gpio_pin_mode = HIGH;
                    digitalWrite(generalconfig.gpio_pin, generalconfig.gpio_pin_mode);
                    MSG_INFOLN("[INFO] Set GPIO on.");
                    break;
            }
            break;


        case Action_Numpad:  // Numpad
            switch (value) {
                case 0:
                    Keyboard.write(KEY_NUM_0);
                    break;
                case 1:
                    Keyboard.write(KEY_NUM_1);
                    break;
                case 2:
                    Keyboard.write(KEY_NUM_2);
                    break;
                case 3:
                    Keyboard.write(KEY_NUM_3);
                    break;
                case 4:
                    Keyboard.write(KEY_NUM_4);
                    break;
                case 5:
                    Keyboard.write(KEY_NUM_5);
                    break;
                case 6:
                    Keyboard.write(KEY_NUM_6);
                    break;
                case 7:
                    Keyboard.write(KEY_NUM_7);
                    break;
                case 8:
                    Keyboard.write(KEY_NUM_8);
                    break;
                case 9:
                    Keyboard.write(KEY_NUM_9);
                    break;
                case 10:
                    Keyboard.write(KEY_NUM_SLASH);
                    break;
                case 11:
                    Keyboard.write(KEY_NUM_ASTERIX);
                    break;
                case 12:
                    Keyboard.write(KEY_NUM_MINUS);
                    break;
                case 13:
                    Keyboard.write(KEY_NUM_PLUS);
                    break;
                case 14:
                    Keyboard.write(KEY_NUM_ENTER);
                    break;
                case 15:
                    Keyboard.write(KEY_NUM_PERIOD);
                    break;
            }
            break;
        case Action_CustomFn:  // Custom functions
            switch (value) {
                case 1:
                    userAction1();
                    break;
                case 2:
                    userAction2();
                    break;
                case 3:
                    userAction3();
                    break;
                case 4:
                    userAction4();
                    break;
                case 5:
                    userAction5();
                    break;
                case 6:
                    userAction6();
                    break;
                case 7:
                    userAction7();
                    break;
            }
            break;
        case Action_ChangePage:  // Custom functions
            if ((value >= 0) && (value < NUM_PAGES)) {
                int callingPageNum = pageNum;
                pageNum = value;
                pageHistoryStack.push(pageNum);
                if (generalconfig.usbcommsenable) {
                    char usbData[40];
                    snprintf(usbData, sizeof(usbData), "{NewPage, %s , %s}", menu[callingPageNum].name, menu[pageNum].name);
                    Serial.println(usbData);
                }
                drawKeypad();
            }
            break;

        case Action_CADFn:  // Special CAD functions
            switch (value) {
                case CADFn_SetCADProgram:
                    break;

                case CADFn_CalibrateZero:
                    calibrate_zero_analog_controls();
                    break;

                case CADFn_CalibrateControlScale:
                    calibrate_scale_analog_controls();
                    break;

                case CADFn_Spare1:
                    MSG_ERRORLN("[ERROR] Invalid CAD Function 4");
                    break;

                case CADFn_Spare2:
                    MSG_ERRORLN("[ERROR] Invalid CAD Function 5");
                    break;

                case CADFn_InvertJoyScaleX:
                    joystick.SetScale(0, joystick.GetScale(0) * -1.0);
                    cadconfig.joy_scale_x = joystick.GetScale(0);
                    break;

                case CADFn_InvertJoyScaleY:
                    joystick.SetScale(1, joystick.GetScale(1) * -1.0);
                    cadconfig.joy_scale_y = joystick.GetScale(1);
                    break;

                case CADFn_InvertScaleZoom:
                    zoomControl.SetScale(zoomControl.GetScale() * -1.0);
                    cadconfig.zoom_scale = zoomControl.GetScale();
                    break;

                case CADFn_InvertScaleRotate:
                    rotateControl.SetScale(rotateControl.GetScale() * -1.0);
                    cadconfig.rotate_scale = rotateControl.GetScale();
                    break;

                case CADFn_SaveCADConfig:
                    saveCurrentConfigCADParams();
                    break;

                case CADFn_Spare3:
                    MSG_ERRORLN("[ERROR] Invalid CAD Function 11");
                    break;

                case CADFn_Spare4:
                    MSG_ERRORLN("[ERROR] Invalid CAD Function 12");
                    break;

                case CADFn_Spare5:
                    MSG_ERRORLN("[ERROR] Invalid CAD Function 13");
                    break;

                case CADFn_Spare6:
                    MSG_ERRORLN("[ERROR] Invalid CAD Function 14");
                    break;

                case CADFn_ModeSelect:
                    mode_select_button_pin = HWButton_Pins[hwbutton_index];
                    break;
            }
            break;

        case Action_CADProgram:  // Select CAD program
            if ((value >= 0) && (value < NUM_CAD_PROGRAMS)) {
                cadconfig.current_program = value;
                if (generalconfig.usbcommsenable) {
                    char usbData[40];
                    snprintf(usbData, sizeof(usbData), "{CADProgram, %s}", cadprogramconfig[cadconfig.current_program].name);
                    Serial.println(usbData);
                }
            }
            break;
        case Action_MouseButton:
            switch (value) {
                case 1:
                    Mouse.press(MOUSE_LEFT);
                    break;
                case 2:
                    Mouse.press(MOUSE_RIGHT);
                    break;
                case 3:
                    Mouse.press(MOUSE_MIDDLE);
                    break;
                case 4:
                    Mouse.press(MOUSE_LEFT);
                    Mouse.press(MOUSE_MIDDLE);
                    break;
                case 5:
                    Mouse.press(MOUSE_RIGHT);
                    Mouse.press(MOUSE_MIDDLE);
                    break;
                case 6:
                    Mouse.release(MOUSE_LEFT);
                    break;
                case 7:
                    Mouse.release(MOUSE_RIGHT);
                    break;
                case 8:
                    Mouse.release(MOUSE_MIDDLE);
                    break;
                case 9:
                    Mouse.release(MOUSE_LEFT);
                    Mouse.release(MOUSE_MIDDLE);
                    break;
                case 10:
                    Mouse.release(MOUSE_RIGHT);
                    Mouse.release(MOUSE_MIDDLE);
                    break;
                case 11:
                    Mouse.release(MOUSE_LEFT);
                    Mouse.release(MOUSE_RIGHT);
                    Mouse.release(MOUSE_MIDDLE);
                    break;
                default:
                    break;
            }
            break;

        case Action_PreviousPage:  // Previous page
            if (pageHistoryStack.size() > 1) {
                pageHistoryStack.pop();
                pageNum = pageHistoryStack.peek();
                drawKeypad();
            }
            break;

        default:
            // If nothing matches do nothing
            break;
    }
}
