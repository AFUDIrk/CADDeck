// Start as WiFi station
#include "ConfigHelper.h"

bool startWifiStation()
{
    MSG_INFO1("startWifiStation(): Verbinden mit", wificonfig.ssid);
    if (String(WiFi.SSID()) != String(wificonfig.ssid)) {
        WiFi.mode(WIFI_STA);
        MSG_INFO2("startWifiStation(): ssid, pwd=", wificonfig.ssid, wificonfig.password);
        WiFi.begin(wificonfig.ssid, wificonfig.password);
        uint8_t attempts = wificonfig.attempts;
//        MSG_DEBUG1("startWifiStation(): attempts =", attempts);
//        MSG_DEBUG1("startWifiStation(): attempt delay =", wificonfig.attemptdelay);
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
    //Keyboard.end();

    // Stop BLE from interfering with our WIFI signal
    btStop();
    esp_bt_controller_disable();
    esp_bt_controller_deinit();
    esp_bt_controller_mem_release(ESP_BT_MODE_BTDM);

    MSG_INFOLN("");
    MSG_INFOLN("[INFO] BLE gestoppt");
    MSG_INFO("[INFO] Verbunden! IP Adresse: ");
    MSG_INFOLN(WiFi.localIP());

    MDNS.begin(wificonfig.hostname);
    MDNS.addService("http", "tcp", 80);

    // Set pageNum to 7 so no buttons are displayed and touches are ignored
    pageNum = WEB_REQUEST_PAGE;
    pageHistoryStack.push(pageNum);

    // Start the webserver
    webserver.begin();
    MSG_INFOLN("[INFO] Webserver gestartet");
    return true;
}

// Start as WiFi AP

void startWifiAP()
{
    WiFi.mode(WIFI_AP);
    WiFi.softAP(wificonfig.ssid, wificonfig.password);
    MSG_INFOLN("");
    MSG_INFO("[INFO] Zugangspunkt gestartet! IP Adresse: ");
    MSG_INFOLN(WiFi.softAPIP());

    // Delete the task Keyboard had create to free memory and to not interfere with AsyncWebServer
    //Keyboard.end();

    // Stop BLE from interfering with our WIFI signal
    btStop();
    esp_bt_controller_disable();
    esp_bt_controller_deinit();
    esp_bt_controller_mem_release(ESP_BT_MODE_BTDM);

    MSG_INFOLN("");
    MSG_INFOLN("[INFO] BLE gestoppt");

    MDNS.begin(wificonfig.hostname);
    MDNS.addService("http", "tcp", 80);

    // Set pageNum to WEB_REQUEST_PAGE so no buttons are displayed and touches are ignored
    pageNum = WEB_REQUEST_PAGE;
    pageHistoryStack.push(pageNum);

    // Start the webserver
    webserver.begin();
    MSG_INFOLN("[INFO] Webserver gestartet");
}

// Start the default AP

void startDefaultAP()
{
    const char* ssid = "CADDeck";
    const char* password = "defaultpass";

    WiFi.mode(WIFI_AP);
    WiFi.softAP(ssid, password);
    MSG_INFO("[INFO] Zugangspunkt gestartet! IP Adresse: ");
    MSG_INFOLN(WiFi.softAPIP());

    // Delete the task Keyboard had create to free memory and to not interfere with AsyncWebServer
    //leKeyboard.end();

    // Stop BLE from interfering with our WIFI signal
    btStop();
    esp_bt_controller_disable();
    esp_bt_controller_deinit();
    esp_bt_controller_mem_release(ESP_BT_MODE_BTDM);

    MSG_INFOLN("[INFO] BLE gestoppt");

    MDNS.begin("CADDeck");
    MDNS.addService("http", "tcp", 80);

    // Set pageNum to WEB_REQUEST_PAGE so no buttons are displayed and touches are ignored
    pageNum = WEB_REQUEST_PAGE;
    pageHistoryStack.push(pageNum);

    // Start the webserver
    webserver.begin();
    MSG_INFOLN("[INFO] Webserver gestartet");
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

    MSG_INFOLN("[INFO] Konfigurationsmodus aufrufen");
    tft.println("Verbindung mit WLAN herstellen...");
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
        tft.println("Stellen Sie zum Konfigurieren eine Verbindung zu „CADDeck“ mit dem Passwort „defaultpass“ her'");
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
        tft.println("Stellen Sie zum Konfigurieren eine Verbindung zu „CADDeck“ mit dem Passwort „defaultpass“ her'");
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
            tft.println("Stellen Sie zum Konfigurieren eine Verbindung zu „CADDeck“ mit dem Passwort „defaultpass“ her'");
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
        MSG_WARNLN("[WARNUNG]: Kein gültiger Konfigurationsmodus identifiziert.");
    }
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
        MSG_WARNLN("[WARNUNG]: Das Schreiben in die Datei ist fehlgeschlagen");
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
        MSG_WARNLN("[WARNUNG]: Das Schreiben in die Datei ist fehlgeschlagen");
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
        MSG_WARN2("[WARNUNG]: WLAN-Modus:", wifimode.c_str(), ": nicht unterstützt. Versuchen Sie WIFI_STA oder WIFI_AP.");
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
            tft.printf("%s nicht gefunden!\n\n", filename);
            tft.setTextSize(1);
            tft.printf("Sollte dies nach der Konfiguration geschehen sein, kann es sein, dass die Daten auf dem ESP \nbeschädigt sein.");
        }
        return false;
    }
    else {
        return true;
    }
}

bool resetconfig(String file)
{
    char fileNameType[20];
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
            MSG_INFOLN("[INFO] Zurücksetzen abgeschlossen.");
            MSG_INFOLN("[INFO] Geben Sie \"restart\" ein, um die Konfiguration neu zu laden.");

            // Close the default.json file
            defaultfile.close();
            return true;
        }
    }
    else if (file == "allgemein") {
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
        newfile.println("\"CADProgram\": \"Solidworks\" ");

        newfile.println("}");

        newfile.close();
        MSG_BASICLN("[INFO] Das Zurücksetzen der allgemeinen Konfiguration ist abgeschlossen.");
        MSG_BASICLN("[INFO] Geben Sie \"restart\" ein, um die Konfiguration neu zu laden.");
        return true;
    }
    else {
        MSG_WARNLN("[WARNUNG]: Ungültige Reset-Option. Wählen Sie: Allgemein, Menü1, Menü2, Menü3...");
        return false;
    }

    return false;
}

bool CopyFile(String FileOriginal, String FileCopy)
{
    bool success = true;

    uint8_t ibuffer[64];  // declare a buffer

    MSG_INFOLN("[INFO] CopyFile()");

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
            MSG_ERROR1("[FEHLER] Copy File () kann die Zieldatei nicht öffnen ", FileCopy.c_str());  // debug
            success = false;
        }

        f1.close();  // done, close the source file
    }
    else {
        MSG_ERROR1("[FEHLER] CopyFile() kann die Quelldatei nicht öffnen ", FileOriginal.c_str());  // debug
        success = false;
    }

    if (success) {
        MSG_INFOLN("[INFO] CopyFile() mit Erfolg beendet");
    }
    else {
        MSG_ERRORLN("[FEHLER] CopyFile() mit Fehler beendet.");
    }

    return success;
}