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
    _server.on("/setcolor", std::bind(&WebServer::receiveLEDPattern, this));
    _server.on("/getcolor", std::bind(&WebServer::broadcastLEDPattern, this));
    _server.begin();
    return 0;
}

void WebServer::handleClient()
{
    _server.handleClient();
    syncLEDS();
}

void WebServer::receiveLEDPattern() // Called by desktop application
{
    String data_string = _server.arg("plain");
    DynamicJsonDocument jDoc(5000);
    DeserializationError err = deserializeJson(jDoc, data_string);
    if(!err)
    {
        JsonObject object = jDoc.as<JsonObject>();
        String debugStr;
        serializeJsonPretty(jDoc, debugStr);
        // Serial.println(debugStr);
        std::vector<TIP_RGB> patternBuilder;

        for(size_t i = 0; i < object["rgbDataPoints"].size(); i++)
        {
            TIP_RGB tempRGB;
            String r = object["rgbDataPoints"][i]["r"];
            String g = object["rgbDataPoints"][i]["g"];
            String b = object["rgbDataPoints"][i]["b"];

            tempRGB.r = r.toInt();
            tempRGB.g = g.toInt();
            tempRGB.b = b.toInt();

            patternBuilder.push_back(tempRGB);
        }

        _rgbPattern = patternBuilder;

        // debugPrintRGBPattern();
        _server.send(204);
        return;
    }
    Serial.println("Error Deserializing Input JSON");
}

void WebServer::broadcastLEDPattern() // Called by other NodeMCU Clients
{
    DynamicJsonDocument jDoc(5000);
    for(size_t i = 0; i < _rgbPattern.size(); i++)
    {
        StaticJsonDocument<150> rgbItem;
        rgbItem["r"] = _rgbPattern[i].r;
        rgbItem["g"] = _rgbPattern[i].g;
        rgbItem["b"] = _rgbPattern[i].b;
        jDoc["rgbDataPoints"].add(rgbItem);
    }

    String out;
    serializeJson(jDoc, out);
    String debugStr;
    serializeJsonPretty(jDoc, debugStr);
    _server.send(200, "text/plain", out);
}

void WebServer::initLEDS()
{
    // Initialize LEDS & set to debug state
    FastLED.addLeds<WS2812B, _led_pin, GRB>(_leds, _numLEDS);
    FastLED.setBrightness(45);
    _ledController.debugPattern(_leds);
}

void WebServer::syncLEDS(){
    _ledController.staticPattern(_leds, _rgbPattern);
}

// DEBUG
void WebServer::debugPrintRGBPattern()
{
    for(size_t i = 0; i < _rgbPattern.size(); i++)
    {
        Serial.print("Item: ");
        Serial.println(i);
        Serial.print("R: ");
        Serial.println(_rgbPattern[i].r);
        Serial.print("G: ");
        Serial.println(_rgbPattern[i].g);
        Serial.print("B: ");
        Serial.println(_rgbPattern[i].b);
        Serial.println("---------------------------------");
    }
    Serial.println("****_________________________________****");

}