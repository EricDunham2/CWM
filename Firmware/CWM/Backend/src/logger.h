#ifndef LOGGER_H
#define LOGGER_H

#include <Arduino.h>
#include <ArduinoJson.h>

using namespace std;

class Logger
{
    protected:
        bool debug = false;

    public:
        vector<String> logArr;
        Logger();
        void logln(String msg, int line);
        void logln(String msg);
        void logd(String msg, int line);
        void logd(String msg);
        String getServerLogs();
};

#endif