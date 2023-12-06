
#include "FS.h"
#include <ESPAsyncTCP.h>
#include <ESP8266WiFi.h>
#include "ESPAsyncWebServer.h"
#include <Arduino.h>
#include <ArduinoJson.h>
#include "wifi.h"
#include "eepromHelper.h"
#include <AsyncElegantOTA.h>

#include "logger.h"

extern const uint8_t data_bootStrapJs[] PROGMEM;
// extern const uint8_t data_chromaJs[] PROGMEM;
extern const uint8_t data_iconifyJs[] PROGMEM;
extern const uint8_t data_materialWoff[] PROGMEM;
extern const uint8_t data_materialCss[] PROGMEM;
extern const uint8_t data_toastrCss[] PROGMEM;

private boolean running = false;

// Create Server
AsyncWebServer server(80);
WifiController wifiController = WifiController();

Logger logger = Logger();

uint16_t DeserializationCheck(DeserializationError error) {
  switch (error.code()) {
  case DeserializationError::Ok:
    logger.logln(("Deserialization succeeded"), __LINE__);
    return 200;
  case DeserializationError::InvalidInput:
    logger.logln(("Invalid input!"), __LINE__);
    return 500;
  case DeserializationError::NoMemory:
    logger.logln(("Not enough memory"), __LINE__);
    return 500;
  default:
    logger.logln(("Deserialization failed"), __LINE__);
    return 500;
  }
}

uint16_t wind(uint8_t speed, float gauge, uint16_t numberOfWindings) {
  bool shaft = false;

    /*
      Replace this part of the code with 

      G = Gauge
      R = Rotation
      Ma = R
      Mb = (1/G * R)

      digitalWrite(STEP_PIN, HIGH);
      delayMicroseconds(165);
      digitalWrite(STEP_PIN, LOW);
      delayMicroseconds(165);
    */

  for (uint16_t i = numberOfWindings; i > 0; i--) {
    digitalWrite(STEP_PIN, HIGH);
    delayMicroseconds(speed / 2);
    digitalWrite(STEP_PIN, LOW);
    delayMicroseconds(speed / 2);

    if (readStartEndstop() == true || readEndEndstop() == true) {
      shaft = !shaft; // reverse directions
      driver.shaft(shaft);
    }
  }
}

uint16_t homing() {
  bool homeEndstopFound = false;
  bool endEndstopFound = false;

  while (!homeEndstopFound) {
    if (readStartEndstop()) {
      homeEndstopFound = true
      break;
    } else if (readEndEndstop() && !endEndstopFound) {
      shaft = !shaft; // reverse directions
      driver.shaft(shaft);
      endEndstopFound = true;
    }
  }

  while (readStartEndstop()) {
    digitalWrite(STEP_PIN, HIGH);
    delayMicroseconds(165);
    digitalWrite(STEP_PIN, LOW);
    delayMicroseconds(165);
  }
}

uint16_t setCoilSettings(String windingSettingsStr) {
  logger.logln("Starting winding", __LINE__);

  running = true;

  uint8_t speed;
  float gauge;
  uint16_t numberOfWindings;

  DynamicJsonDocument params(1024);
  DeserializationError error = deserializeJson(params, windingSettingsStr.c_str());

  uint16_t status = DeserializationCheck(error);

  if (status != 200) {
    return status;
  }

  logger.logln("Setting Winding Settings", __LINE__);

  params["speed"].as<int>(speed);
  params["gauge"].as<float>(gauge);
  params["count"].as<int>(numberOfWindings);

  logger.logln("Returning Status 200", __LINE__);

  startWinding(speed, gauge, numberOfWindings);

  running = false;

  return 200;
}

void setup()
{
  Serial.begin(9600);

  EEPROMHelper::begin(2048);

  logger.logln("Setting up SPIFFS", __LINE__);

  if (!SPIFFS.begin()) {
    logger.logln("An Error has occurred while mounting SPIFFS", __LINE__);
    return;
  }

  wifiController.setStation();
  wifiController.setupMDNS();

  server.on("/windCoil", HTTP_POST, [](AsyncWebServerRequest *request) {
    String coilParams = (request->getParam("data", true)->value());
    setCoilSettings(coilParams);
    request->send(200, "application/json", ""); 
  });

  server.on("/getStatus", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "application/json", running);
  });

  /* Website Files */
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) { 
    request->send(SPIFFS, "/index.html", "text/html"); 
  });

  server.on("/static/js/app.7aa973b0b0a35d80fda7.js", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/app.js", "text/javascript");
  });

  server.on("/static/js/manifest.2ae2e69a05c33dfc65f8.js", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/manifest.js", "text/javascript");
  });

  server.on("/static/js/vendor.850a05f7ad8242b66bd3.js", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/vendor.js", "text/javascript");
  });

  server.on("/static/css/app.6c143ddd65509e57f7d8740e2db95d6d.css", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/app.css", "text/css");
  });

  server.on("/static/css/bootstrap.min.css", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/bootstrap.min.css", "text/css");
  });

  server.on("/static/js/bootstrap.min.js", HTTP_GET, [](AsyncWebServerRequest *request) {
    AsyncWebServerResponse *response = request->beginResponse_P(200, "text/javascript", data_bootStrapJs, sizeof(data_bootStrapJs));
    request->send(response);
  });

  server.on("/static/font/material.woff2", HTTP_GET, [](AsyncWebServerRequest *request) {
    AsyncWebServerResponse *response = request->beginResponse_P(200, "font/woff2", data_materialWoff, sizeof(data_materialWoff));
    request->send(response);
  });

  server.on("/static/css/materialicons.css", HTTP_GET, [](AsyncWebServerRequest *request) {
    AsyncWebServerResponse *response = request->beginResponse_P(200, "text/css", data_materialCss, sizeof(data_materialCss));
    request->send(response);
  });

  server.on("/static/js/toastr.css", HTTP_GET, [](AsyncWebServerRequest *request) {
    AsyncWebServerResponse *response = request->beginResponse_P(200, "text/css", data_toastrCss, sizeof(data_toastrCss));
    request->send(response);
  });

  /* End Website Files */
  AsyncElegantOTA.begin(&server); // Start ElegantOTA

  server.begin();

  Serial.println("Server started");

  delay(100);

  logger.logln("Starting Server", __LINE__);
}

void loop() {
  MDNS.update();
  yield();
}