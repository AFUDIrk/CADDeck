/*
  Author: Dustin Watts
  Date: 27-08-2020

  My thanks goes out to Brian Lough, Colin Hickey, and the people on my Discord server
  for helping me a lot with the code and troubleshooting! https://discord.gg/RE3XevS

  FreeTouchDeck is based on the FreeDeck idea by Koriwi. It uses the TFT_eSPI library
  by Bodmer for the display and touch functionality and it uses an ESP32-BLE-Keyboard fork
  with a few modifications. For saving and loading configuration it uses ArduinoJson V6.

  FreeTouchDeck uses some libraries from other sources. These must be installed for
  FreeTouchDeck to compile and run.

  These are those libraries:

      !----------------------------- Library Dependencies --------------------------- !
      - Adafruit-GFX-Library (tested with version 1.10.4), available through Library Manager
      - TFT_eSPI (tested with version 2.4.70), available through Library Manager
      - ESPAsyncWebserver (latest version) download from: https://github.com/me-no-dev/ESPAsyncWebServer
      - AsyncTCP (latest version) download from: https://github.com/me-no-dev/AsyncTCP
      - ArduinoJson (tested with version 6.17.3), available through Library Manager
      ---One difference from the original FreeTouchDeck is that this program needs to send
        - both mouse and keyboard events. For this it uses the ESP32-BLE-Combo library
        - ESP32-BLE-Combo download from: https://github.com/BlynkGO/ESP32-BLE-Combo
        - However, the basic download is missing a number of key codes in BLEComboKeyboard.h.
        - I have added these key codes in the version of the library contained in the "lib"
        - folder of this project. If you want to use the original library, you will need to
        - correct the key codes yourself.


      --- If you use capacitive touch screen you will also need a library for that
      - The WT32-SC01 appears to be a FT6336, but works with both of the following libraries
      - Dustin Watts FT6236 Library (version 1.0.2), https://github.com/DustinWatts/FT6236
      - or the FT6336U library (https://github.com/aselectroworks)
      - You can select which library to use in the CADDeck.h file

  !-- Make sure you have setup your TFT display and ESP setup correctly in platformio.ini

        Select the right screen driver and the board (ESP32 is the only one tested) you are
        using. Also make sure TOUCH_CS is defined correctly. TFT_BL is also needed!

*/
#include <Arduino.h>

#include "CADDeck.h"

const char *versionnumber = "0.0.8WIP";

/*
 * Version 0.0.8     - In work
 *
 * Version 0.0.7     - Adapted for four button knob.
 *                   - Added support for AC3D as fifth CAD program
 *
 * Version 0.0.6     - Add top status bar with BT link status and active CAD program
 *
 * Version 0.0.5     - Integrated thumbwheel encoder. The encoder is connected directly to the ESP32 as there were problems with getting the
 *                     interrupts working properly with the encoder functions in the PCF857X library.
 *                   - Using "PCF857X" library for I2C IO expander. This came from the PCF8574 library, but has
 *                     been modified to support the PCF8575. The PCF8575 is a 16-bit IO expander, but the library
 *                     for the PCF8575 was well behind the PCF8574 library functionality. A #define statement in
 *                     the PCF857X library allows the library to be used for both the PCF8574 and PCF8575.
 *                   - Overall, working well. Next steps are:
 *                      - Highlight the selected CAD program
 *                      - Get the web configrator up to date with all the changes made to the code
 *
 *
 * Version 0.0.4     - Allow any menu to be displayed on startup. Previously, the default menu had to be menu0.json
 *                   - h/w button function definitions in cadparams.json
 *                   - CAD program selection in cadparams.json
 *                   - Joystick functionality for pan, rotate, zoom working
 *
 * Version 0.0.3     - Basic joystick functionality for pan, rotate, zoom
 *
 * Version 0.0.2     - Updated to use ESP32-BLE-Combo so that Mouse events can be sent as well as Keyboard events
 *
 * Version 0.0.1     - Starting from FreeTouchDeckWT32 V0.2.1-AF
 *
 * Version 0.2.0-AF  - Merge pull request that improves key spacing calculations by adding a margin around the screen.
 *
 * Version 0.1.12-AF - Use PSRAM to save logos and draw directly from PSRAM to screen. Overall, drawing a 4x3 keypad goes from about 2.2s to about 1.1s
 *                     the first time the menu is accessed down to about 0.2s all future times.
 *
 * Version 0.1.11-AF - Improved algorithm for calculating button size from key spacing rather than key spacing from button size
 *                   - Added espressif32 version requirement to the platform_packages callout (not needed on some platforms, but
 *                     this should make it more compatible across development platforms)
 *                   - Changed the WiFi connect code to show the SSID and Password that is being used to connect. The password display
 *                     is dependent on defining WIFI_PASSWORD_DISPLAY, and is disabled by default
 * Version 0.1.10-AF - Support for both WT32-SC01 and generic H/W configurations. Some sample configurations are provided.
 *                   - Don't draw inactive buttons. Inactive means the logo is blank.bmp and there are no actions defined.
 *
 * Version 0.1.9-AF  - Fix to variables in wificonfig.json. Defined TOUCH_CS to avoid warning.
 *
 * Version 0.1.8-AF  - Added support for FT6236 capacitive touch screen libraryOSO
 *
 * Version 0.1.7-AF  - Fix menu1.json for brightness down button code
 *
 * Version 0.1.6-AF  - A.Fernie 2022-09-09
 *    Updates to codes sent on com port for each button press
 *
 * Version 0.1.5-AF  - A.Fernie 2022-08-27
 *    Added USB Comms functionality (like FreeTeamsDeck)
 *
 * Version 0.1.4-AF  - A.Fernie 2022-08-19
 *    Added ability to locally save general config. Allows
 *    SleepEnable status to be preserved without going through web page. Later on allow
 *    additional params like sleeptimer, colors, etc.
 *
 * Version 0.1.3-AF  - A.Fernie 2022-08-08
 *  Added monitoring of the voltage of an external battery
 *
 * Version 0.1.2-AF  - A.Fernie 2022-08-04
 * 1. Load config.json file (wasn't being read, so always used defaults)
 * 2. Fix logic for button background color so that it is based on the action for that button being
 *    a menu change or not
 * 3. Fix VSCode.bmp background color (was not completely back)
 * 4. Added some "[INFO]" messages for colors and sleepenable/sleeptimer. Note that yuou need to enable
 *    [INFO] messages by setting "#define LOG_MSG_LEVEL 3  // 1=ERROR, 2=ERROR+WARN, 3=ERROR+WARN+INFO"
 *    in CADDeck.h to see them
 * 5. No colons after [INFO], [WARN], [ERROR]
 *
 *
 * Version 0.1.1-AF  - A.Fernie 2022-07-18
 * 1. Store sample 3x5 menus
 * 2. Switch to 3x4 (more readable).
 *
 * Version 0.1.0-AF  - A.Fernie 2022-7-15
 *
 * Started from 0.9.16 and added/modified:
 * 1. Now iterates through arrays of buttons rather than using repeated code for each button in
 *    most cases. Still two sections in index.htm that use repeated code.
 * 2. Support up to a 3x5 array of buttons (limited by index.htm)
 * 3. Home page is no longer a special page that just links to five other pages, and the other pages
 *    are no longer simply five action buttons plus a link back to the home page. All buttons on
 *    each page can be actions or links. Just make sure you always have a way back.
 * 4. There are now 10 pages. This includes Menu0 (home).
 * 5. Updated to latest platformio Arduino platform (2.x)
 * 6. Changed from SPIFFS to LittleFS. Note that if you check the serial monitor output you will see
 *    some error messages like "[vfs_api.cpp:104] open(): /littlefs/list does not exist". This is a
 *    known issue with ESPAsyncWebsServer related to LittleFS supporting folders while SPIFFS does not.
 *    see https://github.com/lorol/LITTLEFS/issues/2 for more on the topic.
 * 7. Currently only working on WT32-SC01 card http://www.wireless-tag.com/portfolio/wt32-sc01/
 *
 * Version 0.9.16.
 *
 * Added UserActions. In the UserAction.h file there are a few functions you can define and
 * select through the configurator. The functions have to written before compiling. These functions
 * are then hardcoded. Look at UserActions.h for some examples.
 *
 * Added some missing characters.
 */

#ifdef USECAPTOUCH
#ifdef USE_FT6336U_LIB
#ifdef CUSTOM_TOUCH_SDA
FT6336U ts = FT6336U(PIN_SDA, PIN_SCL);
#else
FT6336U ts = FT6336U();
#endif
#else
FT6236 ts = FT6236();
#endif
#endif

AsyncWebServer webserver(80);

TFT_eSPI tft = TFT_eSPI(SCREEN_HEIGHT, SCREEN_WIDTH);  // Apparently the display is a portrait mode, but we want landscape

Preferences savedStates;

// placeholder for the pagenumber we are on (0 indicates home)
int pageNum = 0;
Stack pageHistoryStack(10);

// Initial LED brightness
int ledBrightness = 255;

// path to the directory the logo are in ! including leading AND trailing / !
char logopath[64] = "/logos/";

// templogopath is used to hold the complete path of an image. It is empty for now.
char templogopath[64] = "";

// Create instances of the structs
Menu menu[NUM_PAGES];

Wificonfig wificonfig;

Config generalconfig;
CADConfig cadconfig;
CADProgramConfig cadprogramconfig[NUM_CAD_PROGRAMS];

Generallogos generallogo;

unsigned long previousMillis = 0;
unsigned long Interval = 0;
bool displayinginfo;
bool displayingIOValues;

char jsonFileFail[32] = "";

bool psramAvailable = false;

uint8_t last_hwbutton_state[NUM_HW_BUTTONS] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

// Invoke the TFT_eSPI button class and create all the button objects
TFT_eSPI_Button key[BUTTON_ROWS][BUTTON_COLS];

#ifdef READ_EXTERNAL_BATTERY
float readExternalBattery();
float externalBatteryVoltage = 0.0;
long lastADCRead = 0;
#endif

long last_loop_start = 0;
long loop_count = 0;
long this_loop_start = 0;
long loop_100_count = 0;
long loop_100_time = 0;

//-------------------------------- SETUP --------------------------------------------------------------

void setup()
{
    // Use serial port
    Serial.begin(115200);
    Serial.setDebugOutput(true);
    MSG_INFOLN("");

    MSG_INFOLN("[INFO] Loading saved brightness state");
    savedStates.begin("ftd", false);

    ledBrightness = savedStates.getInt("ledBrightness", 255);
    MSG_INFOLN("[INFO] Brightness has been set");

    psramAvailable = psramFound();

#ifdef USECAPTOUCH
#ifdef CUSTOM_TOUCH_SDA
#ifdef USE_FT6336U_LIB
    if (!ts.begin())
#else
    if (!ts.begin(40, CUSTOM_TOUCH_SDA, CUSTOM_TOUCH_SCL))
#endif
#else
#ifdef USE_FT6336U_LIB
    if (!ts.begin())
#else
    if (!ts.begin(40))
#endif
#endif  // defined(CUSTOM_TOUCH_SDA)

        if (!ts.begin()) {
            MSG_WARNLN("[WARNING]: Unable to start the capacitive touchscreen.");
        }
        else {
            MSG_INFOLN("[INFO] Capacitive touch started!");
        }
#endif

    // Setup PWM channel and attach pin 32
    ledcSetup(0, 5000, 8);
#ifdef TFT_BL
    ledcAttachPin(TFT_BL, 0);
#else
    ledcAttachPin(32, 0);
#endif
    ledcWrite(0, ledBrightness);  // Start @ initial Brightness

    // --------------- Init Display -------------------------

    // Initialise the TFT screen
    tft.init();

    // Set the rotation before we calibrate
    tft.setRotation(1);

    // Clear the screen
    tft.fillScreen(TFT_BLACK);

    esp_sleep_wakeup_cause_t wakeup_reason;
    wakeup_reason = esp_sleep_get_wakeup_cause();

    // -------------- Start filesystem ----------------------

    if (!FILESYSTEM.begin()) {
        MSG_ERRORLN("[ERROR] LittleFS initialisation failed!");
        drawErrorMessage("Failed to init LittleFS! Did you upload the data folder?");
        while (1)
            yield();  // We stop here
    }
    MSG_INFOLN("[INFO] LittleFS initialised.");

    // Check for free space

    MSG_INFO("[INFO] Free Space: ");
    MSG_INFOLN(FILESYSTEM.totalBytes() - FILESYSTEM.usedBytes());

    //------------------ Load Wifi Config ----------------------------------------------

    MSG_INFOLN("[INFO] Loading Wifi Config");
    if (!loadMainConfig()) {
        MSG_WARNLN("[WARNING]: Failed to load WiFi Credentials!");
    }
    else {
        MSG_INFOLN("[INFO] WiFi Credentials Loaded");
    }

    // ----------------- Load webserver ---------------------

    MSG_INFOLN("[INFO] Handler setup");
    handlerSetup();

    // ------------------- Splash screen ------------------

    MSG_INFOLN("[INFO] Splash screen");
    // If we are woken up we do not need the splash screen
    if (wakeup_reason > 0) {
        // But we do draw something to indicate we are waking up
        tft.setTextFont(2);
        tft.println(" Waking up...");
    }
    else {
        // Draw a splash screen
        if (checkfile("/logos/CADDeck_logo.bmp", false)) {
            drawBmp("/logos/CADDeck_logo.bmp", 0, 0, false);
        }
        else {
            MSG_INFOLN("[INFO] No \"/logos/CADDeck_logo.bmp\" file found for splash screen.");
        }
        tft.setCursor(1, 3);
        tft.setTextFont(2);
        tft.setTextSize(1);
        tft.setTextColor(TFT_WHITE, TFT_BLACK);
        tft.printf("Loading version %s\n", versionnumber);
        MSG_INFO1F("[INFO] Loading version %s\n", versionnumber);
    }

// Calibrate the touch screen and retrieve the scaling factors
#ifndef USECAPTOUCH
    MSG_INFOLN("[INFO] Calibrate touch screen");
    touch_calibrate();
#endif

#ifdef READ_EXTERNAL_BATTERY
    // External battery read voltage setup
    MSG_INFOLN("[INFO] Battery monitor setup");
    pinMode(EXTERNAL_BATTERY_PIN, INPUT);
    analogSetPinAttenuation(EXTERNAL_BATTERY_PIN, ADC_11db);
    adcAttachPin(EXTERNAL_BATTERY_PIN);
#endif
    MSG_INFOLN("[INFO] Check for general.json");
    // Let's first check if all the files we need exist
    if (!checkfile("/config/general.json", true)) {
        MSG_ERRORLN("[ERROR] /config/general.json not found!");
        tft.printf("[ERROR] /config/general.json not found!\n");
        while (1)
            yield();  // Stop!
    }

    for (size_t i = 0; i < NUM_PAGES; i++) {
        char filename[32];
        char filenumber[4];
        strcpy(filename, "/config/menu");
        sprintf(filenumber, "%d", i);
        strcat(filename, filenumber);
        strcat(filename, ".json");
        MSG_INFO1("[INFO] Check for ", filename);

        if (!checkfile(filename, false)) {
            MSG_WARN2("[WARNING]: ", filename, " not found. Initializing to default.json");
            tft.printf("[WARNING]: %s not found. Initializing to default.json\n", filename);

            if (CopyFile("/config/default.json", filename)) {
                MSG_INFO2("[WARN] Successful initialization of ", filename, " to default.json");
            }
            else {
                MSG_ERROR1("[ERROR] Failed to initialize ", filename);
                checkfile(filename, true);  // This will force a message to the TFT screen

                while (1)
                    yield();  // Stop!
            }
        }
    }

    // Setup PWM channel for Piezo speaker

#ifdef speakerPin
    ledcSetup(2, 500, 8);

    if (generalconfig.beep) {
        ledcAttachPin(speakerPin, 2);
        ledcWriteTone(2, 600);
        delay(150);
        ledcDetachPin(speakerPin);
        ledcWrite(2, 0);

        ledcAttachPin(speakerPin, 2);
        ledcWriteTone(2, 800);
        delay(150);
        ledcDetachPin(speakerPin);
        ledcWrite(2, 0);

        ledcAttachPin(speakerPin, 2);
        ledcWriteTone(2, 1200);
        delay(150);
        ledcDetachPin(speakerPin);
        ledcWrite(2, 0);
    }

#endif

    // Now load the config files, starting with general.json
    if (!loadConfig("general")) {
        MSG_WARNLN("[WARNING]: general.json failed to load!");
        MSG_WARNLN("[WARNING]: To reset to default type \'reset general\'");
        pageNum = SPECIAL_4_PAGE;
        pageHistoryStack.push(pageNum);
    }
    else if (!loadConfig("cadparams")) {
        MSG_WARNLN("[WARNING]: cadparams.json failed to load!");
        MSG_WARNLN("[WARNING]: To reset to default type \'reset cadparams\'");
        pageNum = SPECIAL_4_PAGE;
        pageHistoryStack.push(pageNum);
    }
    else {
        // Menu to display on startup
        pageNum = constrain(generalconfig.startup_menu, 0, NUM_PAGES - 1);
        pageHistoryStack.push(pageNum);

#ifdef TOUCH_INTERRUPT_PIN
        Interval = generalconfig.sleeptimer * MIN_TO_MS;
        if (generalconfig.sleepenable) {
            pinMode(TOUCH_INTERRUPT_PIN, INPUT_PULLUP);
            MSG_INFOLN("[INFO] Sleep enabled.");
            MSG_INFO("[INFO] Sleep timer = ");
            MSG_INFO(generalconfig.sleeptimer);
            MSG_INFOLN(" minutes");
        }
        else {
            MSG_INFOLN("[INFO] Sleep not enabled.");
            MSG_INFO("[INFO] Sleep timer = ");
            MSG_INFO(generalconfig.sleeptimer);
            MSG_INFOLN(" minutes");
        }
#endif
    }

    // Next, load the menu pages
    for (size_t i = 0; i < NUM_PAGES; i++) {
        char menuName[16];
        char menuNumber[4];
        char menuNumberZeroIndex[4];
        strcpy(menuName, "menu");
        sprintf(menuNumber, "%d", i);
        strcat(menuName, menuNumber);

        if (!loadConfig(menuName)) {
            Serial.print("[WARNING]: menu");
            sprintf(menuNumberZeroIndex, "%d", i);
            Serial.print(menuNumberZeroIndex);
            MSG_INFOLN(".json seems to be corrupted!");
            Serial.print("[WARNING]: To reset to default type 'reset ");
            Serial.print(menuName);
            MSG_INFOLN("'.");
            strcpy(jsonFileFail, menuName);
            pageNum = SPECIAL_4_PAGE;
            pageHistoryStack.push(pageNum);
        }
    }

    MSG_INFOLN("[INFO] All configs loaded");

    // Preload the logos into PSRAM

#ifdef PRELOAD_LOGOS
    if (psramAvailable) {
        tft.printf("[INFO] PSRAM available\n");
        MSG_BASICLN("[INFO] PSRAM available");
        tft.printf("[INFO] Preloading logos. Please wait...\n");
        MSG_BASICLN("[INFO] Preloading logos. Please wait...");
        uint16_t imageBGColor;
        uint8_t status;
        for (uint8_t page = 0; page < NUM_PAGES; page++) {
            for (uint8_t row = 0; row < BUTTON_ROWS; row++) {
                for (uint8_t col = 0; col < BUTTON_COLS; col++) {
                    // Load the logo into PSRAM
                    status = loadBmpToPSRAM(menu[page].button[row][col].logo, &(menu[page].button[row][col].pImage));

                    if (status == ReturnSuccess) {
                        imageBGColor = menu[page].button[row][col].pImage[2];
                        menu[page].button[row][col].imageBGColour = imageBGColor;
                        menu[page].button[row][col].imageBGColourValid = true;
                    }
                    else {
                        menu[page].button[row][col].imageBGColourValid = false;
                        menu[page].button[row][col].pImage = nullptr;
                        tft.printf("Error allocating PSRAM for logo: %s\n", menu[page].button[row][col].logo);
                        MSG_ERROR1("Error allocating PSRAM for logo: ", menu[page].button[row][col].logo);
                    }

                    // Load the latchLogo into PSRAM
                    if (menu[page].button[row][col].latch) {
                        status = loadBmpToPSRAM(menu[page].button[row][col].latchlogo, &(menu[page].button[row][col].pLatchImage));
                        if (status == ReturnSuccess) {
                            imageBGColor = menu[page].button[row][col].pLatchImage[2];
                            menu[page].button[row][col].latchImageBGColour = imageBGColor;
                            menu[page].button[row][col].latchImageBGColourValid = true;
                        }
                        else {
                            menu[page].button[row][col].latchImageBGColourValid = false;
                            menu[page].button[row][col].pLatchImage = nullptr;
                            tft.printf("Error allocating PSRAM for latch logo: %s\n", menu[page].button[row][col].latchlogo);
                            MSG_ERROR1("Error allocating PSRAM for latch logo: ", menu[page].button[row][col].latchlogo);
                        }
                    }
                }
            }
        }
        tft.printf("[INFO] Finished preloading logos\n");
        MSG_BASICLN("[INFO] Finished preloading logos");
    }
    else {
        tft.printf("[INFO] PSRAM not available\n");
        MSG_BASICLN("[INFO] PSRAM not available");
    }
#endif

    // -------------- User IO Setup -------------------------------
    MSG_INFOLN("[INFO] Init Physical IO");
    init_io();

    // Setup the Font used for plain text
    tft.setFreeFont(LABEL_FONT);

    //------------------BLE Initialization ------------------------
    MSG_INFOLN("[INFO] Starting BLE");
    // bleKeyboard.begin();
    // bleMouse.begin();
    Keyboard.begin();
    Mouse.begin();

    // ---------------- Printing version numbers ------------------
    // MSG_INFO("[INFO] BLE Keyboard version: ");
    // MSG_INFOLN(BLE_COMBO_VERSION);
    MSG_INFO("[INFO] ArduinoJson version: ");
    MSG_INFOLN(ARDUINOJSON_VERSION);
    MSG_INFO("[INFO] TFT_eSPI version: ");
    MSG_INFOLN(TFT_ESPI_VERSION);

    // ---------------- Start the first keypad -------------

    // Draw background
    tft.fillScreen(generalconfig.backgroundColour);

    // Draw keypad
    MSG_INFOLN("[INFO] Drawing keypad");

    pageHistoryStack.push(pageNum);
    drawKeypad();
}

//--------------------- LOOP ---------------------------------------------------------------------

void loop(void)
{
    bool pressed = false;
    uint16_t t_x = 0, t_y = 0;

    if (millis() > (last_loop_start + loop_period)) {
        this_loop_start = millis();
        loop_count++;

        // Update andf handle the joystick and physical buttons
        update_io();

        // Check and handle any commands from the serial port
        serial_commands();

        // Handle the touch screen

#ifdef USECAPTOUCH
#ifdef USE_FT6336U_LIB
        FT6336U_TouchPointType touchPos;
        touchPos = ts.scan();

        if (touchPos.tp[0].tapped) {
            MSG_TOUCH_DEBUG(" Tap Detected: ");
            MSG_TOUCH_DEBUGLN(touchPos.touch_count);
            MSG_TOUCH_DEBUG(" pageNum: ");
            MSG_TOUCH_DEBUGLN(pageNum);
            MSG_TOUCH_DEBUG(" x,y: ");
            MSG_TOUCH_DEBUG(touchPos.tp[0].x);
            MSG_TOUCH_DEBUG(" : ");
            MSG_TOUCH_DEBUGLN(touchPos.tp[0].y);

            // Flip things around so it matches our screen rotation
            //         p.x = map(p.x, 0, 320, 320, 0);

            t_x = touchPos.tp[0].y;
            t_y = 320 - touchPos.tp[0].x;
            pressed = true;
        }
        else {
            pressed = false;
        }
#else
        if (ts.touched()) {
            // Retrieve a point
            TS_Point p = ts.getPoint();

            // Flip things around so it matches our screen rotation
            p.x = map(p.x, 0, 320, 320, 0);
            t_y = p.x;
            t_x = p.y;

            pressed = true;
        }
        else {
            pressed = false;
        }
#endif
#else
        pressed = tft.getTouch(&t_x, &t_y);
#endif

        if (pageNum == WEB_REQUEST_PAGE) {
            // If the pageNum is set to NUM_PAGES+1, do not draw anything on screen or check for touch
            // and start handling incomming web requests.
        }
        else if (pageNum == SPECIAL_PAGE_INFO) {
            if (!displayinginfo) {
                printinfo();
            }

            if (pressed) {
                displayinginfo = false;
                pageHistoryStack.pop();
                pageNum = pageHistoryStack.peek();

                tft.fillScreen(generalconfig.backgroundColour);
                drawKeypad();
            }
        }
        else if (pageNum == SPECIAL_PAGE_IO_MONITOR) {
            printIOValues();

            if (pressed) {
                displayingIOValues = false;
                pageHistoryStack.pop();
                pageNum = pageHistoryStack.peek();
                tft.fillScreen(generalconfig.backgroundColour);
                drawKeypad();
            }
        }
        else if (pageNum == SPECIAL_3_PAGE) {
            // We were unable to connect to WiFi. Waiting for touch to get back to the settings menu.

            if (pressed) {
                // Return to Settings page
                displayinginfo = false;
                pageHistoryStack.pop();
                pageNum = pageHistoryStack.peek();
                tft.fillScreen(generalconfig.backgroundColour);
                drawKeypad();
            }
        }
        else if (pageNum == SPECIAL_4_PAGE) {
            // A JSON file failed to load. We are drawing an error message. And waiting for a touch.

            if (pressed) {
                // Load home screen
                displayinginfo = false;
                pageHistoryStack.pop();
                pageNum = pageHistoryStack.peek();
                tft.fillScreen(generalconfig.backgroundColour);
                drawKeypad();
            }
        }
        else {
            // Check if sleep is enabled and if our timer has ended.

#ifdef TOUCH_INTERRUPT_PIN
            if (generalconfig.sleepenable) {
                if (millis() > previousMillis + Interval) {
                    // The timer has ended and we are going to sleep  .
                    tft.fillScreen(TFT_BLACK);
                    MSG_INFOLN("[INFO] Going to sleep.");
#ifdef speakerPin
                    if (generalconfig.beep) {
                        ledcAttachPin(speakerPin, 2);
                        ledcWriteTone(2, 1200);
                        delay(150);
                        ledcDetachPin(speakerPin);
                        ledcWrite(2, 0);

                        ledcAttachPin(speakerPin, 2);
                        ledcWriteTone(2, 800);
                        delay(150);
                        ledcDetachPin(speakerPin);
                        ledcWrite(2, 0);

                        ledcAttachPin(speakerPin, 2);
                        ledcWriteTone(2, 600);
                        delay(150);
                        ledcDetachPin(speakerPin);
                        ledcWrite(2, 0);
                    }
#endif
                    MSG_INFOLN("[INFO] Saving latched states");

                    esp_sleep_enable_ext0_wakeup(TOUCH_INTERRUPT_PIN, 0);
                    esp_deep_sleep_start();
                }
            }
#endif

            // // Touch coordinates are stored here

            // Check if the X and Y coordinates of the touch are within one of our buttons
            for (uint8_t i = 0; i < BUTTON_ROWS; i++) {
                for (uint8_t j = 0; j < BUTTON_COLS; j++) {
                    if (pressed && key[i][j].contains(t_x, t_y)) {
                        key[i][j].press(true);  // tell the button it is pressed

                        // After receiving a valid touch reset the sleep timer
                        previousMillis = millis();
                    }
                    else {
                        key[i][j].press(false);  // tell the button it is NOT pressed
                    }
                }
            }
            // Check if any key has changed state
            for (uint8_t row = 0; row < BUTTON_ROWS; row++) {
                for (uint8_t col = 0; col < BUTTON_COLS; col++) {
                    if (key[row][col].justReleased()) {
                        // Draw normal button space (non inverted)
                        drawButtonRowCol(pageNum, row, col);
                    }

                    if (key[row][col].justPressed()) {
// Beep
#ifdef speakerPin
                        if (generalconfig.beep) {
                            ledcAttachPin(speakerPin, 2);
                            ledcWriteTone(2, 600);
                            delay(50);
                            ledcDetachPin(speakerPin);
                            ledcWrite(2, 0);
                        }
#endif

                        bool activeButton = isActiveButton(pageNum, row, col);

                        tft.setFreeFont(LABEL_FONT);
                        if (activeButton) {
                            // Draw inverted button space
                            key[row][col].initButton(&tft, KEY_X + col * (KEY_W + KEY_SPACING_X),
                                                     KEY_Y + row * (KEY_H + KEY_SPACING_Y),  // x, y, w, h, outline, fill, text
                                                     KEY_W, KEY_H, TFT_WHITE, TFT_WHITE, TFT_WHITE,
                                                     (char *)"", KEY_TEXTSIZE);
                        }
                        else {
                            // Draw black button if inactive
                            key[row][col].initButton(&tft, KEY_X + col * (KEY_W + KEY_SPACING_X),
                                                     KEY_Y + row * (KEY_H + KEY_SPACING_Y),  // x, y, w, h, outline, fill, text
                                                     KEY_W, KEY_H, TFT_BLACK, TFT_BLACK, TFT_BLACK,
                                                     (char *)"", KEY_TEXTSIZE);
                        }
                        key[row][col].drawButton();
                        //---------------------------------------- Button press handling --------------------------------------------------

                        if ((pageNum >= 0) && (pageNum < NUM_PAGES)) {
                            if (row < BUTTON_ROWS && col < BUTTON_COLS) {
                                KeyboardMouseAction(menu[pageNum].button[row][col].actions[0].action, menu[pageNum].button[row][col].actions[0].value, menu[pageNum].button[row][col].actions[0].symbol);
                                KeyboardMouseAction(menu[pageNum].button[row][col].actions[1].action, menu[pageNum].button[row][col].actions[1].value, menu[pageNum].button[row][col].actions[1].symbol);
                                KeyboardMouseAction(menu[pageNum].button[row][col].actions[2].action, menu[pageNum].button[row][col].actions[2].value, menu[pageNum].button[row][col].actions[2].symbol);
                                Keyboard.releaseAll();
                                if (menu[pageNum].button[row][col].latch) {
                                    if (menu[pageNum].button[row][col].islatched) {
                                        menu[pageNum].button[row][col].islatched = false;
                                    }
                                    else {
                                        menu[pageNum].button[row][col].islatched = true;
                                    }
                                }

                                if (generalconfig.usbcommsenable) {
                                    // separate filename from menu[pageNum].button[row][col].logo path
                                    char logoname[20];
                                    char *p = strrchr(menu[pageNum].button[row][col].logo, '/');
                                    if (p != NULL) {
                                        strcpy(logoname, p + 1);
                                    }
                                    else {
                                        strcpy(logoname, menu[pageNum].button[row][col].logo);
                                    }

                                    // remove extension from logoname
                                    char *dot = strrchr(logoname, '.');
                                    if (dot != NULL) {
                                        *dot = '\0';
                                    }
                                    char usbData[40];
                                    snprintf(usbData, sizeof(usbData), "{ButtonPress, %s , %s}", menu[pageNum].name, logoname);
                                    Serial.println(usbData);
                                }
                            }
                            else  // Back home
                            {
                                pageNum = 0;
                                drawKeypad();
                            }
                        }
#ifdef READ_BATTERY_VOLTAGE
                        MSG_INFO1("Battery voltage:", externalBatteryVoltage);
#endif
                    }
                    //                    delay(10);  // UI debouncing
                }
            }

            // Check if any hw button has been pressed
            // if(loop_count % 10 == 0) MSG_DEBUG("Checking HW buttons:");
            for (uint8_t i = 0; i < cadprogramconfig[cadconfig.current_program].num_buttons; i++) {
                uint8_t button_state = get_hwbutton(i);
                // if (loop_count % 10 == 0) MSG_DEBUG2("hw:",i, button_state);

                if (button_state && (last_hwbutton_state[i] == 0)) {
                    //---------------------------------------- Button press handling --------------------------------------------------

                    KeyboardMouseAction(cadprogramconfig[cadconfig.current_program].hw_buttons[i][0].action,
                                        cadprogramconfig[cadconfig.current_program].hw_buttons[i][0].value,
                                        cadprogramconfig[cadconfig.current_program].hw_buttons[i][0].symbol, i);

                    KeyboardMouseAction(cadprogramconfig[cadconfig.current_program].hw_buttons[i][1].action,
                                        cadprogramconfig[cadconfig.current_program].hw_buttons[i][1].value,
                                        cadprogramconfig[cadconfig.current_program].hw_buttons[i][1].symbol, i);

                    KeyboardMouseAction(cadprogramconfig[cadconfig.current_program].hw_buttons[i][2].action,
                                        cadprogramconfig[cadconfig.current_program].hw_buttons[i][2].value,
                                        cadprogramconfig[cadconfig.current_program].hw_buttons[i][2].symbol, i);
                }
                last_hwbutton_state[i] = button_state;
            }
            // if (loop_count % 10 == 0) MSG_DEBUGLN("");
        }

        // Draw top status bar.
        drawTopStatusBar(false);

#ifdef READ_EXTERNAL_BATTERY
        if ((millis() - lastADCRead) > 100) {
            float newVoltage = readExternalBattery();

            externalBatteryVoltage = externalBatteryVoltage + 0.1 * (newVoltage - externalBatteryVoltage);
            lastADCRead = millis();
        }
#endif
        loop_100_time = loop_100_time + (millis() - this_loop_start);
        loop_100_count++;
        if (loop_100_count > 100) {
            //           MSG_DEBUG1("[DEBUG] Time for 100 loops:", loop_100_time);
            loop_100_count = 0;

            loop_100_time = 0;
        }
        last_loop_start = this_loop_start;
    }
}

#ifdef READ_EXTERNAL_BATTERY
float readExternalBattery()
{
    uint16_t voltage = analogRead(EXTERNAL_BATTERY_PIN);
    float scaledVoltage = (float)voltage * EXTERNAL_BATTERY_SCALE;
    return scaledVoltage;
}
#endif
