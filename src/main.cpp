#include <Arduino.h>
#include <SPI.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include "AsyncJson.h"
#include "ArduinoJson.h"
#include "index.h"
#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#include "Adafruit_DRV2605.h"

#define LED_PIN 21
#define BTN_PIN 0

// create a variable to store the current effect
uint8_t currentEffect = 123;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(1, LED_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_DRV2605 drv;

AsyncWebServer server(80);
const char *ssid = "Nesar";
const char *password = "fiberwings1431";

void notFound(AsyncWebServerRequest *request)
{
    request->send(404, "application/json", "{\"message\":\"Not found\"}");
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait)
{
    for (uint16_t i = 0; i < strip.numPixels(); i++)
    {
        strip.setPixelColor(i, c);
        strip.show();
        delay(wait);
    }
}

void setLedRed()
{
    colorWipe(strip.Color(100, 0, 100), 50); // Red
}
// Function to add a nested array to a JsonObject
void addNestedArray(JsonObject parent, const char *key, std::initializer_list<int> values)
{
    JsonArray nestedArray = parent.createNestedArray(key);
    for (int value : values)
    {
        nestedArray.add(value);
    }
}

void setup()
{

    const size_t capacity = JSON_OBJECT_SIZE(5) + 5 * JSON_ARRAY_SIZE(3); // Adjust the size based on your data
    DynamicJsonDocument doc(capacity);

    // Create a root JsonObject within the document
    JsonObject effects = doc.to<JsonObject>();
    // Add nested arrays dynamically
    addNestedArray(effects, "strong_click", {1, 2, 3});
    addNestedArray(effects, "soft_click", {4, 5, 6});
    addNestedArray(effects, "buzz", {10, 11, 12});
    addNestedArray(effects, "smooth_hum", {118, 119, 120, 121, 122, 123});

    Serial.begin(115200);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    while (WiFi.waitForConnectResult() != WL_CONNECTED)
    {
        Serial.printf("WiFi Failed!\n");
    }
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    // Initialize LED to red
    setLedRed();
    if (!drv.begin())
    {
        Serial.println("Could not find DRV2605");
        while (1)
            delay(10);
    }
    drv.selectLibrary(1);
    drv.setMode(DRV2605_MODE_INTTRIG);

    // HOME PAGE
    server.on("/", HTTP_GET, [effects](AsyncWebServerRequest *request)
              { request->send(200, "text/html", home_page); });

    // Get Effect REQUEST with effect name parameter and make that string lowercase
    server.on("/get-effect", HTTP_GET, [effects](AsyncWebServerRequest *request)
              {
        StaticJsonDocument<100> data;

        if (request->hasParam("effect")) {
            String newEffect = request->getParam("effect")->value();
            newEffect.toLowerCase();

            // check if the effect exists in the effects object
            if (effects.containsKey(newEffect)) {
                // get the effect array
                JsonArray effect = effects[newEffect];
                // if the newEffect exists in the effects object, get the effect size from the array
                
                    int effectSize = effect.size();
                    uint8_t effectNumber = effect[0];
                    // if the request has a var parameter, get the effect number from the array
                    if(request->hasParam("var")){
                        int var = request->getParam("var")->value().toInt();
                        effectNumber = effect[var];
                    } 
                    // get the effect number from the array
                    
                    // set the current effect to the effect number
                    currentEffect = effectNumber;
                    data["effect"] = newEffect;
                    data["effectNumber"] = effectNumber;
                    data["effectSize"] = effectSize;
                
            } else {
                data["effect"] = "smooth_hum";
                data["effectNumber"] = 123;
                data["effectSize"] = 6;
            } 
        } else {
            data["effect"] = "smooth_hum";
            data["effectNumber"] = 123;
            data["effectSize"] = 6;
        }

        String response;
        serializeJson(data, response);
        request->send(200, "application/json", response); });

    // Controll Get REQUEST
    server.on("/effects", HTTP_GET, [effects](AsyncWebServerRequest *request)
              {
        StaticJsonDocument<100> data;

        // get the effects object, and send it to the client as json data
        data["effects"] = effects;
        String response;
        serializeJson(data, effects);
        request->send(200, "application/json", response); });

    // Controll Post REQUEST
    AsyncCallbackJsonWebHandler *handler = new AsyncCallbackJsonWebHandler("/post-message", [effects](AsyncWebServerRequest *request, JsonVariant &json)
                                                                           {
        StaticJsonDocument<200> data;
        if (json.is<JsonArray>()) {
            data = json.as<JsonArray>();
        } else if (json.is<JsonObject>()) {
            data = json.as<JsonObject>();
        }
        String response;
        serializeJson(data, response);
        request->send(200, "application/json", response);
        Serial.println(response); });
    server.addHandler(handler);

    // Not Found PAGE
    server.onNotFound(notFound);
    server.begin();
}

void loop()
{

    // set the effect to play
    drv.setWaveform(0, currentEffect); // play effect
    drv.setWaveform(1, 0);             // end waveform

    // play the effect!
    drv.go();

    // wait a bit
    delay(500);
}
