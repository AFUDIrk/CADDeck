#pragma once
#include "CADDeck.h"

int saveConfigGeneral(AsyncWebServerRequest *request);
int saveConfigCADParams(AsyncWebServerRequest *request);
int saveConfigWiFi(AsyncWebServerRequest *request);
int saveConfigMenu(AsyncWebServerRequest *request, String savemode);
int saveCurrentConfigGeneral();
int saveCurrentConfigCADParams();
