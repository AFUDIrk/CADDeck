// Start as WiFi station
#include "ConfigHelper.h"

bool startWifiStation()
{
#ifdef LANGUAGE_GERMAN
    MSG_INFO1("startWifiStation(): Verbinden mit", wificonfig.ssid);
#else
    MSG_INFO1("startWifiStation(): Connecting to", wificonfig.ssid);
#endif

    if (String(WiFi.SSID()) != String(wificonfig.ssid)) {
        WiFi.mode(WIFI_STA);
#ifdef LANGUAGE_GERMAN
        MSG_INFO2("startWifiStation(): ssid, pwd=", wificonfig.ssid, wificonfig.password);
#else
        MSG_INFO2("startWifiStation(): ssid, pwd=", wificonfig.ssid, wificonfig.password);
#endif
        WiFi.begin(wificonfig.ssid, wificonfig.password);
        uint8_t attempts = wificonfig.attempts;
        while (WiFi.status() != WL_CONNECTED) {
            if (attempts == 0) {
                WiFi.disconnect();
                MSG_INFOLN("");
                return false;
            }
            delay(wificonfig.attemptdelay);
            MSG_INFO(".");
            attempts--;
        }
    }

    // Delete the task Keyboard had create to free memory and to not interfere with AsyncWebServer
    // Keyboard.end();

    // Stop BLE from interfering with our WIFI signal
    btStop();
    esp_bt_controller_disable();
    esp_bt_controller_deinit();
    esp_bt_controller_mem_release(ESP_BT_MODE_BTDM);

    MSG_INFOLN("");

#ifdef LANGUAGE_GERMAN
    MSG_INFOLN("[INFO] BLE gestoppt");
    MSG_INFO("[INFO] Verbunden! IP Adresse: ");
#else
    MSG_INFOLN("[INFO] BLE Stopped");
    MSG_INFO("[INFO] Connected! IP address: ");
#endif

    MSG_INFOLN(WiFi.localIP());

    MDNS.begin(wificonfig.hostname);
    MDNS.addService("http", "tcp", 80);

    // Set pageNum to 7 so no buttons are displayed and touches are ignored
    uint8_t status = pageHistoryStack.push(pageNum);
    if (status == STACK_STATUS_FULL) {
        MSG_INFOLN("[INFO] Page History Stack is full. Dropped oldest value..");
    }
    pageNum = WEB_REQUEST_PAGE;

    // Start the webserver
    webserver.begin();

#ifdef LANGUAGE_GERMAN
    MSG_INFOLN("[INFO] Webserver gestartet");
#else
    MSG_INFOLN("[INFO] Webserver started");
#endif
    return true;
}

// Start as WiFi AP

void startWifiAP()
{
    WiFi.mode(WIFI_AP);
    WiFi.softAP(wificonfig.ssid, wificonfig.password);
    MSG_INFOLN("");

#ifdef LANGUAGE_GERMAN
    MSG_INFO("[INFO] Zugangspunkt gestartet! IP Adresse: ");
#else
    MSG_INFO("[INFO] Access Point Started! IP address: ");
#endif
    MSG_INFOLN(WiFi.softAPIP());

    // Delete the task Keyboard had create to free memory and to not interfere with AsyncWebServer
    // Keyboard.end();

    // Stop BLE from interfering with our WIFI signal
    btStop();
    esp_bt_controller_disable();
    esp_bt_controller_deinit();
    esp_bt_controller_mem_release(ESP_BT_MODE_BTDM);

    MSG_INFOLN("");

#ifdef LANGUAGE_GERMAN
    MSG_INFOLN("[INFO] BLE gestoppt");
#else
    MSG_INFOLN("[INFO] BLE Stopped");
#endif

    MDNS.begin(wificonfig.hostname);
    MDNS.addService("http", "tcp", 80);

    // Set pageNum to WEB_REQUEST_PAGE so no buttons are displayed and touches are ignored
    uint8_t status = pageHistoryStack.push(pageNum);
    if (status == STACK_STATUS_FULL) {
        MSG_INFOLN("[INFO] Page History Stack is full. Dropped oldest value..");
    }
    pageNum = WEB_REQUEST_PAGE;

    // Start the webserver
    webserver.begin();

#ifdef LANGUAGE_GERMAN
    MSG_INFOLN("[INFO] Webserver gestartet");
#else
    MSG_INFOLN("[INFO] Webserver started");
#endif
}

// Start the default AP

void startDefaultAP()
{
    const char* ssid = "CADDeck";
    const char* password = "defaultpass";

    WiFi.mode(WIFI_AP);
    WiFi.softAP(ssid, password);

#ifdef LANGUAGE_GERMAN
    MSG_INFO("[INFO] Zugangspunkt gestartet! IP Adresse: ");
#else
    MSG_INFO("[INFO] Access Point Started! IP address: ");
#endif
    MSG_INFOLN(WiFi.softAPIP());

    // Delete the task Keyboard had create to free memory and to not interfere with AsyncWebServer
    // leKeyboard.end();

    // Stop BLE from interfering with our WIFI signal
    btStop();
    esp_bt_controller_disable();
    esp_bt_controller_deinit();
    esp_bt_controller_mem_release(ESP_BT_MODE_BTDM);

#ifdef LANGUAGE_GERMAN
    MSG_INFOLN("[INFO] BLE gestoppt");
#else
    MSG_INFOLN("[INFO] BLE Stopped");
#endif

    MDNS.begin("CADDeck");
    MDNS.addService("http", "tcp", 80);

    // Set pageNum to WEB_REQUEST_PAGE so no buttons are displayed and touches are ignored
    uint8_t status = pageHistoryStack.push(pageNum);
    if (status == STACK_STATUS_FULL) {
        MSG_INFOLN("[INFO] Page History Stack is full. Dropped oldest value..");
    }
    pageNum = WEB_REQUEST_PAGE;

    // Start the webserver
    webserver.begin();

#ifdef LANGUAGE_GERMAN
    MSG_INFOLN("[INFO] Webserver gestartet");
#else
    MSG_INFOLN("[INFO] Webserver started");
#endif
}

/**
* @brief This function stops Bluetooth and connects to the given
         WiFi network. It the starts mDNS and starts the Async
         Webserver.
*
* @param none
*
* @return none
*
* @note none
*/
void configmode()
{
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(0, 0);
    tft.setTextFont(2);
    tft.setTextSize(1);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);

#ifdef LANGUAGE_GERMAN
    MSG_INFOLN("[INFO] Konfigurationsmodus aufrufen");
    tft.println("Verbindung mit WLAN...");
    tft.print("  SSID: ");
    tft.println(wificonfig.ssid);
    tft.print("  PWD: ");
#ifdef WIFI_PASSWORD_DISPLAY
    tft.println(wificonfig.password);
#else
    tft.println("********");
#endif
    tft.print("  Konfigurationsmodus: ");
    tft.println(wificonfig.wifimode);
    tft.println();

    if (String(wificonfig.ssid) == "DEINE_WIFI_SSID" || String(wificonfig.password) == "DEIN_WIFI_PASSWORD")  // Still default
    {
        tft.println("WiFi-Konfiguration immer noch auf Standard eingestellt! Starte als AP.");
        MSG_WARNLN("[WARNUNG]: Die WLAN-Konfiguration ist immer noch auf die Standardeinstellung eingestellt! Konfigurator gestartet als AP.");
        startDefaultAP();
        tft.println("Als AP gestartet, da die WLAN-Einstellungen immer noch auf die Standardeinstellungen eingestellt sind.");
        tft.println("");
        tft.println("Stellen Sie zum Konfigurieren eine Verbindung ");
        tft.println("zu �CADDeck� mit dem Passwort �defaultpass� her");
        tft.println("Dann geh zu http://CADDeck.local");
        tft.print("Die IP ist: ");
        tft.println(WiFi.softAPIP());
        return;
    }
    else if (String(wificonfig.ssid) == "FEHLGESCHLAGEN" || String(wificonfig.password) == "FEHLGESCHLAGEN" || String(wificonfig.wifimode) == "FEHLGESCHLAGEN")  // The wificonfig.json failed to load
    {
        tft.println("WLAN-Konfiguration konnte nicht geladen werden! Starte als AP.");
        MSG_WARNLN("[WARNUNG]: WLAN-Konfiguration konnte nicht geladen werden! Konfigurator gestartet als AP.");
        startDefaultAP();
        tft.println("Als AP gestartet, da die WLAN-Einstellungen nicht geladen werden konnten.");
        tft.println("");
        tft.println("Stellen Sie zum Konfigurieren eine Verbindung ");
        tft.println("zu �CADDeck� mit dem Passwort �defaultpass� her");
        tft.println("Dann geh zu http://CADDeck.local");
        tft.print("Die IP ist: ");
        tft.println(WiFi.softAPIP());
        return;
    }
    else if (strcmp(wificonfig.wifimode, "WIFI_STA") == 0) {
        if (!startWifiStation()) {
            startDefaultAP();
            MSG_WARNLN("[WARNUNG]: Verbindung zum AP konnte nicht hergestellt werden, daher als AP gestartet.");
            tft.println("Als AP gestartet, da die WLAN-Verbindung fehlgeschlagen ist.");
            tft.println("");
            tft.println("Stellen Sie zum Konfigurieren eine Verbindung ");
            tft.println("zu �CADDeck� mit dem Passwort �defaultpass� her");
            tft.println("Dann geh zu http://CADDeck.local");
            tft.print("Die IP ist: ");
            tft.println(WiFi.softAPIP());
        }
        else {
            MSG_WARNLN("[WARNUNG]: Als STA und im Konfigurationsmodus gestartet.");
            tft.println("Als STA und im Konfigurationsmodus gestartet.");
            tft.println("");
            tft.println("Zur Konfiguration:");
            tft.println("http://CADDeck.local");
            tft.print("Die IP ist: ");
            tft.println(WiFi.localIP());
        }
    }
    else if (strcmp(wificonfig.wifimode, "WIFI_AP") == 0) {
        MSG_WARNLN("[WARNUNG]: Als AP und im Konfigurationsmodus gestartet.");
        startWifiAP();
        tft.println("Als AP und im Konfigurationsmodus gestartet.");
        tft.println("");
        tft.println("Zur Konfiguration:");
        tft.println("http://CADDeck.local");
        tft.print("Die IP ist: ");
        tft.println(WiFi.softAPIP());
    }
    else {
        MSG_WARNLN("[WARNUNG]: Kein g�ltiger Konfigurationsmodus identifiziert.");
    }
#else
    MSG_INFOLN("[INFO] Entering Config Mode");

    tft.println("Connecting to Wifi...");
    tft.print("  SSID: ");
    tft.println(wificonfig.ssid);
    tft.print("  PWD: ");
#ifdef WIFI_PASSWORD_DISPLAY
    tft.println(wificonfig.password);
#else
    tft.println("********");
#endif
    tft.print("  Config mode: ");
    tft.println(wificonfig.wifimode);
    tft.println();

    if (String(wificonfig.ssid) == "YOUR_WIFI_SSID" || String(wificonfig.password) == "YOUR_WIFI_PASSWORD")  // Still default
    {
        tft.println("WiFi Config still set to default! Starting as AP.");
        MSG_WARNLN("[WARNING]: WiFi Config still set to default! Configurator started as AP.");
        startDefaultAP();
        tft.println("Started as AP because WiFi settings are still set to default.");
        tft.println("");
        tft.println("To configure, connect to 'CADDeck' with password 'defaultpass'");
        tft.println("Then go to http://CADDeck.local");
        tft.print("The IP is: ");
        tft.println(WiFi.softAPIP());
        return;
    }
    else if (String(wificonfig.ssid) == "FAILED" || String(wificonfig.password) == "FAILED" || String(wificonfig.wifimode) == "FAILED")  // The wificonfig.json failed to load
    {
        tft.println("WiFi Config Failed to load! Starting as AP.");
        MSG_WARNLN("[WARNING]: WiFi Config Failed to load! Configurator started as AP.");
        startDefaultAP();
        tft.println("Started as AP because WiFi settings failed to load.");
        tft.println("");
        tft.println("To configure, connect to 'CADDeck' with password 'defaultpass'");
        tft.println("Then go to http://CADDeck.local");
        tft.print("The IP is: ");
        tft.println(WiFi.softAPIP());
        return;
    }
    else if (strcmp(wificonfig.wifimode, "WIFI_STA") == 0) {
        if (!startWifiStation()) {
            startDefaultAP();
            MSG_WARNLN("[WARNING]: Could not connect to AP, so started as AP.");
            tft.println("Started as AP because WiFi connection failed.");
            tft.println("");
            tft.println("To configure, connect to 'CADDeck' with password 'defaultpass'");
            tft.println("Then go to http://CADDeck.local");
            tft.print("The IP is: ");
            tft.println(WiFi.softAPIP());
        }
        else {
            MSG_WARNLN("[WARNING]: Started as STA and in config mode.");
            tft.println("Started as STA and in config mode.");
            tft.println("");
            tft.println("To configure:");
            tft.println("http://CADDeck.local");
            tft.print("The IP is: ");
            tft.println(WiFi.localIP());
        }
    }
    else if (strcmp(wificonfig.wifimode, "WIFI_AP") == 0) {
        MSG_WARNLN("[WARNING]: Started as AP and in config mode.");
        startWifiAP();
        tft.println("Started as AP and in config mode.");
        tft.println("");
        tft.println("To configure:");
        tft.println("http://CADDeck.local");
        tft.print("The IP is: ");
        tft.println(WiFi.softAPIP());
    }
    else {
        MSG_WARNLN("[WARNING]: No valid config mode identified.");
    }
#endif
}

/**
* @brief This function allows for saving (updating) the WiFi SSID
*
* @param String ssid
*
* @return boolean True if succeeded. False otherwise.
*
* @note Returns true if successful. To enable the new set SSID, you must reload the the
         configuration using loadMainConfig()
*/
bool saveWifiSSID(String ssid)
{
    FILESYSTEM.remove("/config/wificonfig.json");
    File file = FILESYSTEM.open("/config/wificonfig.json", "w");

    DynamicJsonDocument doc(384);

    JsonObject wificonfigobject = doc.to<JsonObject>();

    wificonfigobject["ssid"] = ssid;
    wificonfigobject["password"] = wificonfig.password;
    wificonfigobject["wifimode"] = wificonfig.wifimode;
    wificonfigobject["wifihostname"] = wificonfig.hostname;
    wificonfigobject["attempts"] = wificonfig.attempts;
    wificonfigobject["attemptdelay"] = wificonfig.attemptdelay;

    if (serializeJsonPretty(doc, file) == 0) {
        MSG_WARNLN("[WARNING]: Failed to write to file");
        return false;
    }
    file.close();
    return true;
}

/**
* @brief This function allows for saving (updating) the WiFi Password
*
* @param String password
*
* @return boolean True if succeeded. False otherwise.
*
* @note Returns true if successful. To enable the new set password, you must reload the the
         configuration using loadMainConfig()
*/
bool saveWifiPW(String password)
{
    FILESYSTEM.remove("/config/wificonfig.json");
    File file = FILESYSTEM.open("/config/wificonfig.json", "w");

    DynamicJsonDocument doc(384);

    JsonObject wificonfigobject = doc.to<JsonObject>();

    wificonfigobject["ssid"] = wificonfig.ssid;
    wificonfigobject["password"] = password;
    wificonfigobject["wifimode"] = wificonfig.wifimode;
    wificonfigobject["wifihostname"] = wificonfig.hostname;
    wificonfigobject["attempts"] = wificonfig.attempts;
    wificonfigobject["attemptdelay"] = wificonfig.attemptdelay;

    if (serializeJsonPretty(doc, file) == 0) {
        MSG_WARNLN("[WARNING]: Failed to write to file");
        return false;
    }
    file.close();
    return true;
}

/**
* @brief This function allows for saving (updating) the WiFi Mode
*
* @param String wifimode "WIFI_STA" or "WIFI_AP"
*
* @return boolean True if succeeded. False otherwise.
*
* @note Returns true if successful. To enable the new set WiFi Mode, you must reload the the
         configuration using loadMainConfig()
*/
bool saveWifiMode(String wifimode)
{
    if (wifimode != "WIFI_STA" && wifimode != "WIFI_AP") {
        MSG_WARNLN("")
        MSG_WARN2("[WARNING]: WiFi Mode:", wifimode.c_str(), ": not supported. Try WIFI_STA or WIFI_AP.");
        return false;
    }

    FILESYSTEM.remove("/config/wificonfig.json");
    File file = FILESYSTEM.open("/config/wificonfig.json", "w");

    DynamicJsonDocument doc(384);

    JsonObject wificonfigobject = doc.to<JsonObject>();

    wificonfigobject["ssid"] = wificonfig.ssid;
    wificonfigobject["password"] = wificonfig.password;
    wificonfigobject["wifimode"] = wifimode;
    wificonfigobject["wifihostname"] = wificonfig.hostname;
    wificonfigobject["attempts"] = wificonfig.attempts;
    wificonfigobject["attemptdelay"] = wificonfig.attemptdelay;

    if (serializeJsonPretty(doc, file) == 0) {
        MSG_WARNLN("[WARNING]: Failed to write to file");
        return false;
    }
    file.close();
    return true;
}

/**
* @brief This function checks if a file exists and returns a boolean accordingly.
         It then prints a debug message to the serial as wel as the tft.
*
* @param filename (const char *)
*
* @return boolean True if succeeded. False otherwise.
*
* @note Pass the filename including a leading /
*/
bool checkfile(const char* filename, bool showMessage)
{
    if (!FILESYSTEM.exists(filename)) {
        if (showMessage) {
            tft.fillScreen(TFT_BLACK);
            tft.setCursor(1, 3);
            tft.setTextFont(2);
            tft.setTextSize(2);
            tft.setTextColor(TFT_WHITE, TFT_BLACK);
#ifdef LANGUAGE_GERMAN
            tft.printf("%s nicht gefunden!\n\n", filename);
            tft.setTextSize(1);
            tft.printf("Sollte dies nach der Konfiguration geschehen sein, kann es sein, dass die Daten auf dem ESP \nbesch�digt sein.");
#else
            tft.printf("%s not found!\n\n", filename);
            tft.setTextSize(1);
            tft.printf("If this has happend after confguration, the data on the ESP may \nbe corrupted.");
#endif
        }
        return false;
    }
    else {
        return true;
    }
}

bool resetconfig(String file)
{
    char fileNameType[LEN_FILENAME];
    int fileNameMenuNumber = 0;
    int numConverted = 0;
    numConverted = sscanf("%4s%d", file.c_str(), fileNameType, &fileNameMenuNumber);
    if ((numConverted == 2) && strncmp("menu", file.c_str(), 4) && (fileNameMenuNumber >= 0) && (fileNameMenuNumber < NUM_PAGES)) {
        // Reset a menu config

        // Delete the corrupted json file
        String filetoremove = "/config/" + file;
        if (!filetoremove.endsWith(".json")) {
            filetoremove = filetoremove + ".json";
        }

        FILESYSTEM.remove(filetoremove);

        // Copy default.json to the new config file
        File defaultfile = FILESYSTEM.open("/config/default.json", "r");

        size_t n;
        uint8_t buf[64];

        if (defaultfile) {
            File newfile = FILESYSTEM.open(filetoremove, "w");
            if (newfile) {
                while ((n = defaultfile.read(buf, sizeof(buf))) > 0) {
                    newfile.write(buf, n);
                }
                // Close the newly created file
                newfile.close();
            }

#ifdef LANGUAGE_GERMAN
            MSG_INFOLN("[INFO] Zur�cksetzen abgeschlossen.");
            MSG_INFOLN("[INFO] Geben Sie \"restart\" ein, um die Konfiguration neu zu laden.");
#else
            MSG_INFOLN("[INFO] Done resetting.");
            MSG_INFOLN("[INFO] Type \"restart\" to reload configuration.");
#endif

            // Close the default.json file
            defaultfile.close();
            return true;
        }
    }
    else if (file == "general") {
        // Reset the general config
        // For this we do not need to open a default file because we can easily write it ourselfs

        String filetoremove = "/config/" + file;
        if (!filetoremove.endsWith(".json")) {
            filetoremove = filetoremove + ".json";
        }

        FILESYSTEM.remove(filetoremove);

        File newfile = FILESYSTEM.open(filetoremove, "w");
        newfile.println("{");
        newfile.println("\"menubuttoncolor\": \"#009bf4\",");
        newfile.println("\"functionbuttoncolor\": \"#00efcb\",");
        newfile.println("\"latchcolor\": \"#fe0149\",");
        newfile.println("\"background\": \"#000000\",");
        newfile.println("\"sleepenable\": true,");
        newfile.println("\"sleeptimer\": 10,");
        newfile.println("\"usbcommsenable\": false,");
        newfile.println("\"beep\": true,");
        newfile.println("\"modifier1\": 130,");
        newfile.println("\"modifier2\": 129,");
        newfile.println("\"modifier3\": 0,");
        newfile.println("\"helperdelay\": 500,");
        newfile.println("\"startup_menu\": 0,");
        newfile.println("\"gpio_pin\": 255,");
        newfile.println("\"gpio_pin_mode\": 0,");

        newfile.println("}");

        newfile.close();
        MSG_BASICLN("[INFO] Done resetting general config.");
        MSG_BASICLN("[INFO] Type \"restart\" to reload configuration.");
        return true;
    }
    else if (file == "cadparams") {
        // Reset the general config
        // For this we do not need to open a default file because we can easily write it ourselfs

        String filetoremove = "/config/" + file;
        if (!filetoremove.endsWith(".json")) {
            filetoremove = filetoremove + ".json";
        }

        FILESYSTEM.remove(filetoremove);

        File newfile = FILESYSTEM.open(filetoremove, "w");
        newfile.println("{");
        newfile.println("\"version\": 2,");
        newfile.println("\"joy_scale_x\": 1.0,");
        newfile.println("\"joy_scale_y\": 1.0,");
        newfile.println("\"joy_deadzone\": 0.01,");
        newfile.println("\"joy_sensitivity\": 10,");
        newfile.println("\"zoom_scale\": 1.0,");
        newfile.println("\"zoom_deadzone\": 0.01,");
        newfile.println("\"zoom_sensitivity\": 10,");
        newfile.println("\"rotate_scale\": 1.0,");
        newfile.println("\"rotate_deadzone\": 0.01,");
        newfile.println("\"rotate_sensitivity\": 10,");
        newfile.println("\"mouse_sensitivity\": 10,");
        newfile.println("\"CADProgram\": \"Solidworks\" ");
        newfile.println("\"spacemouse_enable\": true");
        newfile.println("}");

        newfile.close();
        MSG_BASICLN("[INFO] Done resetting cadparams.json.");
        MSG_BASICLN("[INFO] Type \"restart\" to reload configuration.");
        return true;
    }
    else {
        MSG_WARNLN("[WARNING]: Invalid reset option. Choose: general, menu1, menu2, menu3...");
        return false;
    }

    return false;
}

bool CopyFile(String FileOriginal, String FileCopy)
{
    bool success = true;

    uint8_t ibuffer[64];  // declare a buffer

#ifdef LANGUAGE_GERMAN
    MSG_INFOLN("[INFO] CopyFile()");
#else
    MSG_INFOLN("[INFO] In CopyFile()");
#endif

    if (LittleFS.exists(FileCopy) == true)  // remove file copy
    {
        LittleFS.remove(FileCopy);
    }

    File f1 = LittleFS.open(FileOriginal, "r");  // open source file to read
    if (f1) {
        File f2 = LittleFS.open(FileCopy, "w");  // open destination file to write
        if (f2) {
            while (f1.available() > 0) {
                size_t i = f1.read(ibuffer, 64);  // i = number of bytes placed in buffer from file f1
                f2.write(ibuffer, i);             // write i bytes from buffer to file f2
            }
            f2.close();  // done, close the destination file
        }
        else {
            MSG_ERROR1("[ERROR] CopyFile() can't open destination file ", FileCopy.c_str());  // debug
            success = false;
        }

        f1.close();  // done, close the source file
    }
    else {
        MSG_ERROR1("[ERROR] CopyFile() can't open source file ", FileOriginal.c_str());  // debug
        success = false;
    }

    if (success) {
#ifdef LANGUAGE_GERMAN
        MSG_INFOLN("[INFO] CopyFile() mit Erfolg beendet");
#else
        MSG_INFOLN("[INFO] End CopyFile() with success");
#endif
    }
    else {
        MSG_ERRORLN("[ERRROR] End CopyFile() with failure.");
    }

    return success;
}