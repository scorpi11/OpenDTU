// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once

#include <ESPAsyncWebServer.h>

class WebApiDtuClass {
public:
    void init(AsyncWebServer* server, void (*resetFunc)(void));
    void loop();

private:
    void onDtuAdminGet(AsyncWebServerRequest* request);
    void onDtuAdminPost(AsyncWebServerRequest* request);
    void onDtuAdminRestartPost(AsyncWebServerRequest* request);

    void (*_resetFunc)(void) = 0;
    AsyncWebServer* _server;
};