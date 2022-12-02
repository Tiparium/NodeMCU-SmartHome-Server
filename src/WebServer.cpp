/*
  ==============================================================================

    WebServer.cpp
    Created: 1 Dec 2022 5:20:??pm
    Author:  Tiparium

  ==============================================================================
*/
#include "WebServer.h"

int WebServer::initWebServer()
{
    const char* ssid        = _data.ssid.c_str();
    const char* password    = _data.pswd.c_str();
    WiFi.begin(ssid, password);
    Serial.begin(115200);
    while(WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".'-");
        delay(60);
    } Serial.println("\n");
    Serial.println("Connecetd to: ");
    Serial.print("  SSID: ");
    Serial.println(ssid);
    Serial.print("  ON IP: " );
    Serial.println(WiFi.localIP());

    _server.on("/", [](){});
    _server.on("/setcolor", std::bind(&WebServer::setLocalColors, this));
    _server.on("/getcolor", std::bind(&WebServer::getLocalColors, this));
    _server.begin();
    return 0;
}

void WebServer::getLocalColors()
{
    DynamicJsonDocument doc(500);
    doc["r"] = _rgb.r;
    doc["g"] = _rgb.g;
    doc["b"] = _rgb.b;

    String out;
    serializeJson(doc, out);
    Serial.println("GET: ");
    debugPrintRGB();
    _server.send(200, "text/plain", out);
}

void WebServer::setLocalColors()
{
    String data_string = _server.arg("plain");
    StaticJsonDocument<400> jDoc;
    DeserializationError err = deserializeJson(jDoc, data_string);
    if(!err)
    {
        JsonObject object = jDoc.as<JsonObject>();
        String debugStr;
        serializeJsonPretty(jDoc, debugStr);

        String r = object["r"];
        String g = object["g"];
        String b = object["b"];

        _rgb.r = r.toInt();
        _rgb.g = g.toInt();
        _rgb.b = b.toInt();

        Serial.println("SET: ");
        debugPrintRGB();
        _server.send(204);
        return;
    }
    Serial.println("Error Deserializing Input JSON");
}

void WebServer::handleClient()
{
    _server.handleClient();
    syncLEDS();
}

void WebServer::initLEDS()
{
    // Initialize LEDS & set to debug state
    FastLED.addLeds<WS2812B, _led_pin, GRB>(_leds, _numLEDS);
    FastLED.setBrightness(45);
    _ledController.debugPattern(_leds);
}

void WebServer::syncLEDS(){
    CRGB color = {_rgb.r, _rgb.g, _rgb.b};
    _ledController.color(_leds, color);
}

// DEBUG
void WebServer::debugPrintRGB()
{
    Serial.print("R: ");
    Serial.println(_rgb.r);
    Serial.print("G: ");
    Serial.println(_rgb.g);
    Serial.print("B: ");
    Serial.println(_rgb.b);
    Serial.println("---------------------------------");
}