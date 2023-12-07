#include "SerialCommands.h"

void serial_commands()
{
    // Check if there is data available on the serial input that needs to be handled.

    if (Serial.available()) {
        String command = Serial.readStringUntil(' ');

        if (command == "cal") {
            MSG_INFOLN("[INFO] received command cal");
            FILESYSTEM.remove(CALIBRATION_FILE);
            ESP.restart();
        }
        else if (command == "setssid") {
            String value = Serial.readString();
            MSG_INFO1("[INFO] received command setssid ", value.c_str());
            if (saveWifiSSID(value)) {
                MSG_INFO1F("[INFO] Saved new SSID: %s\n", value.c_str());
                loadMainConfig();
                MSG_INFOLN("[INFO] New configuration loaded");
            }
        }
        else if (command == "setpassword") {
            String value = Serial.readString();
            MSG_INFO1("[INFO] received command setpassword ", value.c_str());
            if (saveWifiPW(value)) {
                MSG_INFO1F("[INFO] Saved new Password: %s\n", value.c_str());
                loadMainConfig();
                MSG_INFOLN("[INFO] New configuration loaded");
            }
        }
        else if (command == "setwifimode") {
            String value = Serial.readString();
            MSG_INFO1("[INFO] received command setwifimode ", value.c_str());
            if (saveWifiMode(value)) {
                MSG_INFO1F("[INFO] Saved new WiFi Mode: %s\n", value.c_str());
                loadMainConfig();
                MSG_INFOLN("[INFO] New configuration loaded");
            }
        }
        else if (command == "setwifimodesta") {
            String value = "WIFI_STA";
            MSG_INFO1("[INFO] received command setwifimode ", value.c_str());
            if (saveWifiMode(value)) {
                MSG_INFO1F("[INFO] Saved new WiFi Mode: %s\n", value.c_str());
                loadMainConfig();
                MSG_INFOLN("[INFO] New configuration loaded");
            }
        }
        else if (command == "setwifimodeap") {
            String value = "WIFI_AP";
            MSG_INFO1("[INFO] received command setwifimode ", value.c_str());
            if (saveWifiMode(value)) {
                MSG_INFO1F("[INFO] Saved new WiFi Mode: %s\n", value.c_str());
                loadMainConfig();
                MSG_INFOLN("[INFO] New configuration loaded");
            }
        }
        else if (command == "restart") {
            MSG_WARNLN("[WARNING]: Restarting");
            ESP.restart();
        }
        else if (command == "reset") {
            String file = Serial.readString();
            MSG_INFO1F("[INFO] Resetting %s.json now\n", file.c_str());
            resetconfig(file);
        }
        else {
            for (size_t i = 0; i < NUM_PAGES; i++) {
                char menuName[16];
                sprintf(menuName, "menu%d", i);

                if (command == menuName && pageNum != i && pageNum != WEB_REQUEST_PAGE) {
                    uint8_t status = pageHistoryStack.push(pageNum);
                    if (status == STACK_STATUS_FULL) {
                        MSG_INFOLN("[INFO] Page History Stack is full. Dropped oldest value..");
                    }
                    pageNum = i;

                    drawKeypad();
                    MSG_INFO("[Info] Switched to ");
                    MSG_INFOLN(menuName);
                }
            }
        }
    }
}