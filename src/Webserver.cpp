#include "WebServer.h"

#include "SaveConfig.h"

/**
* @brief This function returns all the files in a given directory in a json
         formatted string.
*
* @param path String
*
* @return String
*
* @note none
*/

String errorCode;
String errorText;
String resultHeader;
String resultText;
String resultFiles = "";

String handleFileList(String path)
{
    File root = FILESYSTEM.open(path);
    path = String();
    int filecount = 0;

    String output = "[";
    if (root.isDirectory()) {
        File file = root.openNextFile();
        while (file) {
            if (output != "[") {
                output += ',';
            }

            output += "{\"";
            output += filecount;
            output += "\":\"";

#ifdef ESP_ARDUINO_VERSION_MAJOR
            output += String(file.name());
#else
            output += String(file.name()).substring(7);
#endif

            output += "\"}";
            file = root.openNextFile();
            filecount++;
        }

        file.close();
    }
    output += "]";
    root.close();
    return output;
}

String handleAPISList()
{
    File root = FILESYSTEM.open("/uploads");

    int filecount = 0;

    String output = "[";
    if (root.isDirectory()) {
        File file = root.openNextFile();
        while (file) {
            String filepath = String(file.name()).substring(0, 16);
            if (filepath == "/uploads/config_") {
                file = root.openNextFile();
                filecount++;
            }
            else {
                String filename = String(file.name()).substring(9);
                if (output != "[") {
                    output += ',';
                }

                output += "{\"";
                output += filecount;
                output += "\":\"";
                output += String(file.name()).substring(9);
                output += "\"}";
                file = root.openNextFile();
                filecount++;
            }
        }
        file.close();
    }
    output += "]";
    root.close();
    return output;
}

/**
* @brief This function returns information about CADDeck in a json
         formatted string.
*
* @param none
*
* @return String
*
* @note none
*/
String handleInfo()
{
    float freemem = FILESYSTEM.totalBytes() - FILESYSTEM.usedBytes();

    String output = "[";

    output += "{\"";
    output += "Version";
    output += "\":\"";
    output += String(versionnumber);
    output += "\"},";

    output += "{\"";
    output += "Free Space";
    output += "\":\"";
    output += String(freemem / 1000);
    output += " kB\"},";

    output += "{\"";
    output += "BLE Keyboard-Mouse Combo Version";
    output += "\":\"";
    output += String(BLE_COMBO_VERSION);
    output += "\"},";

    output += "{\"";
    output += "ArduinoJson Version";
    output += "\":\"";
    output += String(ARDUINOJSON_VERSION);
    output += "\"},";

    output += "{\"";
    output += "TFT_eSPI Version";
    output += "\":\"";
    output += String(TFT_ESPI_VERSION);
    output += "\"},";

    output += "{\"";
    output += "ESP-IDF";
    output += "\":\"";
    output += String(esp_get_idf_version());
    output += "\"},";

    output += "{\"";
    output += "WiFi Mode";
    output += "\":\"";
    output += String(wificonfig.wifimode);
    output += "\"},";

#ifdef TOUCH_INTERRUPT_PIN
    output += "{\"";
    output += "Sleep";
    output += "\":\"";
    if (generalconfig.sleepenable) {
        output += String("Enabled. ");
        output += String("Timer: ");
        output += String(generalconfig.sleeptimer);
        output += String(" minutes");
        output += "\"},";
    }
    else {
        output += String("Disabled");
        output += "\"},";
    }
#else
    output += "{\"";
    output += "Sleep";
    output += "\":\"";
    output += String("Disabled");
    output += "\"},";

#endif
    output += "{\"";
    output += "USB Comms";
    output += "\":\"";
    if (generalconfig.usbcommsenable) {
        output += String("Enabled. ");
        output += "\"}";
    }
    else {
        output += String("Disabled");
        output += "\"}";
    }

    output += "]";

    return output;
}

/**
 * @brief This function handles error.htm template processing.
 *
 * @param var const String&
 *
 * @return String
 *
 * @note none
 */
String processor(const String &var)
{
    if (var == "ERROR_CODE") {
        return errorCode;
    }
    if (var == "ERROR_TEXT") {
        return errorText;
    }
    return String();
}

/**
 * @brief This function handles JSON file uploads. only menu0.json, menu1.json, menu2.json (etc.), and colors.json are accepted.
 *
 * @param *request AsyncWebServerRequest
 * @param filename String
 * @param index size_t
 * @param *data uint8_t
 * @param len size_t
 * @param final boolean
 *
 * @return none
 *
 * @note none
 */
void handleJSONUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final)
{
    bool validMenuName = false;
    int endIndex = 0;
    int startIndex = 0;
    String menuIndexText;
    int menuIndexNum = 0;

    if (filename.indexOf("menu") == 0) {
        startIndex = 4;
        endIndex = filename.indexOf(".json");
        if (endIndex > startIndex) {
            menuIndexText = filename.substring(startIndex, endIndex);
            menuIndexNum = menuIndexText.toInt();

            if ((menuIndexNum > 0) || (menuIndexText[0] == '0')) {
                validMenuName = true;
            }
        }
    }

    if (!validMenuName && filename != "general.json" && filename != "wificonfig.json" && filename != "cadparams.json") {
        MSG_INFO1F("[INFO] JSON has invalid name: %s\n", filename.c_str());
        errorCode = "102";
        errorText = "JSON file has an invalid name. You can only upload JSON files with the following file names:";
        errorText += "<ul><li>menu0.json</li>...<li>menu9.json</li>";
        errorText += "<li>cadparams.json</li><li>general.json</li><li>wificonfig.json</li></ul>";
        request->send(FILESYSTEM, "/error.htm", String(), false, processor);
        return;
    }
    if (!index) {
        MSG_INFO1F("[INFO] JSON Upload Start: %s\n", filename.c_str());
        filename = "/config/" + filename;  // TODO: Does the config directory need to be hardcoded?

        // Open the file on first call and store the file handle in the request object
        request->_tempFile = FILESYSTEM.open(filename, "w");
    }
    if (len) {
        // Stream the incoming chunk to the opened file
        request->_tempFile.write(data, len);
    }
    if (final) {
        MSG_INFO1F("[INFO] JSON Uploaded: %s\n", filename.c_str());
        // Close the file handle as the upload is now done
        request->_tempFile.close();
        request->send(FILESYSTEM, "/upload.htm");
    }
}

/**
 * @brief This function handles API JSON file uploads. These are placed in the uploads folder.
 *
 * @param *request AsyncWebServerRequest
 * @param filename String
 * @param index size_t
 * @param *data uint8_t
 * @param len size_t
 * @param final boolean
 *
 * @return none
 *
 * @note none
 */
void handleAPIUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final)
{
    if (!index) {
        MSG_INFO1F("[INFO] API file Upload Start: %s\n", filename.c_str());
        filename = "/uploads/" + filename;  // TODO: Does the uploads directory need to be hardcoded?

        // Open the file on first call and store the file handle in the request object
        request->_tempFile = FILESYSTEM.open(filename, "w");
    }
    if (len) {
        // Stream the incoming chunk to the opened file
        request->_tempFile.write(data, len);
    }
    if (final) {
        MSG_INFO1F("[INFO] API file Uploaded: %s\n", filename.c_str());
        // Close the file handle as the upload is now done
        request->_tempFile.close();
        request->send(FILESYSTEM, "/upload.htm");
    }
}

/* --------------- Checking for free space on FILESYSTEM ----------------
Purpose: This checks if the free memory on the FILESYSTEM is bigger then a set threshold
Input  : none
Output : boolean
Note   : none
*/

bool spaceLeft()
{
    float minmem = 100000.00;  // Always leave 100 kB free pace on FILESYSTEM
    float freeMemory = FILESYSTEM.totalBytes() - FILESYSTEM.usedBytes();
    MSG_INFO1F("[INFO] Free memory left: %f bytes\n", freeMemory);
    if (freeMemory < minmem) {
        return false;
    }

    return true;
}
/**
* @brief This function handles a file upload used by the Webserver
*
* @param *request
* @param filename String
* @param index size_t
* @param *data uint8_t
* @param len size_t
* @param final boolean
*
* @return none
*
* @note The reason the file is first uploaded and then deleted if there is not enough free space, is that
         if the request is not handled, the ESP32 craches. So we have to accept the upload but
         can delete it.
*/
void handleUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final)
{
    if (!index) {
        MSG_INFO1F("[INFO] File Upload Start: %s\n", filename.c_str());
        filename = "/logos/" + filename;
        // Open the file on first call and store the file handle in the request object
        request->_tempFile = FILESYSTEM.open(filename, "w");
    }
    if (len) {
        // Stream the incoming chunk to the opened file
        request->_tempFile.write(data, len);
    }
    if (final) {
        MSG_INFO1F("[INFO] File Uploaded: %s\n", filename.c_str());
        // Close the file handle as the upload is now done
        request->_tempFile.close();

        // If there is not enough space left, we have to delete the recently uploaded file
        if (!spaceLeft()) {
            MSG_WARNLN("[WARNING]: Not enough free space left");
            errorCode = "103";
            errorText = "There is not enough free space left to upload a logo. Please delete unused logos and try again.";
            request->send(FILESYSTEM, "/error.htm", String(), false, processor);

            // Remove the recently uploaded file
            String fileToDelete = "/logos/";
            fileToDelete += filename;
            FILESYSTEM.remove(fileToDelete);
            MSG_WARNLN("[WARNING]: File removed to keep enough free space");
            return;
        }
        else {
            request->send(FILESYSTEM, "/upload.htm");
        }
    }
}

/**
 * @brief This function handles delete.htm template processing.
 *
 * @param  &var const String
 *
 * @return String
 *
 * @note Only need to call this once! This is also where the saving of config files is done.
 */
String deleteProcessor(const String &var)
{
    if (var == "RESULT") {
        return resultHeader;
    }
    if (var == "TEXT") {
        return resultText;
    }
    if (var == "FILES") {
        return resultFiles;
    }
    return String();
}

/**
 * @brief This function adds all the handlers we need to the webserver.
 *
 * @param none
 *
 * @return none
 *
 * @note Only need to call this once! This is also where the saving of config files is done.
 */
void handlerSetup()
{
    webserver.serveStatic("/", FILESYSTEM, "/").setDefaultFile("index.htm");

    //----------- index.htm handler -----------------

    webserver.on("/index.htm", HTTP_POST, [](AsyncWebServerRequest *request) {
        request->send(FILESYSTEM, "/index.htm");
    });

    //----------- saveconfig handler -----------------

    webserver.on("/saveconfig", HTTP_POST, [](AsyncWebServerRequest *request) {
        if (request->hasParam("save", true)) {
            int status = 0;
            AsyncWebParameter *p = request->getParam("save", true);
            String savemode = p->value().c_str();

            if (savemode == "general") {
                status = saveConfigGeneral(request);
            }
            else if (savemode == "wifi") {
                status = saveConfigWiFi(request);
            }
            else if (strncmp("menu", savemode.c_str(), 4) == 0) {
                status = saveConfigMenu(request, savemode);
            }
            else if (savemode == "cadparams") {
                status = saveConfigCADParams(request);
            }
            request->send(FILESYSTEM, "/saveconfig.htm");
        }
    });

    //----------- updatebutton handler -----------------

    webserver.on("/updatebutton", HTTP_GET, [](AsyncWebServerRequest *request) {
        String value;
        String debugString;
        String description;

        int appIndex = 0;
        int buttonIndex = 0;
        int arrayIndex = 0;
        int actionValue = 0;

        uint8_t action = 0;

        if (request->hasParam("app") && request->hasParam("button") &&
            request->hasParam("arrayIndex") && request->hasParam("actionValue") &&
            request->hasParam("action") && request->hasParam("value") && 
            request->hasParam("description")) {
            appIndex = request->getParam("app")->value().toInt();
            buttonIndex = request->getParam("button")->value().toInt();
            arrayIndex = request->getParam("arrayIndex")->value().toInt();
            actionValue = request->getParam("actionValue")->value().toInt();
            action = request->getParam("action")->value().toInt();
            value = request->getParam("value")->value();
            description = request->getParam("description")->value();

            if (appIndex >= 0 && appIndex < NUM_CAD_PROGRAMS) {
                if (buttonIndex >= 0 && buttonIndex < NUM_HW_BUTTONS) {

                     strlcpy(cadprogramconfig[appIndex].hw_button_descriptions[buttonIndex], description.c_str(),
                             sizeof(cadprogramconfig[appIndex].hw_button_descriptions[buttonIndex]));

                     cadprogramconfig[appIndex].hw_buttons[buttonIndex][arrayIndex].action = value.toInt();

                     cadprogramconfig[appIndex].hw_buttons[buttonIndex][arrayIndex].action = action;

                     if (action == Action_Char || action == Action_SpecialChar) {
                         strcpy(cadprogramconfig[appIndex].hw_buttons[buttonIndex][arrayIndex].symbol, value.c_str());
                    }
                    else {
                        cadprogramconfig[appIndex].hw_buttons[buttonIndex][arrayIndex].value = value.toInt();
                    }

                    // debugString = "appIndex: " + String(appIndex) + ", buttonIndex: " + String(buttonIndex) + 
                    //               "description: " + description +
                    //               ", arrayIndex: " + String(arrayIndex) + ", action: " + action + ", value: " + value;
                    // MSG_DEBUG1("[DEBUG] updatebutton: ", debugString.c_str());
                }
                else {
                    MSG_ERROR1("[ERROR] updatebutton: invalid button index: ", buttonIndex);
                }
            }
            else {
                MSG_ERROR1("[ERROR] updatebutton: invalid app index: ", appIndex);
            }
        }
        request->send(200, "text/plain", "OK");
    });
    //----------- updatebuttondescription handler -----------------

    webserver.on("/updatebuttondescription", HTTP_GET, [](AsyncWebServerRequest *request) {
        String value;
        String debugString;
        String description;

        int appIndex = 0;
        int buttonIndex = 0;
        int arrayIndex = 0;
        int actionValue = 0;

        uint8_t action = 0;

        if (request->hasParam("app") && request->hasParam("button") &&
            request->hasParam("description")) {
            appIndex = request->getParam("app")->value().toInt();
            buttonIndex = request->getParam("button")->value().toInt();
            description = request->getParam("description")->value();

            if (appIndex >= 0 && appIndex < NUM_CAD_PROGRAMS) {
                if (buttonIndex >= 0 && buttonIndex < NUM_HW_BUTTONS) {
                    strlcpy(cadprogramconfig[appIndex].hw_button_descriptions[buttonIndex], description.c_str(),
                            sizeof(cadprogramconfig[appIndex].hw_button_descriptions[buttonIndex]));

                    // debugString = "appIndex: " + String(appIndex) + ", buttonIndex: " + String(buttonIndex) +
                    //               "description: " + description ;
                    // MSG_DEBUG1("[DEBUG] updatebuttondescription: ", debugString.c_str());
                }
                else {
                    MSG_ERROR1("[ERROR] updatebuttondescription: invalid button index: ", buttonIndex);
                }
            }
            else {
                MSG_ERROR1("[ERROR] updatebuttondescription: invalid app index: ", appIndex);
            }
        }
        request->send(200, "text/plain", "OK");
    });

    //----------- File list handler -----------------

    webserver.on("/list", HTTP_GET, [](AsyncWebServerRequest *request) {
        if (request->hasParam("dir")) {
            AsyncWebParameter *p = request->getParam("dir");
            request->send(200, "application/json", handleFileList(p->value().c_str()));
        }
    });

    webserver.on("/apislist", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "application/json", handleAPISList());
    });

    webserver.on("/info", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "application/json", handleInfo());
    });

    //----------- 404 handler -----------------

    webserver.onNotFound([](AsyncWebServerRequest *request) {
        Serial.printf("NOT_FOUND: ");
        if (request->method() == HTTP_GET)
            Serial.printf("GET");
        else if (request->method() == HTTP_POST)
            Serial.printf("POST");
        else if (request->method() == HTTP_DELETE)
            Serial.printf("DELETE");
        else if (request->method() == HTTP_PUT)
            Serial.printf("PUT");
        else if (request->method() == HTTP_PATCH)
            Serial.printf("PATCH");
        else if (request->method() == HTTP_HEAD)
            Serial.printf("HEAD");
        else if (request->method() == HTTP_OPTIONS)
            Serial.printf("OPTIONS");
        else
            Serial.printf("UNKNOWN");
        Serial.printf(" http://%s%s\n", request->host().c_str(), request->url().c_str());

        if (request->contentLength()) {
            Serial.printf("_CONTENT_TYPE: %s\n", request->contentType().c_str());
            Serial.printf("_CONTENT_LENGTH: %u\n", request->contentLength());
        }

        int headers = request->headers();
        int i;
        for (i = 0; i < headers; i++) {
            AsyncWebHeader *h = request->getHeader(i);
            Serial.printf("_HEADER[%s]: %s\n", h->name().c_str(), h->value().c_str());
        }

        int params = request->params();
        for (i = 0; i < params; i++) {
            AsyncWebParameter *p = request->getParam(i);
            if (p->isFile()) {
                Serial.printf("_FILE[%s]: %s, size: %u\n", p->name().c_str(), p->value().c_str(), p->size());
            }
            else if (p->isPost()) {
                Serial.printf("_POST[%s]: %s\n", p->name().c_str(), p->value().c_str());
            }
            else {
                Serial.printf("_GET[%s]: %s\n", p->name().c_str(), p->value().c_str());
            }
        }

        request->send(404);
    });

    webserver.onFileUpload([](AsyncWebServerRequest *request, const String &filename, size_t index, uint8_t *data, size_t len, bool final) {
        if (!index)
            Serial.printf("UploadStart: %s\n", filename.c_str());
        Serial.printf("%s", (const char *)data);
        if (final)
            Serial.printf("UploadEnd: %s (%u)\n", filename.c_str(), index + len);
    });

    webserver.onRequestBody([](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
        if (!index)
            Serial.printf("BodyStart: %u\n", total);
        MSG_INFO1F("%s", (const char *)data);
        if (index + len == total)
            MSG_INFO1F("BodyEnd: %u\n", total);
    });

    webserver.on(
        "/upload", HTTP_POST, [](AsyncWebServerRequest *request) {
            request->send(FILESYSTEM, "/upload.htm");
        },
        handleUpload);

    webserver.on("/restart", HTTP_POST, [](AsyncWebServerRequest *request) {
        // First send some text to the browser otherwise an ugly browser error shows up
        request->send(200, "text/plain", "CADDeck is restarting...");
        // Then restart the ESP
        MSG_WARNLN("[WARNING]: Restarting");
        ESP.restart();
    });

    // ----------------------------- Error Handle ---------------------------------

    webserver.on("/error", HTTP_GET, [](AsyncWebServerRequest *request) {
        errorCode = "123";
        errorText = "Nothing went wrong, all is good. This is just a test";
        request->send(FILESYSTEM, "/error.htm", String(), false, processor);
    });

    // ----------------------------- Editor Handle ---------------------------------

    webserver.on("/editor", HTTP_POST, [](AsyncWebServerRequest *request) {
        int params = request->params();
        int i;
        int filecount = 0;
        for (i = 0; i < params; i++) {
            AsyncWebParameter *p = request->getParam(i);
            MSG_INFO1F("[INFO] Deleting file: %s\n", p->value().c_str());
            String filename = "/logos/";
            filename += p->value().c_str();
            if (FILESYSTEM.exists(filename)) {
                FILESYSTEM.remove(filename);
            }

            resultFiles += p->value().c_str();
            resultFiles += "<br>";
            filecount++;
        }
        if (filecount > 0) {
            resultHeader = "Success!";
        }
        else {
            resultHeader = "Fail!";
        }
        resultText = String(filecount);
        request->send(FILESYSTEM, "/editor.htm", String(), false, deleteProcessor);
        resultFiles = "";
    });

    // ----------------------------- Download Handle ---------------------------------

    webserver.on("/download", HTTP_GET, [](AsyncWebServerRequest *request) {
        AsyncWebParameter *p = request->getParam("file");
        String filerequest = p->value().c_str();
        MSG_INFO1F("[INFO] Requested file: %s\n", filerequest.c_str());

        String downloadfile = "/config/" + filerequest;
        MSG_INFO1F("[INFO] Full path: %s\n", downloadfile.c_str());

        if (FILESYSTEM.exists(downloadfile)) {
            MSG_INFO1F("[INFO] Download file %s\n", downloadfile.c_str());
            request->send(FILESYSTEM, downloadfile, String(), true);
        }
        else {
            MSG_INFO1F("[INFO] Download file %s doesn't exits!\n", downloadfile.c_str());
        }
    });

    // ----------------------------- JSON Upload Handle ---------------------------------
    webserver.on(
        "/uploadJSON", HTTP_POST, [](AsyncWebServerRequest *request) {}, handleJSONUpload);

    // ----------------------------- JSON Download Handle ---------------------------------
    webserver.on("/downloadJSON", HTTP_GET, [](AsyncWebServerRequest *request) {
        MSG_INFOLN("webserver.on downloadJSON");

        AsyncWebParameter *p = request->getParam("menunumberlist_download");
        String filerequest = "menu" + p->value() + ".json";

        MSG_INFO1F("[INFO] Requested file: %s\n", filerequest.c_str());

        String downloadfile = "/config/" + filerequest;
        MSG_INFO1F("[INFO] Full path: %s\n", downloadfile.c_str());

        if (FILESYSTEM.exists(downloadfile)) {
            MSG_INFO1F("[INFO] Download file %s\n", downloadfile.c_str());
            request->send(FILESYSTEM, downloadfile, String(), true);
        }
        else {
            MSG_INFO1F("[INFO] Download file %s doesn't exits!\n", downloadfile.c_str());
        }
    });
}
