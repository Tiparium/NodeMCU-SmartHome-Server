/*
  ==============================================================================

    Main.cpp
    Created: 1 Dec 2022 5:22:??pm
    Author:  Tiparium

    Main

  ==============================================================================
*/

// Mandatory
#include <Arduino.h>

// LED Control
#include <Adafruit_NeoPixel.h>
#include <FastLED.h> // <- May be removed

// Local Code
#include "WebServer.h"

WebServer _webServerHost;

void setup()
{
    _webServerHost.initWebServer();
    _webServerHost.initLEDS();
}

void loop()
{
    delay(200);
    _webServerHost.handleClient();
}