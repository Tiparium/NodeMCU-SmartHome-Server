/*
  ==============================================================================

    WebServer.h
    Created: 1 Dec 2022 5:20:??pm
    Author:  Tiparium

    Instantiates & Hosts a Webserver on the NodeMCU

  ==============================================================================
*/

#pragma once
#include <Arduino.h>

// WebServer
#include <Wire.h>
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>

// Data
#include "data.h"
#include "RGBStructs.h"
#include "ArduinoUtils.h"
#include "../src/led_controller.h"

#define _led_pin D2

class WebServer {
public:
    // Webserver
    int     initWebServer();
    void    handleClient();

    void    setLocalColors();
    void    getLocalColors();

    // LED Controller
    void    initLEDS();
    void    syncLEDS();

    // Debug
    void debugPrintRGBPattern();
private:
    // Webserver
    data                    _data;
    ESP8266WebServer        _server;
    std::vector<TIP_RGB>    _rgbPattern;
    TIP_RGB                 _rgb = TIP_RGB(0, 0, 0);
    // LED Controller
    static const int _numLEDS = 9; // 9 for microtesting, 300 for whole strip, 175 far large scale testing
    CRGB             _leds[_numLEDS];
    led_controller   _ledController = led_controller(_numLEDS, _led_pin, _leds);
};