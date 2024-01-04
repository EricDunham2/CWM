#ifndef WIFI_H
#define WIFI_H

#include "wifi.h"
#include "eepromHelper.h"
#include <Arduino.h>
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>
#include "logger.h"

 /* char ssid[32]; EEPROM 0-32
  char ssidPass[30];  EEPROM 33 - 63
  char mdnsName[10];  EEPROM 64 - 74
  char apName[32];  EEPROM 75 - 107
  char apPass[30];  EEPROM 108 - 138
*/

class WifiController {

    public:
        char ssid[32];
        char ssidPass[30];
        char mdns[10];
        char apSSID[32];
        char apPass[30];

        WifiController();
        void setLocalIp();
        void setSsid(char ssid[32]);
        void setupMDNS();
        String getSsid();
        void setSsidPass(char ssidPass[30]);
        String getSsidPass();
        void setMdns(char mdns[10]);
        String getMdns();
        void setApSsid(char apSsid[32]);
        String getApSsid();
        void setApPass(char apPass[30]);
        String getApPass();
        void setupAccesspoint();
        void setStation();
        void setDefaults();
};
#endif
