#include "SaveConfig.h"

int saveConfigGeneral(AsyncWebServerRequest *request)
{
    int status = 0;

    // --- Saving general config
    MSG_INFOLN("[INFO] Allgemeine Konfiguration speichern");

    FILESYSTEM.remove("/config/general.json");
    File file = FILESYSTEM.open("/config/general.json", "w");
    if (!file) {
        MSG_WARNLN("[WARNUNG]: Datei /config/general.json konnte nicht erstellt werden");
        status = 1;
        return status;
    }

    DynamicJsonDocument doc(400);

    JsonObject general = doc.to<JsonObject>();

    AsyncWebParameter *menubuttoncolor = request->getParam("menubuttoncolor", true);
    general["menubuttoncolor"] = menubuttoncolor->value().c_str();
    AsyncWebParameter *functionbuttoncolor = request->getParam("functionbuttoncolor", true);
    general["functionbuttoncolor"] = functionbuttoncolor->value().c_str();
    AsyncWebParameter *latchcolor = request->getParam("latchcolor", true);
    general["latchcolor"] = latchcolor->value().c_str();
    AsyncWebParameter *background = request->getParam("background", true);
    general["background"] = background->value().c_str();

    AsyncWebParameter *sleepenable = request->getParam("sleepenable", true);
    String sleepEnable = sleepenable->value().c_str();

    if (sleepEnable == "true") {
        general["sleepenable"] = true;
    }
    else {
        general["sleepenable"] = false;
    }

    AsyncWebParameter *usbcommsenable = request->getParam("usbcommsenable", true);
    String usbcommsEnable = usbcommsenable->value().c_str();

    if (usbcommsEnable == "true") {
        general["usbcommsenable"] = true;
    }
    else {
        general["usbcommsenable"] = false;
    }

    AsyncWebParameter *beep = request->getParam("beep", true);
    String Beep = beep->value().c_str();

    if (Beep == "true") {
        general["beep"] = true;
    }
    else {
        general["beep"] = false;
    }

    // Sleep timer
    AsyncWebParameter *sleeptimer = request->getParam("sleeptimer", true);

    String sleepTimer = sleeptimer->value().c_str();
    general["sleeptimer"] = sleepTimer.toInt();

    // Modifiers
    
	AsyncWebParameter *modifier1 = request->getParam("modifier1", true);
    String Modifier1 = modifier1->value().c_str();
    general["modifier1"] = Modifier1.toInt();

    AsyncWebParameter *modifier2 = request->getParam("modifier2", true);
    String Modifier2 = modifier2->value().c_str();
    general["modifier2"] = Modifier2.toInt();

    AsyncWebParameter *modifier3 = request->getParam("modifier3", true);
    String Modifier3 = modifier3->value().c_str();
    general["modifier3"] = Modifier3.toInt();

    AsyncWebParameter *helperdelay = request->getParam("helperdelay", true);
    String Helperdelay = helperdelay->value().c_str();
    general["helperdelay"] = Helperdelay.toInt();

    AsyncWebParameter *startup_menu = request->getParam("startup_menu", true);
    String Startup_menu = startup_menu->value().c_str();
    general["startup_menu"] = Startup_menu.toInt();

AsyncWebParameter *gpio_pin = request->getParam("gpio_pin", true);
    String Gpio_pin = gpio_pin->value().c_str();
    general["gpio_pin"] = Gpio_pin.toInt();

    general["gpio_pin_mode"] = generalconfig.gpio_pin_mode;

    if (serializeJsonPretty(doc, file) == 0) {
        MSG_WARNLN("[WARNING]: Failed to write to /config/general.json file");
        status = 2;
    }
    file.close();

    return status;
}

int saveCurrentConfigGeneral()
{
    int status = 0;

    // --- Saving general config
    MSG_INFOLN("[INFO] Saving General Config");

    FILESYSTEM.remove("/config/general.json");
    File file = FILESYSTEM.open("/config/general.json", "w");
    if (!file) {
        MSG_WARNLN("[WARNING]: Failed to create /config/general.json file");
        status = 1;
        return status;
    }

    DynamicJsonDocument doc(400);

    JsonObject general = doc.to<JsonObject>();

    char htmlMenuButtonColor[20];
    char htmlFunctionButtonColour[20];
    char htmlLatchColor[20];
    char htmlBackgroundColor[20];

    convertRGB565toHTML(generalconfig.menuButtonColour, htmlMenuButtonColor, sizeof(htmlMenuButtonColor));
    general["menubuttoncolor"] = htmlMenuButtonColor;
    convertRGB565toHTML(generalconfig.functionButtonColour, htmlFunctionButtonColour, sizeof(htmlFunctionButtonColour));
    general["functionbuttoncolor"] = htmlFunctionButtonColour;
    convertRGB565toHTML(generalconfig.latchedColour, htmlLatchColor, sizeof(htmlLatchColor));
    general["latchcolor"] = htmlLatchColor;
    convertRGB565toHTML(generalconfig.backgroundColour, htmlBackgroundColor, sizeof(htmlBackgroundColor));
    general["background"] = htmlBackgroundColor;

    if (generalconfig.sleepenable) {
        general["sleepenable"] = true;
    }
    else {
        general["sleepenable"] = false;
    }

    if (generalconfig.usbcommsenable) {
        general["usbcommsenable"] = true;
    }
    else {
        general["usbcommsenable"] = false;
    }

    if (generalconfig.beep) {
        general["beep"] = true;
    }
    else {
        general["beep"] = false;
    }

    general["sleeptimer"] = generalconfig.sleeptimer;

    // Modifiers

    general["modifier1"] = generalconfig.modifier1;
    general["modifier2"] = generalconfig.modifier2;
    general["modifier3"] = generalconfig.modifier3;

    general["helperdelay"] = generalconfig.helperdelay;

    general["startup_menu"] = generalconfig.startup_menu;

	general["gpio_pin"] = generalconfig.gpio_pin;
    general["gpio_pin_mode"] = generalconfig.gpio_pin_mode;

    if (serializeJsonPretty(doc, file) == 0) {
        MSG_WARNLN("[WARNING]: Failed to write to /config/general.json file");
        status = 2;
    }
    file.close();

    return status;
}

int saveConfigCADParams(AsyncWebServerRequest *request)
{
    int status = 0;

    // --- Saving cadparams config
    MSG_INFOLN("[INFO] Saving CADParams Config");

    FILESYSTEM.remove("/config/cadparams.json");
    File file = FILESYSTEM.open("/config/cadparams.json", "w");
    if (!file) {
        MSG_WARNLN("[WARNING]: Failed to create /config/cadparams.json file");
        status = 1;
        return status;
    }
    MSG_INFOLN("[INFO] /config/cadparams.json opened for write");

    DynamicJsonDocument doc(16000);

    JsonObject cadparams = doc.to<JsonObject>();

    cadparams["version"] = CADCONFIG_VERSION;

    MSG_DEBUGLN("[DEBUG] Requesting joy_scale_x");
    AsyncWebParameter *scalex = request->getParam("joy_scale_x", true);
    String ScaleX = scalex->value().c_str();
    cadparams["joy_scale_x"] = ScaleX.toFloat();

    MSG_DEBUGLN("[DEBUG] Requesting joy_scale_y");
    AsyncWebParameter *scaley = request->getParam("joy_scale_y", true);
    String ScaleY = scaley->value().c_str();
    cadparams["joy_scale_y"] = ScaleY.toFloat();

    AsyncWebParameter *deadzone = request->getParam("joy_deadzone", true);
    String Deadzone = deadzone->value().c_str();
    cadparams["joy_deadzone"] = Deadzone.toFloat();

    AsyncWebParameter *sensitivity = request->getParam("joy_sensitivity", true);
    String Sensitivity = sensitivity->value().c_str();
    cadparams["joy_sensitivity"] = Sensitivity.toFloat();

    AsyncWebParameter *scalezoom = request->getParam("zoom_scale", true);
    String ScaleZoom = scalezoom->value().c_str();
    cadparams["zoom_scale"] = ScaleZoom.toFloat();

    AsyncWebParameter *deadzonezoom = request->getParam("zoom_deadzone", true);
    String DeadzoneZoom = deadzonezoom->value().c_str();
    cadparams["zoom_deadzone"] = DeadzoneZoom.toFloat();

    AsyncWebParameter *sensitivityzoom = request->getParam("zoom_sensitivity", true);
    String SensitivityZoom = sensitivityzoom->value().c_str();
    cadparams["zoom_sensitivity"] = SensitivityZoom.toFloat();

    AsyncWebParameter *scalerotate = request->getParam("rotate_scale", true);
    String ScaleRotate = scalerotate->value().c_str();
    cadparams["rotate_scale"] = ScaleRotate.toFloat();

    AsyncWebParameter *deadzonerotate = request->getParam("rotate_deadzone", true);
    String DeadzoneRotate = deadzonerotate->value().c_str();
    cadparams["rotate_deadzone"] = DeadzoneRotate.toFloat();

    AsyncWebParameter *sensitivityrotate = request->getParam("rotate_sensitivity", true);
    String SensitivityRotate = sensitivityrotate->value().c_str();
    cadparams["rotate_sensitivity"] = SensitivityRotate.toFloat();

    MSG_DEBUGLN("[DEBUG] Requesting current_program");
    AsyncWebParameter *current_program = request->getParam("current_program", true);
    String Current_program = current_program->value().c_str();
    cadparams["current_program"] = Current_program.toInt();

    // TODO: Fix this to include all of the program-specific parameters. Need to figure out how to do
    // this with the AsyncWebServerRequest object. Look at how the menu parameters are saved.
    // For the moment, ignore the web page and just save back the currently loaded parameters.
    //
    JsonArray programObject_programarray = cadparams.createNestedArray("programs");

    for (int program = 0; program < cadconfig.num_programs; program++) {
        MSG_DEBUG1("[DEBUG] Requesting partameters for program", program);
        JsonObject programObject = programObject_programarray.createNestedObject();

        programObject["name"] = cadprogramconfig[program].name;
        programObject["logo"] = cadprogramconfig[program].logo;
        programObject["default_joystick_mode"] = cadprogramconfig[program].default_joystick_mode;

        JsonArray programHWButtonsArray = programObject.createNestedArray("buttons");
        for (int button = 0; button < cadprogramconfig[program].num_buttons; button++) {
            JsonObject programButtonObject = programHWButtonsArray.createNestedObject();

            programButtonObject["name"] = "button" + String(button);

            JsonArray programButtonObject_actionarray = programButtonObject.createNestedArray("actionarray");

            programButtonObject_actionarray.add(String((cadprogramconfig[program].hw_buttons[button][0].action)));
            programButtonObject_actionarray.add(String((cadprogramconfig[program].hw_buttons[button][1].action)));
            programButtonObject_actionarray.add(String((cadprogramconfig[program].hw_buttons[button][2].action)));

            JsonArray programButtonObject_valuearray = programButtonObject.createNestedArray("valuearray");
            if (cadprogramconfig[program].hw_buttons[button][0].action == Action_Char || cadprogramconfig[program].hw_buttons[button][0].action == Action_SpecialChar) {
                programButtonObject_valuearray.add(cadprogramconfig[program].hw_buttons[button][0].symbol);
            }
            else {
                programButtonObject_valuearray.add(String((cadprogramconfig[program].hw_buttons[button][0].value)));
            }

            if (cadprogramconfig[program].hw_buttons[button][1].action == Action_Char || cadprogramconfig[program].hw_buttons[button][1].action == Action_SpecialChar) {
                programButtonObject_valuearray.add(cadprogramconfig[program].hw_buttons[button][1].symbol);
            }
            else {
                programButtonObject_valuearray.add(String((cadprogramconfig[program].hw_buttons[button][1].value)));
            }

            if (cadprogramconfig[program].hw_buttons[button][2].action == Action_Char || cadprogramconfig[program].hw_buttons[button][2].action == Action_SpecialChar) {
                programButtonObject_valuearray.add(cadprogramconfig[program].hw_buttons[button][2].symbol);
            }
            else {
                programButtonObject_valuearray.add(String((cadprogramconfig[program].hw_buttons[button][2].value)));
            }
        }
    }

    if (serializeJsonPretty(doc, file) == 0) {
        MSG_WARNLN("[WARNING]: Failed to write to /config/cadparams.json file");
        status = 2;
    }
#ifdef LOG_JSON_FILES
    else {
        serializeJsonPretty(doc, Serial);
    }
#endif
    file.close();

    return status;
}

int saveCurrentConfigCADParams()
{
    int status = 0;

    // --- Saving cadparams config
    MSG_INFOLN("[INFO] Saving CADParams");

    FILESYSTEM.remove("/config/cadparams.json");
    File file = FILESYSTEM.open("/config/cadparams.json", "w");
    if (!file) {
        MSG_WARNLN("[WARNING]: Failed to create /config/cadparams.json file");
        status = 1;
        return status;
    }

    DynamicJsonDocument doc(16000);

    JsonObject cadparams = doc.to<JsonObject>();

    cadparams["version"] = CADCONFIG_VERSION;

    cadparams["joy_scale_x"] = cadconfig.joy_scale_x;
    cadparams["joy_scale_y"] = cadconfig.joy_scale_y;
    cadparams["joy_deadzone"] = cadconfig.joy_deadzone;
    cadparams["joy_sensitivity"] = cadconfig.joy_sensitivity;
    cadparams["zoom_scale"] = cadconfig.zoom_scale;
    cadparams["zoom_deadzone"] = cadconfig.zoom_deadzone;
    cadparams["zoom_sensitivity"] = cadconfig.zoom_sensitivity;
    cadparams["rotate_scale"] = cadconfig.rotate_scale;
    cadparams["rotate_deadzone"] = cadconfig.rotate_deadzone;
    cadparams["rotate_sensitivity"] = cadconfig.rotate_sensitivity;
    cadparams["current_program"] = cadconfig.current_program;

    JsonArray programObject_programarray = cadparams.createNestedArray("programs");

    for (int program = 0; program < cadconfig.num_programs; program++) {
        JsonObject programObject = programObject_programarray.createNestedObject();

        programObject["name"] = cadprogramconfig[program].name;
        programObject["logo"] = cadprogramconfig[program].logo;
        programObject["default_joystick_mode"] = cadprogramconfig[program].default_joystick_mode;

        JsonArray programHWButtonsArray = programObject.createNestedArray("buttons");
        for (int button = 0; button < cadprogramconfig[program].num_buttons; button++) {
            JsonObject programButtonObject = programHWButtonsArray.createNestedObject();

            programButtonObject["name"] = "button" + String(button);

            JsonArray programButtonObject_actionarray = programButtonObject.createNestedArray("actionarray");

            programButtonObject_actionarray.add(String((cadprogramconfig[program].hw_buttons[button][0].action)));
            programButtonObject_actionarray.add(String((cadprogramconfig[program].hw_buttons[button][1].action)));
            programButtonObject_actionarray.add(String((cadprogramconfig[program].hw_buttons[button][2].action)));

            JsonArray programButtonObject_valuearray = programButtonObject.createNestedArray("valuearray");
            if (cadprogramconfig[program].hw_buttons[button][0].action == Action_Char || cadprogramconfig[program].hw_buttons[button][0].action == Action_SpecialChar) {
                programButtonObject_valuearray.add(cadprogramconfig[program].hw_buttons[button][0].symbol);
            }
            else {
                programButtonObject_valuearray.add(String((cadprogramconfig[program].hw_buttons[button][0].value)));
            }

            if (cadprogramconfig[program].hw_buttons[button][1].action == Action_Char || cadprogramconfig[program].hw_buttons[button][1].action == Action_SpecialChar) {
                programButtonObject_valuearray.add(cadprogramconfig[program].hw_buttons[button][1].symbol);
            }
            else {
                programButtonObject_valuearray.add(String((cadprogramconfig[program].hw_buttons[button][1].value)));
            }

            if (cadprogramconfig[program].hw_buttons[button][2].action == Action_Char || cadprogramconfig[program].hw_buttons[button][2].action == Action_SpecialChar) {
                programButtonObject_valuearray.add(cadprogramconfig[program].hw_buttons[button][2].symbol);
            }
            else {
                programButtonObject_valuearray.add(String((cadprogramconfig[program].hw_buttons[button][2].value)));
            }
        }
    }

    if (serializeJsonPretty(doc, file) == 0) {
        MSG_WARNLN("[WARNING]: Failed to write to /config/cadparams.json file");
        status = 2;
    }
#ifdef LOG_JSON_FILES
    else {
        serializeJsonPretty(doc, Serial);
    }
#endif
    file.close();

    return status;
}

int saveConfigWiFi(AsyncWebServerRequest *request)
{
    int status = 0;

    // --- Saving wifi config
    MSG_INFOLN("[INFO] Saving Wifi Config");

    FILESYSTEM.remove("/config/wificonfig.json");
    File file = FILESYSTEM.open("/config/wificonfig.json", "w");
    if (!file) {
        MSG_WARNLN("[WARNING]: Failed to create /config/wificonfig.json file");
        status = 1;
        return status;
    }

    DynamicJsonDocument doc(384);

    JsonObject wifi = doc.to<JsonObject>();

    AsyncWebParameter *menubuttoncolor = request->getParam("ssid", true);
    wifi["ssid"] = menubuttoncolor->value().c_str();
    AsyncWebParameter *functionbuttoncolor = request->getParam("password", true);
    wifi["password"] = functionbuttoncolor->value().c_str();
    AsyncWebParameter *latchcolor = request->getParam("wifimode", true);
    wifi["wifimode"] = latchcolor->value().c_str();
    AsyncWebParameter *background = request->getParam("wifihostname", true);
    wifi["wifihostname"] = background->value().c_str();

    AsyncWebParameter *attempts = request->getParam("attempts", true);
    String Attempts = attempts->value().c_str();
    wifi["attempts"] = Attempts.toInt();

    AsyncWebParameter *attemptdelay = request->getParam("attemptdelay", true);
    String Attemptdelay = attemptdelay->value().c_str();
    wifi["attemptdelay"] = Attemptdelay.toInt();

    if (serializeJsonPretty(doc, file) == 0) {
        MSG_WARNLN("[WARNING]: Failed to write to /config/wificonfig.json file");
        status = 2;
    }
    file.close();
    return status;
}

int saveConfigMenu(AsyncWebServerRequest *request, String savemode)
{
    // --- Save menu
    char tmp[5];
    int status = 0;
    String fileNameRoot = String("menu");
    String fileName = String("/config/") + fileNameRoot;
    int menuNumber;
    File file;

    AsyncWebParameter *p = request->getParam("menunumberlist_save", true);
    String save_menu_number_str = p->value().c_str();

    int16_t numScanned = sscanf(save_menu_number_str.c_str(), "%d", &menuNumber);

    if ((numScanned == 1) && (menuNumber >= 0) && (menuNumber < NUM_PAGES)) {
        MSG_INFO1("[INFO] Saving Menu ", menuNumber);
        fileName += menuNumber;
        fileName += ".json";
        FILESYSTEM.remove(fileName.c_str());
        file = FILESYSTEM.open(fileName.c_str(), "w");
        if (!file) {
            MSG_WARN1("[WARNING]: Failed to create ", fileName.c_str());
            status = 1;
            return status;
        }
        else {
            MSG_INFO1("[INFO] Will write to: ", fileName.c_str());
        }
    }
    else {
        MSG_WARN2("[WARNING]: Invalid menu ", savemode.c_str(), save_menu_number_str.c_str());
        status = 3;
        return status;
    }

    DynamicJsonDocument doc(5000);

    JsonObject menu = doc.to<JsonObject>();

    String menuName;

    AsyncWebParameter *aSyncMenuName;
    try {
        aSyncMenuName = request->getParam("menuname", true);
        menuName = aSyncMenuName->value();
    }
    catch (...) {
        MSG_WARNLN("[WARNING] problem accessing data for menu name for save. Using default");
        menuName = fileNameRoot + menuNumber;
    }
    menu["menuname"] = menuName.c_str();

    for (uint8_t row = 0; row < BUTTON_ROWS; row++) {
        for (uint8_t col = 0; col < BUTTON_COLS; col++) {
            String buttonName = "button" + String(row + 1) + String(col + 1);
            MSG_INFO1("Starting button ", buttonName.c_str());

            try {
                JsonObject buttonObject = doc.createNestedObject(buttonName);

                AsyncWebParameter *buttonXXlogo = request->getParam(buttonName + "logo", true);
                if (buttonXXlogo != nullptr) {
                    if (strcmp(buttonXXlogo->value().c_str(), "---") == 0) {
                        buttonObject["logo"] = "question.bmp";
                    }
                    else {
                        buttonObject["logo"] = buttonXXlogo->value().c_str();
                    }
                }
                else {
                    buttonObject["logo"] = "question.bmp";
                }
                MSG_INFOLN("... finished logo");

                if (request->hasParam(buttonName + "latch", true)) {
                    buttonObject["latch"] = true;
                }
                else {
                    buttonObject["latch"] = false;
                }
                MSG_INFOLN("... finished latch");

                AsyncWebParameter *buttonXXlatchlogo = request->getParam(buttonName + "latchlogo", true);
                if (buttonXXlatchlogo != nullptr) {
                    if (strcmp(buttonXXlatchlogo->value().c_str(), "---") == 0) {
                        buttonObject["latchlogo"] = "";
                    }
                    else {
                        buttonObject["latchlogo"] = buttonXXlatchlogo->value().c_str();
                    }
                }
                else {
                    buttonObject["latchlogo"] = "";
                }
                MSG_INFOLN("... finished latchlogo");

                JsonArray buttonObject_actionarray = buttonObject.createNestedArray("actionarray");
                AsyncWebParameter *buttonXXaction0 = request->getParam(buttonName + "action0", true);
                AsyncWebParameter *buttonXXaction1 = request->getParam(buttonName + "action1", true);
                AsyncWebParameter *buttonXXaction2 = request->getParam(buttonName + "action2", true);

                if (buttonXXaction0 != nullptr && buttonXXaction1 != nullptr && buttonXXaction2 != nullptr) {
                    buttonObject_actionarray.add(buttonXXaction0->value().c_str());
                    buttonObject_actionarray.add(buttonXXaction1->value().c_str());
                    buttonObject_actionarray.add(buttonXXaction2->value().c_str());
                }
                else {
                    buttonObject_actionarray.add("0");
                    buttonObject_actionarray.add("0");
                    buttonObject_actionarray.add("0");
                }
                MSG_INFOLN("... finished actionarray");

                JsonArray buttonObject_valuearray = buttonObject.createNestedArray("valuearray");
                AsyncWebParameter *buttonXXvalue0 = request->getParam(buttonName + "value0", true);
                AsyncWebParameter *buttonXXvalue1 = request->getParam(buttonName + "value1", true);
                AsyncWebParameter *buttonXXvalue2 = request->getParam(buttonName + "value2", true);

                if (buttonXXvalue0 != nullptr && buttonXXvalue1 != nullptr && buttonXXvalue2 != nullptr) {
                    buttonObject_valuearray.add(buttonXXvalue0->value().c_str());
                    buttonObject_valuearray.add(buttonXXvalue1->value().c_str());
                    buttonObject_valuearray.add(buttonXXvalue2->value().c_str());
                }
                else {
                    buttonObject_valuearray.add("0");
                    buttonObject_valuearray.add("0");
                    buttonObject_valuearray.add("0");
                }

                MSG_INFOLN("... finished button");
            }
            catch (...) {
                MSG_WARN2("[WARNING] problem accessing data for button ", buttonName, " for save.");
            }
        }
    }
    if (serializeJsonPretty(doc, file) == 0) {
        MSG_WARNLN("[WARNING]: Failed to write to file");
        status = 2;
    }
#ifdef DUMP_JSON_DOC_ON_SAVE
    else {
        char buffer[5000];
        serializeJsonPretty(doc, buffer);
        MSG_INFOLN(buffer);
    }
#endif
    file.close();
    return status;
}