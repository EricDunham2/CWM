#include <logger.h>

void Logger::logd(String msg, int line) {
    String log = line + " => [" + msg + "]";

    if (debug) {
        Serial.println(msg);
    }

    logArr.push_back(msg);
}

void Logger::logd(String msg) {
    String log = " => [" + msg + "]";
    // lg.push_back(log);

    if (debug)
    {
        if (newline == false) {
            Serial.print(msg);
        } else {
            Serial.println(msg);
        }
    }

    logArr.push_back(msg);
}

void Logger::logd(String msg, int line)
{
    String log = line + " => [" + msg + "]";

    if (debug) {
        Serial.print(msg);
    }

    logArr.push_back(msg);
}

String Logger::getServerLogs()
{
    /*DynamicJsonDocument doc(2048);
    JsonArray data = doc.createNestedArray("logs");

    for (size_t i = 0; i < logArr.size(); i++)
    {
        data[i].add(logArr.at(i));
    }

    String output;
    serializeJson(doc, output);*/

    return "";//output;
}
