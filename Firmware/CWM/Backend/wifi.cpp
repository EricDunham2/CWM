#include "wifi.h"

Logger logger = Logger();

WifiController::WifiController() {
}

void WifiController::setLocalIp()
{
    IPAddress local_IP(192,168,2,100);
    IPAddress gateway(192,168,2,1);
    IPAddress subnet(255, 255, 0, 0);
    IPAddress primaryDNS(8, 8, 8, 8);   // optional
    IPAddress secondaryDNS(8, 8, 4, 4); // optional

    if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS))
    {
        logger.logln("AP Failed to configure" , __LINE__);
    }
}

void WifiController::setMdns(char mdns[10])
{
    String mdnsStr = String(mdns);
    mdnsStr.toCharArray(mdns, 10);
    ArduinoOTA.begin();
    ArduinoOTA.setHostname(mdns);

    if (mdnsStr == NULL || mdnsStr == "" || mdnsStr.length() < 10)
    {
        EEPROMHelper::saveObject(64, "Sisyphus");

        //EEPROM.put(64, "sisyphus");
        //EEPROM.commit();
        if (MDNS.begin("sisyphus"))
        {
            logger.logln("MDNS started width" , __LINE__);
        }
    }
    else if (MDNS.begin(mdns))
    {
        EEPROMHelper::saveObject(64, mdns);
        logger.logln("MDNS started" , __LINE__);
    }
}

void WifiController::setupMDNS() {
    String mdnsStr = getMdns();
    mdnsStr.toCharArray(mdns, 10);
    ArduinoOTA.begin();
    ArduinoOTA.setHostname(mdns);

    if (mdnsStr == NULL || mdnsStr == "" || mdnsStr.length() < 10)
    {
        if (MDNS.begin("sisyphus"))
        {
            logger.logln("MDNS started with default value" , __LINE__);
        }
    }
    else if (MDNS.begin(mdns))
    {
        logger.logln("MDNS started" , __LINE__);
    }
}

void WifiController::setSsid(char ssid[32])
{
    EEPROMHelper::saveObject(0, ssid);
    logger.logln("Saving SSID: " + String(ssid) , __LINE__);
}

String WifiController::getSsid()
{
    char ssid[32];
    EEPROMHelper::getObject(0, ssid);

    logger.logln("SSID: " + String(ssid) , __LINE__);

    return String(ssid);
}

void WifiController::setSsidPass(char ssidPass[30])
{
    EEPROMHelper::saveObject(33, ssidPass);
    logger.logln("Saving SSID Pass: " + String(ssidPass), __LINE__);
}

String WifiController::getSsidPass()
{
    char ssidPass[30];   
    EEPROMHelper::getObject(33, ssidPass);

    logger.logln("SSID: " + String(ssidPass), __LINE__);

    return String(ssidPass);
}

String WifiController::getMdns()
{
    char mdns[10];
    EEPROMHelper::getObject(64, mdns);

    logger.logln("mDNS: " + String(mdns), __LINE__);

    return String(mdns);
}

void WifiController::setApSsid(char apSsid[32])
{
    EEPROMHelper::saveObject(75, apSsid);
    logger.logln("Saving AP SSID: " + String(apSsid), __LINE__);
}

String WifiController::getApSsid()
{
    char apSsid[32];
    EEPROMHelper::getObject(75, apSsid);
    logger.logln("Accespoint SSID:" + String(apSsid), __LINE__);

    return String(apSsid);
}

void WifiController::setApPass(char apPass[30])
{
    EEPROMHelper::saveObject(108, apPass);
    logger.logln("Saving AP Pass: " + String(apPass), __LINE__);

}

String WifiController::getApPass()
{
    char apPass[30];
    EEPROMHelper::getObject(108, apPass);

    logger.logln("Accespoint SSID Pass:" + String(apPass), __LINE__);

    return String(apPass);
}

void WifiController::setupAccesspoint()
{
    WiFi.mode(WIFI_AP);

    char apSsid[32];
    char apPass[30];

    String apSsidStr = getApSsid();
    String apPassStr = getApPass();

    if (apSsidStr == NULL || apSsidStr == "" || apPassStr == NULL || apPassStr == "" || apPassStr.length() < 8)// || apPass. < 8)
    {
        String("WS281X").toCharArray(apSsid, 12);
        String("LedControls").toCharArray(apPass, 12);
    }

    apSsidStr.toCharArray(apSsid, 32);
    apPassStr.toCharArray(apPass, 30);

    boolean result = WiFi.softAP(apSsid, apPass);

    if (result == 1)
    {
        logger.logln("Accesspoint setup", __LINE__);
    }

    IPAddress IP = WiFi.softAPIP();

    logger.logln("IP Address: " + IP.toString(), __LINE__);

    delay(100);
}

void WifiController::setDefaults() {
    char ssidDefault[32] = "SSID Default";
    char ssidPassDefault[30] = "SSID Password";
    char mDNSDefault[10] = "sisyphus";
    char apDefault[32] = "WS281X Controller";
    char apPassDefault[30] = "LedController";

    EEPROMHelper::saveObject(0, ssidDefault);
    EEPROMHelper::saveObject(33, ssidPassDefault);
    EEPROMHelper::saveObject(64, mDNSDefault);
    EEPROMHelper::saveObject(75, apDefault);
    EEPROMHelper::saveObject(108, apPassDefault);
}

void WifiController::setStation()
{
    logger.logln("Setting up as Station", __LINE__);

    WiFi.softAPdisconnect(true);

    //setDefaults();

    String ssid = getSsid();
    String ssidPass = getSsidPass();

    char ssidArr[32];
    char ssidPassArr[30];

    ssid.toCharArray(ssidArr, 32);
    ssidPass.toCharArray(ssidPassArr, 30);

    //setLocalIp();

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssidArr, ssidPassArr);

    logger.logln("Waiting for connection", __LINE__);

    if (WiFi.waitForConnectResult() != WL_CONNECTED)
    {
        logger.logln("WiFi Failed!", __LINE__);

        setupAccesspoint();

        return;
    }

    int retryAttempts = 0;
    logger.logln("Connected", __LINE__);

    while (WiFi.status() != WL_CONNECTED)
    {
        if (retryAttempts == 10)
        {
            return;
        }

        delay(500);
        Serial.print(".");
    }

    logger.logln("IP Address: ", __LINE__, false);
    logger.logln(WiFi.localIP().toString());
    delay(100);
}
