// Load Wi-Fi library
#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <FastLED.h>
#include <ESP8266WiFi.h>

#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>

#include <WiFiClient.h>
#include <ArduinoJson.h>

#include "data.h"

ESP8266WebServer server;

data _data;

uint8 _r = 0;
uint8 _g = 0;
uint8 _b = 0;

void debugPrintRGB()
{
    Serial.print("R: ");
    Serial.println(_r);
    Serial.print("G: ");
    Serial.println(_g);
    Serial.print("B: ");
    Serial.println(_b);
    Serial.println("---------------------------------");
}

void setLocalColors()
{
    String data_string = server.arg("plain");
    StaticJsonDocument<400> jDoc;
    DeserializationError err = deserializeJson(jDoc, data_string);

    if(!err)
    {
        JsonObject object = jDoc.as<JsonObject>();
        String r = object["r"];
        String g = object["g"];
        String b = object["b"];

        _r = r.toInt();
        _g = g.toInt();
        _b = b.toInt();

        debugPrintRGB();
        return;
    }
    Serial.println("Error Deserializing Input JSON");
}

void getLocalColors()
{
    DynamicJsonDocument doc(500);
    doc["r"] = _r;
    doc["g"] = _g;
    doc["b"] = _b;

    String out;
    serializeJson(doc, out);
    server.send(200, "text/plain", out);
}

void setup()
{
    const char* ssid        = _data.ssid.c_str();
    const char* password    = _data.pswd.c_str();
    WiFi.begin(ssid, password);
    Serial.begin(115200);
    while(WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".'");
        delay(100);
    } Serial.println("\n");
    Serial.println("Connecetd to: ");
    Serial.print("  SSID: ");
    Serial.println(ssid);
    Serial.print("  IP: " );
    Serial.println(WiFi.localIP());

    server.on("/", [](){server.send(200, "text/plain", "Hello World!");});
    server.on("/setcolor", setLocalColors);
    server.on("/getcolor", getLocalColors);
    server.begin();
}

void loop()
{
    server.handleClient();
}