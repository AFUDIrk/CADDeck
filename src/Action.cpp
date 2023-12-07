#include "Action.h"

#include "BleCombo.h"
#include "SaveConfig.h"

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
* @note Case Action_SpecialFn is used for special functions, not bleKeyboard related.
*/
void KeyboardMouseAction(int action, int value, char *symbol, uint8_t hwbutton_index)
{
    int callingPageNum;
    uint8_t status;

    switch (action) {
        case Action_NoAction:
            // No Action
            break;
        case Action_Delay:  // Delay
            delay(value);
            break;
        case Action_TabArrow:  // Send TAB ARROW etc
            switch (value) {
                case ActionTabArrowKey_Up:
                    Keyboard.write(KEY_UP_ARROW);
                    break;
                case ActionTabArrowKey_Down:
                    Keyboard.write(KEY_DOWN_ARROW);
                    break;
                case ActionTabArrowKey_Left:
                    Keyboard.write(KEY_LEFT_ARROW);
                    break;
                case ActionTabArrowKey_Right:
                    Keyboard.write(KEY_RIGHT_ARROW);
                    break;
                case ActionTabArrowKey_Backspace:
                    Keyboard.write(KEY_BACKSPACE);
                    break;
                case ActionTabArrowKey_Tab:
                    Keyboard.write(KEY_TAB);
                    break;
                case ActionTabArrowKey_Enter:
                    Keyboard.write(KEY_RETURN);
                    break;
                case ActionTabArrowKey_PageUp:
                    Keyboard.write(KEY_PAGE_UP);
                    break;
                case ActionTabArrowKey_PageDown:
                    Keyboard.write(KEY_PAGE_DOWN);
                    break;
                case ActionTabArrowKey_Delete:
                    Keyboard.write(KEY_DELETE);
                    break;
                case ActionTabArrowKey_PrtSc:
                    Keyboard.write(KEY_PRTSC);
                    break;
                case ActionTabArrowKey_Esc:
                    Keyboard.write(KEY_ESC);
                    break;
                case ActionTabArrowKey_Home:
                    Keyboard.write(KEY_HOME);
                    break;
                case ActionTabArrowKey_End:
                    Keyboard.write(KEY_END);
                    break;

                default:
                    // if nothing matches do nothing
                    break;
            }
            break;
        case Action_Media:  // Send Media Key
            switch (value) {
                case ActionMediaKey_Mute:
                    Keyboard.write(KEY_MEDIA_MUTE);
                    break;
                case ActionMediaKey_VolumeDown:
                    Keyboard.write(KEY_MEDIA_VOLUME_DOWN);
                    break;
                case ActionMediaKey_VolumeUp:
                    Keyboard.write(KEY_MEDIA_VOLUME_UP);
                    break;
                case ActionMediaKey_PlayPause:
                    Keyboard.write(KEY_MEDIA_PLAY_PAUSE);
                    break;
                case ActionMediaKey_Stop:
                    Keyboard.write(KEY_MEDIA_STOP);
                    break;
                case ActionMediaKey_NextTrack:
                    Keyboard.write(KEY_MEDIA_NEXT_TRACK);
                    break;
                case ActionMediaKey_PreviousTrack:
                    Keyboard.write(KEY_MEDIA_PREVIOUS_TRACK);
                    break;
                case ActionMediaKey_WWWHome:
                    Keyboard.write(KEY_MEDIA_WWW_HOME);
                    break;
                case ActionMediaKey_LocalMachineBrowser:
                    Keyboard.write(KEY_MEDIA_LOCAL_MACHINE_BROWSER);
                    break;
                case ActionMediaKey_Calculator:
                    Keyboard.write(KEY_MEDIA_CALCULATOR);
                    break;
                case ActionMediaKey_WWWBookmarks:
                    Keyboard.write(KEY_MEDIA_WWW_BOOKMARKS);
                    break;
                case ActionMediaKey_WWWSearch:
                    Keyboard.write(KEY_MEDIA_WWW_SEARCH);
                    break;
                case ActionMediaKey_WWWStop:
                    Keyboard.write(KEY_MEDIA_WWW_STOP);
                    break;
                case ActionMediaKey_WWWBack:
                    Keyboard.write(KEY_MEDIA_WWW_BACK);
                    break;
                case ActionMediaKey_ConsumerControlConfiguration:
                    Keyboard.write(KEY_MEDIA_CONSUMER_CONTROL_CONFIGURATION);
                    break;
                case ActionMediaKey_EmailReader:
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
                case OptionKey_LCtrl:
                    Keyboard.press(KEY_LEFT_CTRL);
                    break;
                case OptionKey_LShift:
                    Keyboard.press(KEY_LEFT_SHIFT);
                    break;
                case OptionKey_LAlt:
                    Keyboard.press(KEY_LEFT_ALT);
                    break;
                case OptionKey_LGui:
                    Keyboard.press(KEY_LEFT_GUI);
                    break;
                case OptionKey_RCtrl:
                    Keyboard.press(KEY_RIGHT_CTRL);
                    break;
                case OptionKey_RShift:
                    Keyboard.press(KEY_RIGHT_SHIFT);
                    break;
                case OptionKey_RAlt:
                    Keyboard.press(KEY_RIGHT_ALT);
                    break;
                case OptionKey_RGui:
                    Keyboard.press(KEY_RIGHT_GUI);
                    break;
                case OptionKey_ReleaseAll:
                    Keyboard.releaseAll();
                    break;
                default:
                    // if nothing matches do nothing
                    break;
            }
            break;
        case Action_FnKey:  // Function Keys
            switch (value) {
                case ActionFnKey_F1:
                    Keyboard.press(KEY_F1);
                    break;
                case ActionFnKey_F2:
                    Keyboard.press(KEY_F2);
                    break;
                case ActionFnKey_F3:
                    Keyboard.press(KEY_F3);
                    break;
                case ActionFnKey_F4:
                    Keyboard.press(KEY_F4);
                    break;
                case ActionFnKey_F5:
                    Keyboard.press(KEY_F5);
                    break;
                case ActionFnKey_F6:
                    Keyboard.press(KEY_F6);
                    break;
                case ActionFnKey_F7:
                    Keyboard.press(KEY_F7);
                    break;
                case ActionFnKey_F8:
                    Keyboard.press(KEY_F8);
                    break;
                case ActionFnKey_F9:
                    Keyboard.press(KEY_F9);
                    break;
                case ActionFnKey_F10:
                    Keyboard.press(KEY_F10);
                    break;
                case ActionFnKey_F11:
                    Keyboard.press(KEY_F11);
                    break;
                case ActionFnKey_F12:
                    Keyboard.press(KEY_F12);
                    break;
                case ActionFnKey_F13:
                    Keyboard.press(KEY_F13);
                    break;
                case ActionFnKey_F14:
                    Keyboard.press(KEY_F14);
                    break;
                case ActionFnKey_F15:
                    Keyboard.press(KEY_F15);
                    break;
                case ActionFnKey_F16:
                    Keyboard.press(KEY_F16);
                    break;
                case ActionFnKey_F17:
                    Keyboard.press(KEY_F17);
                    break;
                case ActionFnKey_F18:
                    Keyboard.press(KEY_F18);
                    break;
                case ActionFnKey_F19:
                    Keyboard.press(KEY_F19);
                    break;
                case ActionFnKey_F20:
                    Keyboard.press(KEY_F20);
                    break;
                case ActionFnKey_F21:
                    Keyboard.press(KEY_F21);
                    break;
                case ActionFnKey_F22:
                    Keyboard.press(KEY_F22);
                    break;
                case ActionFnKey_F23:
                    Keyboard.press(KEY_F23);
                    break;
                case ActionFnKey_F24:
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
                    }
                    break;
                case SpecialFn_DisplayBrightnessUp:  // Display Brightness Up
                    if (ledBrightness < 230) {
                        ledBrightness = ledBrightness + 25;
                        ledcWrite(0, ledBrightness);
                        savedStates.putInt("ledBrightness", ledBrightness);
                    }
                    break;
                case SpecialFn_SleepEnable:  // Sleep Enabled
                    if (generalconfig.sleepenable) {
                        generalconfig.sleepenable = false;
                    }
                    else {
                        generalconfig.sleepenable = true;
                        Interval = generalconfig.sleeptimer * MIN_TO_MS;
                    }
                    break;

                case SpecialFn_InfoPage:
                    callingPageNum = pageNum;

                    status = pageHistoryStack.push(pageNum);
                    if (status == STACK_STATUS_FULL) {
                        MSG_INFOLN("[INFO] Page History Stack is full. Dropped oldest value..");
                    }
                    pageNum = SPECIAL_PAGE_INFO;

                    if (generalconfig.usbcommsenable) {
                        char usbData[40];
                        snprintf(usbData, sizeof(usbData), "{NewPage, %s , %s}", menu[callingPageNum].name, "Info");
                        Serial.println(usbData);
                    }
                    break;

                case SpecialFn_HomePage:
                    callingPageNum = pageNum;

                    status = pageHistoryStack.push(pageNum);
                    if (status == STACK_STATUS_FULL) {
                        MSG_INFOLN("[INFO] Page History Stack is full. Dropped oldest value..");
                    }
                    pageNum = 0;

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
                    }
                    else {
                        generalconfig.usbcommsenable = true;
                    }
                    break;

                case SpecialFn_IOMonitor:
                    callingPageNum = pageNum;

                    status = pageHistoryStack.push(pageNum);
                    if (status == STACK_STATUS_FULL) {
                        MSG_INFOLN("[INFO] Page History Stack is full. Dropped oldest value..");
                    }
                    pageNum = SPECIAL_PAGE_IO_MONITOR;

                    if (generalconfig.usbcommsenable) {
                        char usbData[40];
                        snprintf(usbData, sizeof(usbData), "{NewPage, %s , %s}", menu[callingPageNum].name, "IO Monitor");
                        Serial.println(usbData);
                    }
                    break;
                case SpecialFn_GPIO_Toggle:
                    generalconfig.gpio_pin_mode = !generalconfig.gpio_pin_mode;
                    digitalWrite(generalconfig.gpio_pin, generalconfig.gpio_pin_mode);
                    break;
                case SpecialFn_GPIO_Off:
                    generalconfig.gpio_pin_mode = LOW;
                    digitalWrite(generalconfig.gpio_pin, generalconfig.gpio_pin_mode);
                    break;
                case SpecialFn_GPIO_On:
                    generalconfig.gpio_pin_mode = HIGH;
                    digitalWrite(generalconfig.gpio_pin, generalconfig.gpio_pin_mode);
                    break;
                case SpecialFn_ButtonInfoPage:
                    callingPageNum = pageNum;

                    status = pageHistoryStack.push(pageNum);
                    if (status == STACK_STATUS_FULL) {
                        MSG_INFOLN("[INFO] Page History Stack is full. Dropped oldest value..");
                    }
                    pageNum = SPECIAL_PAGE_BUTTON_INFO;

                    if (generalconfig.usbcommsenable) {
                        char usbData[40];
                        snprintf(usbData, sizeof(usbData), "{NewPage, %s , %s}", menu[callingPageNum].name, "ButtonInfo");
                        Serial.println(usbData);
                    }
                    break;
                case SpecialFn_Spacemouse_Enable_Toggle:
                    cadconfig.spacemouse_enable = !cadconfig.spacemouse_enable;
                    if (cadconfig.spacemouse_enable) {
                        MSG_INFOLN("[DEBUG] Spacemouse Mode Enabled.");
                    }
                    else {
                        MSG_INFOLN("[DEBUG] Spacemouse Mode Disabled.");
                    }
                    break;
            }
            break;

        case Action_Numpad:  // Numpad
            switch (value) {
                case ActionNumpadKey_0:
                    Keyboard.write(KEY_NUM_0);
                    break;
                case ActionNumpadKey_1:
                    Keyboard.write(KEY_NUM_1);
                    break;
                case ActionNumpadKey_2:
                    Keyboard.write(KEY_NUM_2);
                    break;
                case ActionNumpadKey_3:
                    Keyboard.write(KEY_NUM_3);
                    break;
                case ActionNumpadKey_4:
                    Keyboard.write(KEY_NUM_4);
                    break;
                case ActionNumpadKey_5:
                    Keyboard.write(KEY_NUM_5);
                    break;
                case ActionNumpadKey_6:
                    Keyboard.write(KEY_NUM_6);
                    break;
                case ActionNumpadKey_7:
                    Keyboard.write(KEY_NUM_7);
                    break;
                case ActionNumpadKey_8:
                    Keyboard.write(KEY_NUM_8);
                    break;
                case ActionNumpadKey_9:
                    Keyboard.write(KEY_NUM_9);
                    break;
                case ActionNumpadKey_Slash:
                    Keyboard.write(KEY_NUM_SLASH);
                    break;
                case ActionNumpadKey_Asterix:
                    Keyboard.write(KEY_NUM_ASTERIX);
                    break;
                case ActionNumpadKey_Minus:
                    Keyboard.write(KEY_NUM_MINUS);
                    break;
                case ActionNumpadKey_Plus:
                    Keyboard.write(KEY_NUM_PLUS);
                    break;
                case ActionNumpadKey_Enter:
                    Keyboard.write(KEY_NUM_ENTER);
                    break;
                case ActionNumpadKey_Period:
                    Keyboard.write(KEY_NUM_PERIOD);
                    break;
            }
            break;
        case Action_CustomFn:  // Custom functions
            switch (value) {
                case ActionCustomFnKey_1:
                    userAction1();
                    break;
                case ActionCustomFnKey_2:
                    userAction2();
                    break;
                case ActionCustomFnKey_3:
                    userAction3();
                    break;
                case ActionCustomFnKey_4:
                    userAction4();
                    break;
                case ActionCustomFnKey_5:
                    userAction5();
                    break;
                case ActionCustomFnKey_6:
                    userAction6();
                    break;
                case ActionCustomFnKey_7:
                    userAction7();
                    break;
            }
            break;
        case Action_ChangePage:  // Custom functions
            if ((value >= 0) && (value < NUM_PAGES)) {
                int callingPageNum = pageNum;

                status = pageHistoryStack.push(pageNum);
                if (status == STACK_STATUS_FULL) {
                    MSG_INFOLN("[INFO] Page History Stack is full. Dropped oldest value..");
                }
                pageNum = value;

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
                case MouseButton_PL:
                    Mouse.press(MOUSE_LEFT);
                    break;
                case MouseButton_PR:
                    Mouse.press(MOUSE_RIGHT);
                    break;
                case MouseButton_PM:
                    Mouse.press(MOUSE_MIDDLE);
                    break;
                case MouseButton_PLM:
                    Mouse.press(MOUSE_LEFT);
                    Mouse.press(MOUSE_MIDDLE);
                    break;
                case MouseButton_PRM:
                    Mouse.press(MOUSE_RIGHT);
                    Mouse.press(MOUSE_MIDDLE);
                    break;
                case MouseButton_RL:
                    Mouse.release(MOUSE_LEFT);
                    break;
                case MouseButton_RR:
                    Mouse.release(MOUSE_RIGHT);
                    break;
                case MouseButton_RM:
                    Mouse.release(MOUSE_MIDDLE);
                    break;
                case MouseButton_RLM:
                    Mouse.release(MOUSE_LEFT);
                    Mouse.release(MOUSE_MIDDLE);
                    break;
                case MouseButton_RRM:
                    Mouse.release(MOUSE_RIGHT);
                    Mouse.release(MOUSE_MIDDLE);
                    break;
                case MouseButton_RLRM:
                    Mouse.release(MOUSE_LEFT);
                    Mouse.release(MOUSE_RIGHT);
                    Mouse.release(MOUSE_MIDDLE);
                    break;
                default:
                    break;
            }
            break;

        case Action_PreviousPage:  // Previous page
            callingPageNum = pageNum;
            if (pageHistoryStack.size() > 1) {
                pageNum = pageHistoryStack.pop(&status);

                if (!isValidPageNumber(pageNum)) {
                    MSG_WARN1("[WARN] Action_PreviousPage: Invalid page number: ", pageNum);
                    pageNum = 0;
                }
            }
            else {
                // If the stack is empty, go to the home page
                status = pageHistoryStack.push(pageNum);
                pageNum = 0;
                MSG_WARN1("[WARN] Action_PreviousPage: Stack was empty. Switching to home page: ", pageNum);
            }

            if (generalconfig.usbcommsenable) {
                char usbData[40];
                snprintf(usbData, sizeof(usbData), "{NewPage, %s , %s}", menu[callingPageNum].name, menu[pageNum].name);
                Serial.println(usbData);
            }

            drawKeypad();
            break;

        case Action_SpaceMouseButton:  // Emulate a SpaceMouse button press as long as the button number is valid
            if ((value > (cadprogramconfig[cadconfig.current_program].num_buttons - 1)) && (value <= SPACEMOUSE_MAX_BUTTON)) {
                spaceMouse.SetButton(value);
            }
            else {
                MSG_WARN1("[WARN] Action_SpaceMouseButton: Invalid SpaceMouse button number", value);
            }
            break;

        default:
            // If nothing matches do nothing
            break;
    }
}
