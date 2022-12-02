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

#define FASTLED_INTERRUPT_RETRY_COUNT 0
#define FASTLED_ALLOW_INTERRUPTS 0

WebServer _webServerHost;

void setup()
{
    _webServerHost.initWebServer();
    _webServerHost.initLEDS();
}

void loop()
{
    delay(100);
    _webServerHost.handleClient();
}